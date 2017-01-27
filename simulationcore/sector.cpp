//--begin_license--
//
//Copyright 	2013 	Søren Vissing Jørgensen.
//				2014-2016	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.
//
//This file is part of RANA.
//
//RANA is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//RANA is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with RANA.  If not, see <http://www.gnu.org/licenses/>.
//
//--end_license--

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <api/phys.h>
//#include <vector>

#include "lua.hpp"
#include "lauxlib.h"
#include "lualib.h"

#include "sector.h"
#include "interfacer.h"

#include "ID.h"
#include "supervisor.h"
#include "output.h"
#include "../api/phys.h"
#include "../api/fieldphys.h"

Sector::Sector(double posX, double posY, double posZ, double width, double height, Supervisor* master, int id)
    :initAmount(0),master(master), posX(posX), posY(posY), posZ(posZ),width(width),height(height),id(id)
{

}

Sector::~Sector()
{
}

void Sector::populate(int LUASize ,std::string filename)
{
    for(int i=0; i<LUASize; i++)
    {

        if(Output::KillSimulation.load()) return;
        double xtmp = Phys::getMersenneFloat(0,width);
        double ytmp = Phys::getMersenneFloat(0,height);
        double ztmp = Phys::getMersenneFloat(0,depth);

        std::shared_ptr<AgentLuaInterface> luaPtr =
                std::make_shared<AgentLuaInterface>(ID::generateAgentID(), xtmp, ytmp,
                                                    ztmp, this, filename);

        luaAgents.insert(std::make_pair(luaPtr->getID(), luaPtr));
        Interfacer::addLuaAgentPtr(luaPtr);

        luaPtr->InitializeAgent();
    }
}

/**
 * Get the X and Y positions of all actors
 * Retrieves the information from all actors, writes the information to the lists given.
 * @param agentinfo address of array that holds the info needed for graphic rendering.
 */
void Sector::retrievePopPos(std::list<agentInfo> &infolist){

    for(const auto &lua : luaAgents)
    {

        if(master->removedIDs.find(lua.second->getID()) ==
                master->removedIDs.end())
        {
            agentInfo info = lua.second->getAgentInfo();

            infolist.push_back(info);

        }
    }
}

void Sector::fieldMovement(std::list<agentInfo> &infolist){
    for(const auto &lua : luaAgents)
    {
        agentInfo info = lua.second->getAgentInfo();
        info.thrust = lua.second->thrust;
        info.momentum = true;//lua.second->momentum;

        double xThrust = 0;
        double yThrust = 0;
        double zThrust = 0;

        if ( memory::lastPositions->find(info.id) == memory::lastPositions->end() ) {
            memory::lastPositions->insert(std::make_pair(info.id,info));
            std::vector<double> init_mom = {0,0,0};
            memory::momentum->insert(std::make_pair(info.id,init_mom));
        }

        std::vector<double> edit = {0,0,0};
        if(info.mass!=0){
            edit=fieldEffects(info, Phys::getScale());
        }
        if(info.thrust!=0){
            double totVel=sqrt(pow(memory::momentum->find(info.id)->second[0],2)+pow(memory::momentum->find(info.id)->second[1],2)+pow(memory::momentum->find(info.id)->second[2],2));
            if(totVel!=0){
                xThrust = info.thrust*memory::momentum->find(info.id)->second[0]/totVel;
                yThrust = info.thrust*memory::momentum->find(info.id)->second[1]/totVel;
                zThrust = info.thrust*memory::momentum->find(info.id)->second[2]/totVel;
            }
        }
        if (info.momentum){
            info.x=info.x+edit[0]+memory::momentum->find(info.id)->second[0]+xThrust;
            info.y=info.y+edit[1]+memory::momentum->find(info.id)->second[1]+yThrust;
            info.z=info.z+edit[2]+memory::momentum->find(info.id)->second[2]+zThrust;
            info.shiftX = edit[0]+memory::momentum->find(info.id)->second[0]+xThrust;
            info.shiftY = edit[1]+memory::momentum->find(info.id)->second[1]+yThrust;
            info.shiftZ = edit[2]+memory::momentum->find(info.id)->second[2]+zThrust;
        }else{
            info.x=info.x+edit[0]+xThrust;
            info.y=info.y+edit[1]+yThrust;
            info.z=info.z+edit[2]+zThrust;
            info.shiftX = edit[0]+xThrust;
            info.shiftY = edit[1]+yThrust;
            info.shiftZ = edit[2]+zThrust;
        }

        std::vector<double> vel;
        vel.push_back(info.x-memory::lastPositions->find(info.id)->second.x);
        vel.push_back(info.y-memory::lastPositions->find(info.id)->second.y);
        vel.push_back(info.z-memory::lastPositions->find(info.id)->second.z);
        memory::lastPositions->at(info.id)=info;
        memory::momentum->at(info.id)=vel;

        infolist.push_back(info);

    }

    Interfacer::modifyAgentInfo(infolist);
}

