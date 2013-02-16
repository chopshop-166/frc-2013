/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Dumper.h     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2013
*  File Description	: Dumper header file for tasks, with template functions
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

#define DUMPER_CYCLE_TIME (50) // 50ms
#define ROTATE_SPEED (.2)

class Dumper166 : public Team166Task
{
	
public:
	
	// task constructor
	Dumper166(void);

	// task destructor
	virtual ~Dumper166(void);

	// Main function of the task
	virtual int Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10);
	
private:
	//Declare Proxy and Robot handles
	Proxy *proxy;				// Handle to proxy
	Robot *lHandle;            // Local handle

	
	Victor DumperMotorA;
	DigitalInput DumpLimit;
	DigitalInput BottomLimit;
	DoubleSolenoid DumperPiston;
	
	float RotateSpeed;
	
	//Location of dumper
	enum location {
		kStorage,
		kLoading,
		kDumping,
		kPyramid
	};
	location TargetPosition;
	location OldPosition;
	int Blips;
	int indent;
	int first_transition;
};
