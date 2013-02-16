/*******************************************************************************
*  Project   		: chopshop13
*  File Name  		: TIM.h     
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

//
// This constant defines how often we want this task to run in the form
// of miliseconds. Max allowed time is 999 miliseconds.

#define TIM_CYCLE_TIME (50) // 10ms


class TIM166 : public Team166Task
{

public:
	
	// task constructor
	TIM166(void);

	// task destructor
	virtual ~TIM166(void);

	// Main function of the task
	virtual int Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10);
	
private:
	//Declare Proxy and Robot handles
	Proxy *proxy;				// Handle to proxy
	Robot *lHandle;            // Local handle-
	Victor LittleArm;		//TIM's little arm
	DigitalInput LimitA;
	DigitalInput LimitB;
	
	// Any variables that the task has as members go here
	// <<CHANGEME>>
	float timmyspeed;   //speed of little arm
	float timmyinfo;	//proxy->get the angle	
	
};
