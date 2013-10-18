/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Autonomous.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 26, 2013
*  File Description	: Robot code to execute in autonomous mode
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2013.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

#include "Robot.h"
#include "Autonomous.h"

// To locally enable debug printing: set true, to disable false
#define DPRINTF if(false)dprintf

AutonomousTask::AutonomousTask() {
	// Create handles for proxy and robot
	Robot *lHandle;
	Proxy *proxy;
	// Register robot handle
	while( !(lHandle = Robot::getInstance()) && !( lHandle->IsAutonomous() ) ) {
		Wait(AUTONOMOUS_WAIT_TIME);
	}
	
	// Register proxy handle
	wait_count = 0;
	while( (proxy = Proxy::getInstance()) == NULL ) {
		Wait(AUTONOMOUS_WAIT_TIME);
	}
	//hi ben! How are you?
	while( lHandle->IsAutonomous() ) {
		proxy->set(JOY_COPILOT_FIRE_AUTO , -.5);
		/*
		if (wait_count >= 50) {
			proxy->set(JOY_COPILOT_FIRE_AUTO,0);
		} else if (wait_count >= 30) {
			proxy->set(JOY_COPILOT_FIRE_AUTO, -1);
		} else if (wait_count >= 20) {
			proxy->set(JOY_COPILOT_FIRE_AUTO,0);
		}
		*/
		Wait(AUTONOMOUS_WAIT_TIME);
	}
	wait_count = 0;
}

