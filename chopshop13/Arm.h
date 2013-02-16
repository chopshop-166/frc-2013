/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Arm.h     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: February 2, 2013
*  File Description	: Arm header file for tasks, with Arm functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2013.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

#pragma once

#include "WPILib.h"
#include "Robot.h"
#include "Encoder.h"

//
// This constant defines how often we want this task to run in the form
// of miliseconds. Max allowed time is 999 miliseconds.
// You should rename this when you copy it into a new file
// <<CHANGEME>>
#define Arm_CYCLE_TIME (10) // 10ms

// Rename this, too, or you'll run into collisions
// <<CHANGEME>>
class Arm166 : public Team166Task
{
	
public:
	
	// task constructor
	Arm166(void);

	// task destructor
	virtual ~Arm166(void);

	// Main function of the task
	virtual int Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10);
	
private:
	//Declare Proxy and Robot handles
	Proxy *proxy;				// Handle to proxy
	Robot *lHandle;            // Local handle
	Encoder left_enc;
	Encoder right_enc;
	float clutch_btnl;	
	float clutch_btnr;
	float right_rotations;
	float left_rotations;
	float right_distance;
	float left_distance;
	// Any variables that the task has as members go here
	// <<CHANGEME>>
};
