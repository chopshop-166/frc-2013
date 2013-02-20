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
	state = INIT; //Set the state to INIT to start
	proxy->add("AUTO_STATE");
	// Register robot handle
	while( !(lHandle = Robot::getInstance()) && !( lHandle->IsAutonomous() ) ) {
		Wait(AUTONOMOUS_WAIT_TIME);
	}
	
	// Register proxy handle
	
	while( (proxy = Proxy::getInstance()) == NULL ) {
		Wait(AUTONOMOUS_WAIT_TIME);
	}
	
	while( lHandle->IsAutonomous() ) {
		GyroAngle = proxy->get("GYROANGLE");
		OffsetValue = proxy->get ("TargetOffset"); //Get the offset of the target from the center of the target(camera)
		SonarDistance = proxy->get ("Sonar_Distance");//Get output from sonar sensor
		int Valid_Image = int(proxy->get("Valid_Image"));//Get whether the camera has a valid target
		
		//USE THIS PRINTF TO DEBUG ALL OF AUTO.
		printf("State: %d  Offset: %f Image: %d Distance: %f Gain: %f\r", state, OffsetValue, Valid_Image, SonarDistance,DRIVE2_GAIN);
		
		switch(state){
	           case INIT: 
	        	   wait_count = 0;
	        	   lHandle->DriverStationDisplay("We are Initializing");
	        	   if (SonarDistance != 0.0){//Make sure we have a output from sonar sensor
	        		   state = DRIVE1;}//goto next state
	        	   if ((Valid_Image == 1) && (OffsetValue > 0))
	        	   {
	        		   MAX_SONAR_DIST = MAX_SONAR_DIST_RIGHT;
	        		   //right side
	        	   }
	        	   else
	        	   {
	        		   MAX_SONAR_DIST = MAX_SONAR_DIST_LEFT;
	        		   //left side
	        	   }
	        			   
	        	   break;
	        	   
	           case DRIVE1:
	        	   GyroOffset = min(fabs((GyroAngle) / 20.0),.15) * (-GyroAngle/fabs(GyroAngle));
	        	   if(SonarDistance > MAX_SONAR_DIST){//If we are farther away than desired, go forward
	        		   proxy->set(JOY_LEFT_Y, -FORWARD_SPEED - GyroOffset);
	        		   proxy->set(JOY_RIGHT_Y, -FORWARD_SPEED + GyroOffset);
	        	   }
	        	   else{
	        	   state = TURN;
	        	   } //goto next state
	        	   break;
	        	   
	           case TURN:
	        	   if(proxy->get("Valid_Image") == 0){//If the camera doesn't see a target, turn untill you see one
	        		   proxy->set(JOY_LEFT_Y, -TURNSPEED);
	        		   proxy->set(JOY_RIGHT_Y, TURNSPEED);
	        	   }
	        	   else{
	        	   state = ALIGNING;
	        	   }//goto next state
	        	   break;
	        	   
	           case ALIGNING: //points the robot towards the goal using an offset provided by the camera
	        	   if( (fabs(OffsetValue) >= DEAD_ZONE) && (proxy->get("Valid_Image") == 1) ){
   		        	  AlignSpeedAlign = max(fabs(OffsetValue * ALIGN_SPEED_CONST), MIN_SPEED_ALIGN) * (OffsetValue/fabs(OffsetValue));		   
   		        	  	  proxy->set(JOY_LEFT_Y, -AlignSpeedAlign);
   		        	  	  proxy->set(JOY_RIGHT_Y, AlignSpeedAlign);
	        	   }
   	        	   else{
   	        		   if( (fabs(OffsetValue) < DEAD_ZONE) && (proxy->get("Valid_Image") == 1) ){//If we are within a certain amount of the center of the target, and we have a target, goto next state
   	        		   	   state = DRIVE2;
   	        		   	   proxy->set(JOY_LEFT_Y, 0);
   	        		   	   proxy->set(JOY_RIGHT_Y, 0);
   	        		   }
   	        		   if (proxy->get("Valid_Image") == 0){//We we don't have an image at this point, stop the robot(somthing went wrong)
   	        			   proxy->set(JOY_LEFT_Y, 0);
   	        			   proxy->set(JOY_RIGHT_Y, 0);}}
   	        	   
   	        	   break;
	        	   
	           case DRIVE2: //drives the robot towards the goal, while still aligning
	        	   	   DRIVE2_GAIN = min((SonarDistance - DUMP_DISTANCE)/ 48.0,1.0);
		        	   if (SonarDistance > DUMP_DISTANCE)
		        	   {
		        		   	   if (Valid_Image == 0)
		        		   	   {
		        		   		  AlignSpeed = 0;
		        		   		  proxy->set(JOY_LEFT_Y, -AlignSpeed - max(FORWARD_SPEED * DRIVE2_GAIN,MIN_SPEED));
		        		   		  proxy->set(JOY_RIGHT_Y, AlignSpeed - max(FORWARD_SPEED * DRIVE2_GAIN,MIN_SPEED));
		        		   	   }
		        		   	   else
		        		   	   {
		        		   		   AlignSpeed = OffsetValue * ALIGN_SPEED_CONST; //Speed the robot aligns at, proportional
		        		   		   proxy->set(JOY_LEFT_Y, -AlignSpeed - max(FORWARD_SPEED * DRIVE2_GAIN,MIN_SPEED));
		        		   		   proxy->set(JOY_RIGHT_Y, AlignSpeed - max(FORWARD_SPEED * DRIVE2_GAIN,MIN_SPEED));
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
	 	 		   proxy->set(JOY_COPILOT_DUMP_TRACK,1);
	 	 		 if (proxy->get(DUMPER_IN_POSITION))
				   {
					   wait_count = wait_count + 1;
					   proxy->set(JOY_COPILOT_EJECT,1);
					   if (wait_count == 10)
					   {
					   wait_count = 0;
					   state = TURN2;
					   }
				   }
	 	 		   
	 	 		   break;
	 	 		   
	 	 	 case TURN2:
	 	 		wait_count = wait_count + 1;
	 	 		if (wait_count < 10)
	 	 		{
	 	 			proxy->set(JOY_LEFT_Y, FORWARD_SPEED);
	 	 			proxy->set(JOY_RIGHT_Y, FORWARD_SPEED);  
	 	 		}
			   if (180.0 - GyroAngle > 0 )
			   {
				   GyroOffset = min(fabs((180.0 - GyroAngle) / 60.0),.27) * ((180.0 - GyroAngle)/fabs(180.0 - GyroAngle));
				   if(GyroAngle > 180.0)
				   {
					   proxy->set(JOY_LEFT_Y,-GyroOffset);
					   proxy->set(JOY_RIGHT_Y,GyroOffset);
				   } 
			   }
			   else
			   {
				   wait_count = 0;
				   state = RUN;
			   }
			   
			   
			   break;
			   
		   case RUN:
			   wait_count = wait_count + 1;
			   GyroOffset = min(fabs((180.0 - GyroAngle) / 20.0),.15) * ((180.0 - GyroAngle)/fabs(180.0 - GyroAngle));
			   if(SonarDistance > MAX_SONAR_DIST)
			   {
				   proxy->set(JOY_LEFT_Y, -FORWARD_SPEED - GyroOffset);
				   proxy->set(JOY_RIGHT_Y, -FORWARD_SPEED + GyroOffset);
			   }
			   if (wait_count > 60)
			   {
				 proxy->set(JOY_LEFT_Y,0);
				 proxy->set(JOY_RIGHT_Y,0);
			   }
			   break;
	}
        	   

		Wait(AUTONOMOUS_WAIT_TIME);
		proxy->set("AUTO_STATE",state);
	}
	state = INIT;
	
}