/**
 * Event initiation phase
 * Queries all autons on whether they are going create be an event or not, if
 * an event is initated it will be added the masters eventqueue.
 * @param macroResolution the resolution of the macrostep (microStepRes * macroFactor)
 */
void Sector::takeStepPhase(unsigned long long tmu)
{
    for(const auto &lua : luaAgents)
    {
        int macroFactorMultipler = lua.second->getMacroFactorMultipler();

        if(macroFactorMultipler > 0 &&
                tmu%(macroFactorMultipler*Phys::getMacroFactor()) == 0 )
        {
            std::unique_ptr<EventQueue::eEvent> eevent =
                    lua.second->takeStep();


            if(eevent != NULL)
            {
                master->receiveEEventPtr(std::move(eevent));
            }
        }
    }

    if(!removalIDs.empty())
    {
        //remove all autons set for removal
        for(const auto &r : removalIDs)
        {
            auto itrLua = luaAgents.find(r);
            if(itrLua != luaAgents.end())
            {
                luaAgents.erase(r);
            }
        }
        removalIDs.clear();
    }

    for(const auto &agent : newAgents)
    {
        luaAgents.insert(std::make_pair(agent->getID(),agent));
        Interfacer::addLuaAgentPtr(agent);
        agent->InitializeAgent();


    }
    newAgents.clear();

}

/**
 * Event distribution phase.
 * Recieves an eventlist from the Master to distribute among local autons
 * @param event EventQueue ptr holding external events.
 */
void Sector::distroPhase(const EventQueue::eEvent* event)
{
    for(const auto &lua : luaAgents)
    {
        if(event->originID != lua.second->getID() &&
                (event->targetGroup == 0 ||
                 lua.second->checkGroup(event->targetGroup) == true))
        {
            std::unique_ptr<EventQueue::iEvent> ieventPtr =
                    lua.second->processEvent(event);

            if(ieventPtr != NULL)
            {
                master->incrementEEventCounter(event->id);
                master->receiveIEventPtr(std::move(ieventPtr));
            }
        }
    }
}

void Sector::simDone()
{
    for(auto itlua = luaAgents.begin(); itlua !=luaAgents.end(); itlua++)
    {
        itlua->second->simDone();
    }
}

//perform an event for an auton in question:
void Sector::performEvent(std::unique_ptr<EventQueue::eEvent> event)
{
    master->receiveEEventPtr(std::move(event));
}


int Sector::addAgent(double x, double y, double z,
                     std::string filename, std::string type="Lua")
{
    int id = ID::generateAgentID();

    if(type.compare("Lua") == 0)
    {
        std::shared_ptr<AgentLuaInterface> luaPtr =
                std::make_shared<AgentLuaInterface>(id, x, y, 1, this, filename);

        if(Output::SimRunning)
        {
            newAgents.push_back(luaPtr);
        }
        else
        {
            luaAgents.insert(std::make_pair(luaPtr->getID(),luaPtr));
            Interfacer::addLuaAgentPtr(luaPtr);
            luaPtr->InitializeAgent();
        }

    }

    return id;
}

bool Sector::removeAgent(int arg_id)
{
    auto luaItr = luaAgents.find(arg_id);
    if(luaItr != luaAgents.end())
    {
        luaItr->second->setRemoved();
        removalIDs.push_back(arg_id);
        return true;
    } else return false;

}
