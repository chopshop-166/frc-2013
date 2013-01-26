/*******************************************************************************
*  Project   		: ChopShop 13
*  File Name  		: Autonomous.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 26, 2012
*  File Description	: Robot code to execute in autonomous mode
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2010.  All Rights Reserved.          */
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
	while( (proxy = Proxy::getInstance()) == NULL ) {
		Wait(AUTONOMOUS_WAIT_TIME);
	}
	
	while( lHandle->IsAutonomous() ) {
 switch(state){
           case INIT:
        	   lHandle->DriverStationDisplay("We are Initializing");  
               OffsetValue = proxy->get ("TargetOffset");
        	   state = ALIGNING;
        	   break;
           case ALIGNING:
        	   while(OffsetValue =! 0){
        	   if(OffsetValue > 0){
        		   proxy->set(JOY_RIGHT_Y,ALIGNMENT_SPEED);}
        	   if(OffsetValue < 0){
        		   proxy->set(JOY_LEFT_Y,ALIGNMENT_SPEED);}}
        	   state = DRIVE;
        	   break;
           case DRIVE:
        	   FrontDistance = proxy->get (FRONT_DISTANCE);
        	   OffsetValue = proxy->get ("TargetOffset");
        	   if (FrontDistance > DUMP_DISTANCE){
        		   if(OffsetValue > 0){
        			   proxy->set(JOY_LEFT_Y, AUTO_SPEED);
        			   proxy->set(JOY_RIGHT_Y,AUTO_SPEED_ALIGN);}
        		   if(OffsetValue < 0){
        			   proxy->set(JOY_LEFT_Y, AUTO_SPEED_ALIGN);
        			   proxy->set(JOY_RIGHT_Y,AUTO_SPEED);}
        		   }
        	   if(OffsetValue = 0){
        		   proxy->set(JOY_LEFT_Y, AUTO_SPEED);
        	   	   proxy->set(JOY_RIGHT_Y,AUTO_SPEED);}
 	 	 	   state = DUMP;
 	 	 	   break;
 	 	   case DUMP:
 	 		   proxy->set(JOY_COPILOT_EJECT,1);
 	 		   break;
	}
        	   

		Wait(AUTONOMOUS_WAIT_TIME);
	}
}
