/*******************************************************************************
*  Project   		: Framework
*  File Name  		: Defines.h    
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2012
*  File Description	: Robot code header which contains defines for global usage
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2010.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

#pragma once

//define left motors
#define MOTOR_L1_ID (1)
#define MOTOR_L2_ID (2)
#define MOTOR_L3_ID (3)

//define right motors
#define MOTOR_R1_ID (4)
#define MOTOR_R2_ID (5)
#define MOTOR_R3_ID (6)

//Define Dumper motors
#define MOTOR_DUMPER_A (7)
#define MOTOR_DUMPER_B (8)

//Define drive joysticks
#define JOY_LEFT_Y  ("joy1y")
#define JOY_RIGHT_Y ("joy2y")

//Engage Clutch
#define JOY_LEFT_ENGAGE ("joy1b3")
#define JOY_RIGHT_ENGAGE ("joy2b3")

//Incline robot
#define JOY_LEFT_INCLINE ("joy1b4")
#define JOY_RIGHT_INCLINE ("joy2b4")

//Control Rotation of dumper system
#define JOY_COPILOT_DUMP ("joy3y")

//Frisbee eject trigger
#define JOY_COPILOT_EJECT ("joy3b1")

//Defines The Speeds Used During Autonomous
#define ALIGN_SPEED_CONST (.2)
#define AUTO_SPEED (.4)
#define ALIGNMENT_SPEED (.6)
#define DEAD_LEFT (1)
#define DEAD_RIGHT (-1)

//Sensor Values For Autonomous
#define DUMP_DISTANCE (6)//This is the distance to the wall that we need to be to dump into the goal

// Place any #defines for things you want to enable in multiple tasks, simply, here

