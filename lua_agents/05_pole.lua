----begin_license--
--
--Copyright 	2013 	Søren Vissing Jørgensen.
--			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.  
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
-- ID -- id of the agent.
-- PositionX --	this agents x position.
-- PositionY -- this agents y position.
-- STEP_RESOLUTION 	-- resolution of steps, in the simulation core.
-- EVENT_RESOLUTION	-- resolution of event distribution.
-- StepMultiple 	-- amount of steps to skip.


-- Import valid Rana lua libraries.
Event = require "ranalib_event"
Agent = require "ranalib_agent"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

	l_debug("Pole agent #: " .. ID .. " has been initialized")
	Radius = 0.01

end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)
	
	--Event.emit{targetID=sourceID, speed=0, description="echo"}
	--l_debug("Heard a")
	--l_debug(eventDescription)
	--if eventDescription=="beep" then
		Event.emit{speed=0,description="echo"}
	--	l_debug("Echo!")
	--end

end

function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end

