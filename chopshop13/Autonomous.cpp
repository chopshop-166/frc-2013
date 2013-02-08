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
	state = INIT;
	// Register robot handle
	while( !(lHandle = Robot::getInstance()) && !( lHandle->IsAutonomous() ) ) {
		Wait(AUTONOMOUS_WAIT_TIME);
	}
	
	// Register proxy handle
	while( (proxy = Proxy::getInstance()) == NULL ) {
		Wait(AUTONOMOUS_WAIT_TIME);
	}
	
	while( lHandle->IsAutonomous() ) {
		
		OffsetValue = proxy->get ("TargetOffset");
		SonarDistance = proxy->get ("Sonar_Distance");
		int Valid_Image = int(proxy->get("Valid_Image"));
		printf("State: %d  Offset: %f Image: %d Distance: %f\r", state, OffsetValue, Valid_Image, SonarDistance);
 switch(state){
	           case INIT: //initializes autonomous
	        	   lHandle->DriverStationDisplay("We are Initializing");
	        	   if (SonarDistance != 0.0){
	        		   state = DRIVE1;}
	        	   break;
	        	   
	           case DRIVE1:
	        	   if(SonarDistance > MAX_SONAR_DIST){
	        		   proxy->set(JOY_LEFT_Y, -FORWARD_SPEED);
	        		   proxy->set(JOY_RIGHT_Y, -FORWARD_SPEED);}
	        	   else{
	        	   state = TURN;}
	        	   break;
	        	   
	           case TURN:
	        	   if(proxy->get("Valid_Image") == 0){
	        		   proxy->set(JOY_LEFT_Y, -TURNSPEED);
	        		   proxy->set(JOY_RIGHT_Y, TURNSPEED);}
	        	   else{
	        	   state = ALIGNING;}
	        	   break;
	        	   
	           case ALIGNING: //points the robot towards the goal using an offset provided by the camera
	        	   if( (fabs(OffsetValue) >= DEAD_ZONE) && (proxy->get("Valid_Image") == 1) ){
   		        	  AlignSpeedAlign = OffsetValue * ALIGN_SPEED_CONST;		   
   		        	  	  proxy->set(JOY_LEFT_Y, -AlignSpeedAlign);
   		        	  	  proxy->set(JOY_RIGHT_Y, AlignSpeedAlign);}
   		        	  	  //printf("AlignSpeed: %f", AlignSpeedAlign);
   	        	   else{
   	        		   if( (fabs(OffsetValue) < DEAD_ZONE) && (proxy->get("Valid_Image") == 1) ){ 
   	        		   	   state = DRIVE2;}
   	        		   if (proxy->get("Valid_Image") == 0){
   	        			   proxy->set(JOY_LEFT_Y, 0);
   	        			   proxy->set(JOY_RIGHT_Y, 0);}}
   	        		   //printf("DERP A DERP");
   	        	   
   	        	   break;
	        	   
	           case DRIVE2: //drives the robot towards the goal, while still aligning
	        	   //FrontDistance = proxy->get("FrontDistance");

		        	   if (SonarDistance > DUMP_DISTANCE){
		        			   AlignSpeed = OffsetValue * ALIGN_SPEED_CONST; //Speed the robot aligns at, proportional
		        			   proxy->set(JOY_LEFT_Y, -AlignSpeed - FORWARD_SPEED);
			        	   	   proxy->set(JOY_RIGHT_Y, AlignSpeed - FORWARD_SPEED);}
		        	   else{
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

