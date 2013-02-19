/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: TaskTemplate.h     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2013
*  File Description	: Template header file for tasks, with template functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2013.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

#pragma once

#include "WPILib.h"
#include "Robot.h"

//
// This constant defines how often we want this task to run in the form
// of miliseconds. Max allowed time is 999 miliseconds.
// You should rename this when you copy it into a new file
// <<CHANGEME>>
#define GYRO_CYCLE_TIME (25)

// Rename this, too, or you'll run into collisions
// <<CHANGEME>>
class Gyro166 : public Team166Task
{
	
public:
	
	// task constructor
	Gyro166(void);

	// task destructor
	virtual ~Gyro166(void);

	// Main function of the task
	virtual int Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10);
	
private:
	//Declare Proxy and Robot handles
	Proxy *proxy;				// Handle to proxy
	Robot *lHandle;            // Local handle
	Gyro AutoGyro;
	float GyroAngle;
};
