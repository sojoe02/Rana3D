--begin_license--
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
Stat = require "ranalib_statistic"
Move = require "ranalib_movement"
Agent = require "ranalib_agent"

-- Initialization of the agent.
function initializeAgent()
	say("Agent #: " .. ID .. " has been initialized")

	if Moving ~= true then
		say("I am not moving".. STEP_RESOLUTION.. ":" .. PositionX)
	end



	PositionX=0
	PositionY=0
	PositionZ=0
	Radius = 0.1

	--Moving = true
	--DestinationX = 1
	--DestinationY = 1
	--Speed = 3
end


function takeStep()

	--Agent.changeColor{r=200,g=55,b=0}
	if not Moving then


		local x = Stat.randomInteger(-1, 1)
		local y = Stat.randomInteger(-1, 1)
		local z = Stat.randomInteger(-1, 1)

		PositionX = PositionX+x
		PositionY = PositionY+y
		PositionZ = PositionZ+z
		if PositionX > 1000 then
			PositionX = 1000
		elseif PositionX < -1000 then
			PositionX = -1000
		end
		if PositionY > 1000 then
			PositionY = 1000
		elseif PositionY < -1000 then
			PositionY = -1000
		end
		if PositionZ > 1000 then
			PositionZ = 1000
		elseif PositionZ < -1000 then
			PositionZ = -1000
		end

	end
end

