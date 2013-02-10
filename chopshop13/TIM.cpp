/*******************************************************************************
*  Project   		: chopshop13
*  File Name  		: TIM.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2010
*  File Description	: Template source file for tasks, with template functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2010.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/
	
/*------------------------------------------------------------------------------*/
/* Find & Replace "Timmy" with the name you would like to give this task     */
/* Find & Replace "Testing" with the name you would like to give this task      */
/* Find & Replace "TIM" with the name you would like to give this task */
/*------------------------------------------------------------------------------*/

#include "WPILib.h"
#include "TIM.h"

// To locally enable debug printing: set true, to disable false
#define DPRINTF if(false)dprintf

// Sample in memory buffer
struct abuf
{
	struct timespec tp;               // Time of snapshot
	// Any values that need to be logged go here
	// <<CHANGEME>>
};

//  Memory Log
// <<CHANGEME>>
class TIMLog : public MemoryLog
{
public:
	TIMLog() : MemoryLog(
			sizeof(struct abuf), TIM_CYCLE_TIME, "Timmy",
			"Seconds,Nanoseconds,Elapsed Time\n" // Put the names of the values in here, comma-seperated
			) {
		return;
	};
	~TIMLog() {return;};
	unsigned int DumpBuffer(          // Dump the next buffer into the file
			char *nptr,               // Buffer that needs to be formatted
			FILE *outputFile);        // and then stored in this file
	// <<CHANGEME>>
	unsigned int PutOne(void);     // Log the values needed-add in arguments
};

// Write one buffer into memory
// <<CHANGEME>>
unsigned int TIMLog::PutOne(void)
{
	struct abuf *ob;               // Output buffer
	
	// Get output buffer
	if ((ob = (struct abuf *)GetNextBuffer(sizeof(struct abuf)))) {
		
		// Fill it in.
		clock_gettime(CLOCK_REALTIME, &ob->tp);
		// Add any values to be logged here
		// <<CHANGEME>>
		return (sizeof(struct abuf));
	}
	
	// Did not get a buffer. Return a zero length
	return (0);
}

// Format the next buffer for file output
unsigned int TIMLog::DumpBuffer(char *nptr, FILE *ofile)
{
	struct abuf *ab = (struct abuf *)nptr;
	
	// Output the data into the file
	fprintf(ofile, "%u,%u,%4.5f\n",
			ab->tp.tv_sec, ab->tp.tv_nsec,
			((ab->tp.tv_sec - starttime.tv_sec) + ((ab->tp.tv_nsec-starttime.tv_nsec)/1000000000.))
			// Add values here
			// <<CHANGEME>>
	);
	
	// Done
	return (sizeof(struct abuf));
}


// task constructor
TIM166::TIM166(void):LittleArm(TIM_ID),LimitA(INCLINER_LIMIT_A),LimitB(INCLINER_LIMIT_B)
{
	Start((char *)"166TIMTask", TIM_CYCLE_TIME);
	// ^^^ Rename those ^^^
	// <<CHANGEME>>
	// Register the proxy
	proxy = Proxy::getInstance();
	return;
};
	
// task destructor
TIM166::~TIM166(void)
{
	return;
};
	
// Main function of the task
int TIM166::Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10)
{
	TIMLog sl;                   // log
	
	// Let the world know we're in
	DPRINTF(LOG_DEBUG,"In the 166 Timmy task\n");
	
	// Wait for Robot go-ahead (e.g. entering Autonomous or Tele-operated mode)
	// lHandle = Robot::getInstance() MUST go after this, otherwise code breaks
	WaitForGoAhead();
	
	// Register our logger
	lHandle = Robot::getInstance();
	lHandle->RegisterLogger(&sl);
	
	proxy->add("RobotAngle");
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
		timmyinfo=proxy->get("Angle");
		
		//timmyspeed = proxy->get(joytim_y);
		//printf("Angle: %f\r",timmyinfo);
		
		//Adjust TIM's arm to meet the required angle
		/*
		if(timmyinfo<TIM_ANGLE)
			timmyspeed=TIMMY_SPEED;
		if(timmyinfo>TIM_ANGLE)
			timmyspeed=-TIMMY_SPEED;
		else
			timmyspeed=0;
		 
		//If timmy is smashing into the limit switch, bind his arm
		if(LimitA.Get()&&timmyspeed>0)
			timmyspeed=0;
		if(LimitB.Get()&&timmyspeed<0)
			timmyspeed=0;
		*/
		timmyspeed = proxy->get("joy3y");
		LittleArm.Set(timmyspeed);
		
        // Logging any values
		// <<CHANGEME>>
		// Make this match the declaraction above
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
