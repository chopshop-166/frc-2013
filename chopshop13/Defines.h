/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Defines.h    
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2012
*  File Description	: Robot code header which contains defines for global usage
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2010.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

#pragma once

/////////////////
//Motor Defines//
/////////////////

//define left motors
#define MOTOR_L1_ID (1)
#define MOTOR_L2_ID (2)
#define MOTOR_L3_ID (3)

//define right motors
#define MOTOR_R1_ID (4)
#define MOTOR_R2_ID (5)
#define MOTOR_R3_ID (6)

//Define Dumper motors
#define MOTOR_DUMPER_A (8)

//Define Inclination managment motor
#define TIM_ID (9)

////////////////////
//JOYSTICK DEFINES//
////////////////////

//Define drive joysticks
#define JOY_LEFT_Y  ("joy1y")
#define JOY_RIGHT_Y ("joy2y")

//Define for Xbox
//#define JOY_RIGHT_Y ("joy1A5")

//Engage Clutch
#define JOY_LEFT_ENGAGE ("joy1b3n")
#define JOY_RIGHT_ENGAGE ("joy2b3n")
#define JOY_LEFT_TRACK ("joy1b3")
#define JOY_RIGHT_TRACK ("joy2b3")

//Incline robot
#define JOY_LEFT_INCLINE ("joy1b4")
#define JOY_RIGHT_INCLINE ("joy2b4")

//Control Position of Dumper
#define JOY_COPILOT_ROTATE ("joy3y")

#define JOY_COPILOT_STORE	("joy3b4n")
#define JOY_COPILOT_STORE_TRACK	("joy3b4")
#define JOY_COPILOT_LOAD	("joy3b2n")
#define JOY_COPILOT_LOAD_TRACK	("joy3b2")
#define JOY_COPILOT_DUMP	("joy3b1n")
#define JOY_COPILOT_DUMP_TRACK	("joy3b1")
#define JOY_COPILOT_DUMP_PYRAMID ("joy3b3")
//Frisbee eject trigger
#define JOY_COPILOT_EJECT ("joy3b6")

//TIM's joystick control
#define JOYTIM_Y ("joy4z")


/////////////////
//ANALOG INPUTS//
/////////////////

//Define Sonar Sensor
#define SONAR_INPUT_LEFT (1)
#define SONAR_INPUT_RIGHT (2)

//Define 


//////////////////
//DIGITAL INPUTS//
//////////////////



//Define Inclinometer Inputs
#define INCLINOMETER_A (1)
#define INCLINOMETER_B (2)

// Define Dumper Inputs
#define DUMPER_LIMIT (3)
#define DUMPER_BOTTOM_LIMIT (4)

//Define Encoder Inputs
#define RIGHT_ENCODER_A (5)
#define RIGHT_ENCODER_B (6)
#define LEFT_ENCODER_A (7)
#define LEFT_ENCODER_B (8)

//Define pressure switch
#define PRESSURE_SWITCH (14)

//Define Incliner limit switches
#define INCLINER_LIMIT_A	(12)
#define INCLINER_LIMIT_B	(13)


////////////////
//RELAY OUTPUT//
////////////////

//Define Compressor relay
#define COMPRESSOR_RELAY (1)

/////////////////////
//Pneumatics Output//
/////////////////////

//Define Clutch Piston
#define CLUTCH_PISTON (1)
//Define Dumper ejection piston
#define DUMPER_PISTON_A (2)
#define DUMPER_PISTON_B (3)

/////////////////////////
//TASK SPECIFIC DEFINES//
/////////////////////////

//Defines for TIM task
#define TARGET_ANGLE (60)
#define TIMMY_SPEED (.3)

//Defines Used During Autonomous
#define ALIGN_SPEED_CONST (.5)//was .5 
#define FORWARD_SPEED (.6)//was .4
#define ALIGNMENT_SPEED (.4)//was .5
#define DEAD_ZONE (.2)
#define DUMP_DISTANCE (22)
#define TURNSPEED (.3) // was .3
#define MIN_SPEED (.21)
#define MIN_SPEED_ALIGN (.3)
#define MAX_SONAR_DIST_LEFT (100)
#define MAX_SONAR_DIST_RIGHT (72)

//Defines for dumper
#define DUMPER_IN_POSITION ("InPosition")

//Define for Robots current angle
#define ROBOT_ANGLE ("Angle")


