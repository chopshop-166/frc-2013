/*******************************************************************************
*  Project   		: ChopShop 13
*  File Name  		: Autonomous.h     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 26, 2013
*  File Description	: Header for robot code to execute in autonomous mode
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2010.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/
#pragma once

#include "Robot.h"
#include "MemoryLog.h"
#include "WPILib.h"
#include "BaeUtilities.h"


#define AUTONOMOUS_WAIT_TIME (0.050)

class AutonomousTask
{
public:
	AutonomousTask(void);
	enum {INIT,ALIGNING,DRIVE,DUMP} state;
	float OffsetValue; // The offset of the goal in degrees, positive or negative, provided by camera
	float FrontDistance;  // The distance the robot is from the wall, provided by distance sensor
	float AlignSpeed; // The speed that one side is set to when it needs to speed up, proportional
	float OffsetValueAlign;//offset of goal in alignment stage
	float AlignSpeedAlign;//speed one side is set to in alignment stage
};