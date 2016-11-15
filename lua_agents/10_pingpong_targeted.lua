----begin_license--
--
--Copyright 	2013 - 2016 	Søren Vissing Jørgensen.
--
--This file is part of RANA.
--
--RANA is free software: you can redistribute it and/or modify
--it under the terms of the GNU General Public License as published by
--the Free Software Foundation, either version 3 of the License, or
--(at your option) any later version.
--
--RANA is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--GNU General Public License for more details.
--
--You should have received a copy of the GNU General Public License
--along with RANA.  If not, see <http://www.gnu.org/licenses/>.
--
----end_license--

--The following global values are set via the simulation core:
-- ------------------------------------
-- IMMUTABLES.
-- ------------------------------------
-- ID -- id of the agent.
-- STEP_RESOLUTION 	-- resolution of steps, in the simulation core.
-- EVENT_RESOLUTION	-- resolution of event distribution.
-- ENV_WIDTH -- Width of the environment in meters.
-- ENV_HEIGHT -- Height of the environment in meters.
-- ------------------------------------
-- VARIABLES.
-- ------------------------------------
-- PositionX	 	-- Agents position in the X plane.
-- PositionY	 	-- Agents position in the Y plane.
-- DestinationX 	-- Agents destination in the X plane. 
-- DestinationY 	-- Agents destination in the Y plane.
-- StepMultiple 	-- Amount of steps to skip.
-- Speed 			-- Movement speed of the agent in meters pr. second.
-- Moving 			-- Denotes wether this agent is moving (default = false).
-- GridMove 		-- Is collision detection active (default = false).
-- ------------------------------------

-- Import valid Rana lua libraries.
Event = require "ranalib_event"
Shared = require "ranalib_shared"	
Agent = require "ranalib_agent"
Stat = require "ranalib_statistic"

group = 0
joined = false

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

	say("Agent #: " .. ID .. " has been initialized")
	group = 1--Stat.randomInteger(1,5)
	PositionZ=Stat.randomInteger(-ENV_WIDTH,ENV_WIDTH)
	PositionY=Stat.randomInteger(-ENV_WIDTH,ENV_WIDTH)
	PositionX=Stat.randomInteger(-ENV_WIDTH,ENV_WIDTH)
	Agent.joinGroup(group)


end

function handleEvent(sourceX, sourceY, sourceZ, sourceID, eventDescription, eventTable)
	
	--if eventDescription == "ping" and ID ~= 1 then
	
	--	l_print("Agent: "..ID .." received a ping from: "..sourceID ..", saying: "..eventTable.msg)
	--	Event.emit{targetID=sourceID, speed=343, description="pong", targetGroup=group}

	--elseif eventDescription == "pong" then
		
	--	l_print("Agent: "..ID.." received a pong from agent: ".. sourceID)
	--end

	--l_debug("Heard something from "..eventTable.msg)
	direction=math.sqrt(sourceX*sourceX+sourceY*sourceY+sourceZ*sourceZ)
	if direction>0.1 then
	PositionX=PositionX+10*(sourceX-PositionX)/direction
	PositionY=PositionY+10*(sourceY-PositionY)/direction
	PositionZ=PositionZ+10*(sourceZ-PositionZ)/direction
end



end

function takeStep()

	--if not joined then 
	--	Agent.joinGroup(group)
	--	say("Agent #"..ID.." joined a group "..group )
	--	joined = true
	--end
	Agent.changeColor{r=255,g=0,b=0}

	if Stat.randomInteger(1,1000) <= 1 and ID==1 then
	 	--Event.emit{speed=0, description="ping",table={msg="I am agent "..ID}, targetGroup=group}
	 	Event.emit{speed=343,description="ping",targetGroup=group,table={msg="hello?"}} --eventTable and ping are nil values.
	 	PositionX=PositionX+Stat.randomInteger(-10,10)
	 	PositionY=PositionY+Stat.randomInteger(-10,10)
	 	PositionZ=PositionZ+Stat.randomInteger(-10,10)
	end

end

function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end

