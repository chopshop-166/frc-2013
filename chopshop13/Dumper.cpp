/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Dumper.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2013
*  File Description	: Dumper source file for tasks, with template functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2013.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

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
Dumper166::Dumper166(void):
		DumperMotorA(MOTOR_DUMPER_A),
		//State				Switch	SW
		//Indent 			Open 	1
		//Not in indent		Closed	0
		IndentLimit(DUMPER_LIMIT),
		//State				Switch	SW
		//Storage			Closed	0
		//Not Storage		Open	1
		BottomLimit(DUMPER_BOTTOM_LIMIT),
		DumperPiston(DUMPER_PISTON_A, DUMPER_PISTON_B)
{
	Start((char *)"Dumper", DUMPER_CYCLE_TIME);
	RotateSpeed = 0;
	TargetPosition = kStorage;
	OldPosition = kStorage;
	Blips = 0;
	indent = 1;
	first_transition = 0;
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
	//Track the buttons for various positions
	proxy->TrackNewpress(JOY_COPILOT_STORE_TRACK);
	proxy->TrackNewpress(JOY_COPILOT_LOAD_TRACK);
	proxy->TrackNewpress(JOY_COPILOT_DUMP_TRACK);
	// Add value saying we're in position
	proxy->add(DUMPER_IN_POSITION);
	//Default this value to 0... don't remember if proxy does this
	proxy->set(DUMPER_IN_POSITION, 0);
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
		//Check if we're outside of an indent
		if ((indent == 1) && (IndentLimit.Get()==1)) {
			indent = 0;
		}
		
		//Check If we are moving to a new position;
		if (proxy->get(JOY_COPILOT_STORE)) {
			OldPosition = TargetPosition;
			TargetPosition = kStorage;
			DPRINTF(LOG_DEBUG,"Storing");
			//Going to loading position
		} else if (Blips != 0) {
			DPRINTF(LOG_DEBUG, "STILL MOVING!");
		} else if (proxy->get(JOY_COPILOT_LOAD)){
			OldPosition = TargetPosition;
			TargetPosition = kLoading;
			//Prevent us from going from dumping to loading
			if (OldPosition == kDumping) {
				TargetPosition = kLoading;
			}
			//Calculate number of indents we must pass through
			Blips = TargetPosition - OldPosition;
			DPRINTF(LOG_DEBUG,"Loading");
			//Going to dumping position
		} else if (proxy->get(JOY_COPILOT_DUMP)) {
			OldPosition = TargetPosition;
			TargetPosition = kDumping;
			//Calculate number of indents we must pass through
			Blips = TargetPosition - OldPosition;
			DPRINTF(LOG_DEBUG,"Dumping");
		}
		
		//If we weren't previously in an indent, and now we are set indent, transition
		if ((indent == 0) && (IndentLimit.Get()==0)) {
			indent = 1;
			first_transition = 1;
		}
		//Set us moving in the right direction
		//If we are going to storage and not there, go to it
		if((TargetPosition == kStorage) && (BottomLimit.Get() == 0)) {
			RotateSpeed = -ROTATE_SPEED;
		//Going to storage and there, stop moving
		} else if ((TargetPosition == kStorage) && (BottomLimit.Get() == 1)) {
			Blips = 0;
			RotateSpeed = 0;
		//Need to move forwards
		} else if (Blips > 0) {
			//We found an indent
			Blips -= first_transition;
			RotateSpeed = ROTATE_SPEED;
		//We are where we want to be. 
		} else {
			RotateSpeed = 0;
		}
		
		if ((TargetPosition != kStorage) && (Blips == 0)) {
			DPRINTF(LOG_DEBUG, "Not Storing but we're there!");
			proxy->set(DUMPER_IN_POSITION, 1);
		} else {
			DPRINTF(LOG_DEBUG, "Storing or we're there!!!!");
			proxy->set(DUMPER_IN_POSITION, 0);
		}
		
		DPRINTF(LOG_DEBUG,"Limit: %d Indent: %d State: %d Bottom limit: %d", IndentLimit.Get(), indent, TargetPosition, BottomLimit.Get());
		//Set Motors to move
		DumperMotorA.Set(RotateSpeed);
		
		if(proxy->get(JOY_COPILOT_EJECT)) {
			DumperPiston.Set(DoubleSolenoid::kForward);	
		} else {
			DumperPiston.Set(DoubleSolenoid::kReverse);
		}
		
		first_transition = 0;
		// Make this match the declaraction above
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
