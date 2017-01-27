#include <mutex>

#include "interfacer.h"
#include "output.h"

Supervisor* Interfacer::master = NULL;
std::map<int, std::string> Interfacer::agentFilenames;
std::map<int, std::shared_ptr<AgentLuaInterface>> Interfacer::agents;
std::mutex Interfacer::eventMutex;
std::mutex Interfacer::agentMutex;
std::mutex Interfacer::agentPtrMutex;

void Interfacer::initInterfacer(Supervisor *arg_master)
{
	master = arg_master;
    agentFilenames.clear();
    agents.clear();
	//activesector = arg_activesector;

}

int Interfacer::addLuaAgent(double x, double y, double z, std::string path, std::string filename)
{
    std::lock_guard<std::mutex> guard(agentMutex);

	int id = master->addAgent(x, y, z, path, filename, "Lua");
	//Output::Inst()->addGraphicAgent(id,x,y);

	return id;
}

std::map<int, std::string> Interfacer::getAgentInfo()
{
	return agentFilenames;
}

bool Interfacer::removeAgent(int Id)
{
    std::lock_guard<std::mutex> guard(agentMutex);

	if(master->removeAgent(Id))
	{
		Output::Inst()->removeGraphicAgent(Id);
		return true;
	} return false;
}

void Interfacer::addLuaAgentPtr(std::shared_ptr<AgentLuaInterface> luaPtr)
{
    std::lock_guard<std::mutex> guard(agentPtrMutex);
    agents.insert(make_pair(luaPtr->getID(), luaPtr));
}

void Interfacer::submitEEvent(std::unique_ptr<EventQueue::eEvent> eEvent)
{
    std::lock_guard<std::mutex> guard(eventMutex);

    master->receiveEEventPtr(std::move(eEvent));
}

std::shared_ptr<AgentLuaInterface> Interfacer::getAgentPtr(int id)
{
    std::lock_guard<std::mutex> guard(agentPtrMutex);

	auto itr = agents.find(id);

	if(itr != agents.end())
	{
		return itr->second;
	} else return NULL;
}

void Interfacer::modifyAgentInfo(std::list<agentInfo> infolist)
{
	for(agentInfo &info : infolist)
	{
        auto agent = agents.find(info.id);

        if (agent!=agents.end()){
        agent->second->posX=info.x;
        agent->second->posY=info.y;
        agent->second->posZ=info.z;
        agent->second->shiftX=info.shiftX;
        agent->second->shiftY=info.shiftY;
        agent->second->shiftZ=info.shiftZ;
        }else{
            std::cout<<"Error: Planet not found."<<std::endl;
        }
	}
}

std::vector<double> Interfacer::getShift(int id){
    auto &agent = agents[id];
    std::vector<double> oneShift;
    oneShift.push_back(agent->shiftX);
    oneShift.push_back(agent->shiftY);
    oneShift.push_back(agent->shiftZ);
return oneShift;
}




