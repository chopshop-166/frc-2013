/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: FrontSonar.h     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 30, 2013
*  File Description	: FrontSonar header
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
#define SONAR_CYCLE_TIME (50) // 10ms

class FrontSonar166 : public Team166Task
{
	#define SONAR_CYCLE_TIME (50) 
	#define AVERAGESIZE (2)   //The size of the rolling average for the sonar sensor.    
	#define SONARINPERVOLT (102.4)   //The magical conversion rate from volts to inches
public:
	
	// task constructor
	FrontSonar166(void);

	// task destructor
	virtual ~FrontSonar166(void);

	// Main function of the task
	virtual int Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10);
	
private:
	//Declare Proxy and Robot handles
	Proxy *proxy;				// Handle to proxy
	Robot *lHandle;            // Local handle
	
	AnalogChannel Sonar;
};
