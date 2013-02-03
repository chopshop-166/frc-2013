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
	           case INIT: //initializes autonomous
	        	   lHandle->DriverStationDisplay("We are Initializing");  
	               OffsetValue = proxy->get ("TARGETOFFSET");
	        	   state = ALIGNING;
	        	   break;
	        	   
	           case ALIGNING: //points the robot towards the goal using an offset provided by the camera
	        	   while(OffsetValue =! 0 && proxy->get("VALID_IMAGE") == 1){
		        	  OffsetValueAlign = proxy->get("TARGETOFFSET");
		        	  AlignSpeedAlign = OffsetValueAlign * ALIGN_SPEED_CONST + AUTO_SPEED;
		        	  if (OffsetValue > DEAD_LEFT){
		        	      proxy->set(JOY_LEFT_Y,AUTO_SPEED);
		        	      proxy->set(JOY_RIGHT_Y,AlignSpeedAlign);}
		        	  if(OffsetValue < DEAD_RIGHT){ //If the robot is straying towards the left, make the left wheels faster.
		        	  	  proxy->set(JOY_LEFT_Y, AlignSpeedAlign);
		        	  	  proxy->set(JOY_RIGHT_Y, AUTO_SPEED);}		   
		        	  if(OffsetValue < DEAD_LEFT && OffsetValue > DEAD_RIGHT){ //If the robot is going straight toward the goal, move straight.
		        	  	  proxy->set(JOY_LEFT_Y, AUTO_SPEED);
		        	  	  proxy->set(JOY_RIGHT_Y, AUTO_SPEED);}  
		        	  }
		        	  
	        		   /*if(OffsetValue > 0){ // If the goal is to the left, turn left.
		        		   proxy->set(JOY_RIGHT_Y, ALIGNMENT_SPEED);}
		        	   if(OffsetValue < 0){ // If the goal is to the right, turn right.
		        		   proxy->set(JOY_LEFT_Y, ALIGNMENT_SPEED);}}*/
	        	   state = DRIVE;
	        	   break;
	        	   
	           case DRIVE: //drives the robot towards the goal, while still aligning
	        	   FrontDistance = proxy->get("FrontDistance");
	        	   while(proxy->get("VALID_IMAGE") == 1){
		        	   while (FrontDistance > DUMP_DISTANCE){
		        		   FrontDistance = proxy->get ("FrontDistance");
		        		   OffsetValue = proxy->get ("TARGETOFFSET");
		        		   AlignSpeed = OffsetValue * ALIGN_SPEED_CONST + AUTO_SPEED; //Speed the robot aligns at, proportional
		        		   
		        		   if(OffsetValue > DEAD_LEFT){ //If the robot is straying to the right, make right wheels faster.
		        			   proxy->set(JOY_LEFT_Y, AUTO_SPEED);
		        			   proxy->set(JOY_RIGHT_Y, AlignSpeed);}
		        		   if(OffsetValue < DEAD_RIGHT){ //If the robot is straying towards the left, make the left wheels faster.
		        			   proxy->set(JOY_LEFT_Y, AlignSpeed);
		        			   proxy->set(JOY_RIGHT_Y, AUTO_SPEED);}		   
		        		   if(OffsetValue < DEAD_LEFT && OffsetValue > DEAD_RIGHT){ //If the robot is going straight toward the goal, move straight.
			        		   proxy->set(JOY_LEFT_Y, AUTO_SPEED);
			        	   	   proxy->set(JOY_RIGHT_Y, AUTO_SPEED);}
		        		   }
		        	   proxy->set(JOY_LEFT_Y, 0); //Stops robot
		        	   proxy->set(JOY_RIGHT_Y, 0); //Stops robot   
		 	 	 	   state = DUMP;} 
	 	 	 	   break;
	 	 	 	   
	 	 	   case DUMP: //when at the proper distance, activate dump button
	 	 		   proxy->set(JOY_COPILOT_EJECT,1);
	 	 		   break;
	}
        	   

		Wait(AUTONOMOUS_WAIT_TIME);
	}
}
