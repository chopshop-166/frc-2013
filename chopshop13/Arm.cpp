/*******************************************************************************
*  Project   		: ChopShop13
*  File Name  		: Arm.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: February 2, 2013
*  File Description	: Arm source file for tasks, with Arm functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2013.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Find & Replace "Arm" with the name you would like to give this task     */
/* Find & Replace "Arm" with the name you would like to give this task      */
/* Find & Replace "Arm" with the name you would like to give this task */
/*------------------------------------------------------------------------------*/

#include "WPILib.h"
#include "Arm.h"

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
class ArmLog : public MemoryLog
{
public:
	ArmLog() : MemoryLog(
			sizeof(struct abuf), Arm_CYCLE_TIME, "Arm",
			"Seconds,Nanoseconds,Elapsed Time\n" // Put the names of the values in here, comma-seperated
			) {
		return;
	};
	~ArmLog() {return;};
	unsigned int DumpBuffer(          // Dump the next buffer into the file
			char *nptr,               // Buffer that needs to be formatted
			FILE *outputFile);        // and then stored in this file
	// <<CHANGEME>>
	unsigned int PutOne(void);     // Log the values needed-add in arguments
};

// Write one buffer into memory
// <<CHANGEME>>
unsigned int ArmLog::PutOne(void)
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
unsigned int ArmLog::DumpBuffer(char *nptr, FILE *ofile)
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
Arm166::Arm166(void):left_enc(LEFT_ENCODER_A, LEFT_ENCODER_B, Encoder::k4X),right_enc(RIGHT_ENCODER_A, RIGHT_ENCODER_B, Encoder::k4X)
{
	Start((char *)"166ArmTask", Arm_CYCLE_TIME);
	// ^^^ Rename those ^^^
	// <<CHANGEME>>
	// Register the proxy
	proxy = Proxy::getInstance();
	return;
};
	
// task destructor
Arm166::~Arm166(void)
{
	return;
};
	
// Main function of the task
int Arm166::Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10)
{
	ArmLog sl;                   // log
	
	// Let the world know we're in
	DPRINTF(LOG_DEBUG,"In the 166 Arm task\n");
	
	// Wait for Robot go-ahead (e.g. entering Autonomous or Tele-operated mode)
	// lHandle = Robot::getInstance() MUST go after this, otherwise code breaks
	WaitForGoAhead();
	
	// Register our logger
	lHandle = Robot::getInstance();
	lHandle->RegisterLogger(&sl);
	left_enc.Start();
	right_enc.Start();
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
		clutch_btnl=proxy->get(JOY_LEFT_ENGAGE);
		clutch_btnr=proxy->get(JOY_RIGHT_ENGAGE);
		if(clutch_btnr||clutch_btnl == 1)
		{
			left_enc.Reset();
			right_enc.Reset();
		}
		left_rotations=(left_enc.Get())/1024;
		right_rotations=(right_enc.Get())/1024;
		left_distance=1.5*left_rotations;
		right_distance=1.5*right_rotations;
		// <<CHANGEME>>
		// Insert your own logic here
		
        // Logging any values
		// <<CHANGEME>>
		// Make this match the declaraction above
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
