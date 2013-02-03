/*******************************************************************************
*  Project   		: ChopShop13
*  File Name  		: Inclinometer.h     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2010
*  File Description	: Template header file for tasks, with template functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2010.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

#pragma once

#include "WPILib.h"
#include "Robot.h"
#include "Encoder.h"

// This constant defines how often we want this task to run in the form
// of miliseconds. Max allowed time is 999 miliseconds.

#define INCLINOMETER_CYCLE_TIME (10) // 10ms

class Inclinometer166 : public Team166Task
{
	
public:
	
	// task constructor
	Inclinometer166(void);

	// task destructor
	virtual ~Inclinometer166(void);

	// Main function of the task
	virtual int Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10);
	
private:
	//Declare Proxy and Robot handles
	Proxy *proxy;				// Handle to proxy
	Robot *lHandle;            // Local handle
	Encoder Inclinometer;
	
	
	// Any variables that the task has as members go here
	// <<CHANGEME>>
	int incangle;	//inclinometer angle
};
