/*******************************************************************************
*  Project   		: Framework
*  File Name  		: TaskDumper.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2010
*  File Description	: Dumper source file for tasks, with template functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2010.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Find & Replace "Dumper" with the name you would like to give this task     */
/* Find & Replace "Dumper" with the name you would like to give this task      */
/* Find & Replace "TaskDumper" with the name you would like to give this task */
/*------------------------------------------------------------------------------*/

#include "WPILib.h"
#include "Dumper.h"

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
class DumperLog : public MemoryLog
{
public:
	DumperLog() : MemoryLog(
			sizeof(struct abuf), DUMPER_CYCLE_TIME, "Dumper",
			"Seconds,Nanoseconds,Elapsed Time\n" // Put the names of the values in here, comma-seperated
			) {
		return;
	};
	~DumperLog() {return;};
	unsigned int DumpBuffer(          // Dump the next buffer into the file
			char *nptr,               // Buffer that needs to be formatted
			FILE *outputFile);        // and then stored in this file
	// <<CHANGEME>>
	unsigned int PutOne(void);     // Log the values needed-add in arguments
};

// Write one buffer into memory
// <<CHANGEME>>
unsigned int DumperLog::PutOne(void)
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
unsigned int DumperLog::DumpBuffer(char *nptr, FILE *ofile)
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
Dumper166::Dumper166(void):DumperMotorA(MOTOR_DUMPER_A),
		DumpLimit(DUMPER_LIMIT),
		DumperPiston(DUMPER_PISTON)
{
	Start((char *)"Dumper", DUMPER_CYCLE_TIME);
	RotateSpeed = 0;
	CurPosition = Storage;
	OldPosition = Storage;
	Blips = 0;
	Clicked = 0;
	// Register the proxy
	proxy = Proxy::getInstance();
	return;
};
	
// task destructor
Dumper166::~Dumper166(void)
{
	return;
};
	
// Main function of the task
int Dumper166::Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10)
{
	DumperLog sl;                   // log
	
	// Let the world know we're in
	DPRINTF(LOG_DEBUG,"In the Dumper task\n");
	
	// Wait for Robot go-ahead (e.g. entering Autonomous or Tele-operated mode)
	// lHandle = Robot::getInstance() MUST go after this, otherwise code breaks
	WaitForGoAhead();
	
	// Register our logger
	lHandle = Robot::getInstance();
	lHandle->RegisterLogger(&sl);
		
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
		//Reset Clicked if necessary
		if (DumpLimit.Get()==0) {
			Clicked = 0;
		}
		//Check If we are moving to a new position;
		if (proxy->get(JOY_COPILOT_STORE)) {
			OldPosition = CurPosition;
			CurPosition = Storage;
		} else if (proxy->get(JOY_COPILOT_LOAD)){
			OldPosition = CurPosition;
			CurPosition = Loading;
		} else if (proxy->get(JOY_COPILOT_DUMP)) {
			OldPosition = CurPosition;
			CurPosition = Dumping;
		}
		// Figure out where we are moving
		Blips = CurPosition - OldPosition;
		if ((Clicked == 0) && (DumpLimit.Get())) {
			Clicked = 1;
		}
		//Set us moving in the right direction
		if (Blips < 0) {
			Blips += Clicked;
			RotateSpeed =-.2;
		} else if(Blips > 0) {
			Blips -= Clicked;
			RotateSpeed = .2;
		} else {
			RotateSpeed = 0;
		}
		
		//Set Motors to move
		DumperMotorA.Set(RotateSpeed);
		DumperPiston.Set(proxy->get(JOY_COPILOT_EJECT));
		// Make this match the declaraction above
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
