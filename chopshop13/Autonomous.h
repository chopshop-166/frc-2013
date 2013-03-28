/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Autonomous.h     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 26, 2013
*  File Description	: Header for robot code to execute in autonomous mode
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2013.  All Rights Reserved.          */
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

	int wait_count;
};
