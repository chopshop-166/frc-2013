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


//Define Shooter motor
#define SHOOTER_ID (9)

////////////////////
//JOYSTICK DEFINES//
////////////////////

//Define drive joysticks
#define JOY_LEFT_Y  ("joy1y")
#define JOY_RIGHT_Y ("joy2y")

//Defines for Climbing System
#define JOY_LEFT_ENGAGE ("joy3b3n") // X Button on XBOX
#define JOY_LEFT_TRACK ("joy3b3")

//Fire Frisbee regardless
#define JOY_COPILOT_FIRE	("joy3b6")

//Fire Frisbees automatically
#define JOY_COPILOT_FIRE_AUTO ("joy3z")

//Spinup the shooter
#define JOY_COPILOT_SPINUP 	("joy3b1n")
//Track Spinup button
#define JOY_COPILOT_SPINUP_TRACK ("joy3b1")
#define JOY_COPILOT_LOAD ("joy3b5")

//Set shooter speed
#define SHOOT_SPEED_UP ("joy1b1n")
#define SHOOT_SPEED_UP_TRACK ("joy1b1")
#define SHOOT_SPEED_DOWN ("joy1b2n")
#define SHOOT_SPEED_DOWN_TRACK ("joy1b2")


/////////////////
//ANALOG INPUTS//
/////////////////


//////////////////
//DIGITAL INPUTS//
//////////////////

#define SHOOTER_ENCODER_A (1)
#define SHOOTER_ENCODER_B (2)

//Define pressure switch
#define PRESSURE_SWITCH (14)

////////////////
//RELAY OUTPUT//
////////////////

//Define Compressor relay
#define COMPRESSOR_RELAY (1)

/////////////////////
//Pneumatics Output//
/////////////////////

//Define Clutch Piston
#define CLIMBER_OUT (1)
#define CLIMBER_IN (2)

#define SHOOTER_PISTON (3)

/////////////////////////
//TASK SPECIFIC DEFINES//
/////////////////////////

//Defines for shooter
#define PYRAMID_SHOOT_SPEED (2000)
#define IDLESPEED (1500)
