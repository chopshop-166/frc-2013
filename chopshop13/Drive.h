/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Drive.h     
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
#define DRIVE_CYCLE_TIME (10) // 10ms

// Rename this, too, or you'll run into collisions
// <<CHANGEME>>
class Drive : public Team166Task
{
	
public:
	
	// task constructor
	Drive(void);

	// task destructor
	virtual ~Drive(void);

	// Main function of the task
	virtual int Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10);
	
private:
	//Declare Proxy and Robot handles
	Proxy *proxy;				// Handle to proxy
	Robot *lHandle;            // Local handle
	
	//Define Victors
	Victor motorL1;
	Victor motorL2;
	Victor motorL3;
	Victor motorR1;
	Victor motorR2;
	Victor motorR3;
	
	bool AutoDump1;
	bool AutoDump2;
	
	//The "D" at the end incicates these variables are in Drive not Autonomous
	float OffsetValueD; // The offset of the goal in degrees, positive or negative, provided by camera
	float FrontDistanceD;  // The distance the robot is from the wall, provided by distance sensor
	float AlignSpeedD; // The speed that one side is set to when it needs to speed up, proportional
	float OffsetValueAlignD;//offset of goal in alignment stage
	float SonarDistanceD; // distnace from the sonar sensor
	float DRIVE2_GAIN_D; //Gain for slowing down as we get closer to the target in DRIVE2
	int Valid_Image_D;

	// Any variables that the task has as members go here
	// <<CHANGEME>>
};
