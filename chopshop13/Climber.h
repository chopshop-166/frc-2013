/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Clutch.h     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 27, 2013
*  File Description	: Clutch header file for tasks, with Clutch functions
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
#define Clutch_CYCLE_TIME (50) // 10ms

// Rename this, too, or you'll run into collisions
// <<CHANGEME>>
class Climber166 : public Team166Task
{
	
public:
	
	// task constructor
	Climber166(void);

	// task destructor
	virtual ~Climber166(void);

	// Main function of the task
	virtual int Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10);
	
private:
	//Declare Proxy and Robot handles
	Proxy *proxy;				// Handle to proxy
	Robot *lHandle;            // Local handle
	float climber_btnl;	
	float climber_btnr;
	// Any variables that the task has as members go here
	DoubleSolenoid *climber;
};
