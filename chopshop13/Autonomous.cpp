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
		//printf("State: %d  Offset: %f", state, OffsetValue);
 switch(state){
	           case INIT: //initializes autonomous
	        	   lHandle->DriverStationDisplay("We are Initializing");
	        	   if (proxy->get("VALID_IMAGE") == 1)
	        	   {
	        	   state = ALIGNING;
	        	   }
	        	   break;
	        	   
	           case ALIGNING: //points the robot towards the goal using an offset provided by the camera
   	        	   
	        	   if( (fabs(OffsetValue) >= DEAD_ZONE) && (proxy->get("Valid_Image") == 1) )
   	        	   {
   		        	  AlignSpeedAlign = OffsetValue * ALIGN_SPEED_CONST;		   
   		        	  	  proxy->set(JOY_LEFT_Y, -AlignSpeedAlign);
   		        	  	  proxy->set(JOY_RIGHT_Y, AlignSpeedAlign);
   		        	  	  //printf("AlignSpeed: %f", AlignSpeedAlign);
   	        	   }
   	        	   else
   	        	   {
   	        		   if( (fabs(OffsetValue) < DEAD_ZONE) && (proxy->get("Valid_Image") == 1) )
   	        		   { 
   	        		   	   state = DRIVE;
   	        		   }
   	        		   if (proxy->get("Valid_Image") == 0)
   	        		   {
   	        			   proxy->set(JOY_LEFT_Y, 0);
   	        			   proxy->set(JOY_RIGHT_Y, 0); 
   	        		   }
   	        		   //printf("DERP A DERP");
   	        	   }
   	        	   break;
	        	   
	          /* case ALIGNING: //points the robot towards the goal using an offset provided by the camera
	        	   while(OffsetValue > DEAD_LEFT && OffsetValue < DEAD_RIGHT  && proxy->get("Valid_Image") == 1){
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
		        	  	  proxy->set(JOY_RIGHT_Y, AUTO_SPEED);}}
	        	   state = DRIVE;
	        	   break;*/
	        	   
	           case DRIVE: //drives the robot towards the goal, while still aligning
	        	   //FrontDistance = proxy->get("FrontDistance");
	        	   FrontDistance = 9001;

		        	   if (FrontDistance > DUMP_DISTANCE)
		        	   {
		        		   
		        		   if(proxy->get("VALID_IMAGE") == 1)
		        		   {
		        			   AlignSpeed = OffsetValue * ALIGN_SPEED_CONST; //Speed the robot aligns at, proportional
		        			   proxy->set(JOY_LEFT_Y, -AlignSpeed - FORWARD_SPEED);
			        	   	   proxy->set(JOY_RIGHT_Y, AlignSpeed - FORWARD_SPEED);
			        	   	   }
		        		   else
		        		   		{
		        			   proxy->set(JOY_LEFT_Y, 0);
		        			   proxy->set(JOY_RIGHT_Y, 0);
		        			   }
		        	   }
		        	   else
		        	   {
		        		   	proxy->set(JOY_LEFT_Y, 0); //Stops robot
		        		   	proxy->set(JOY_RIGHT_Y, 0); //Stops robot   
		        		   	state = DUMP;
		        	   }
	 	 	 	   break;
	 	 	 	   
	 	 	   case DUMP: //when at the proper distance, activate dump button
	 	 		   proxy->set(JOY_COPILOT_EJECT,1);
	 	 		   break;
	}
        	   

		Wait(AUTONOMOUS_WAIT_TIME);
	}
}

