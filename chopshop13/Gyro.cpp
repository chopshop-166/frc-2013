/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: GyroTask.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2013
*  File Description	: Gyro source file for tasks, with template functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2013.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Find & Replace "Gyro" with the name you would like to give this task     */
/* Find & Replace "Gyro" with the name you would like to give this task      */
/* Find & Replace "GyroTask" with the name you would like to give this task */
/*------------------------------------------------------------------------------*/

#include "WPILib.h"
#include "Gyro.h"

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
class GyroLog : public MemoryLog
{
public:
	GyroLog() : MemoryLog(
			sizeof(struct abuf), GYRO_CYCLE_TIME, "Gyro",
			"Seconds,Nanoseconds,Elapsed Time\n" // Put the names of the values in here, comma-seperated
			) {
		return;
	};
	~GyroLog() {return;};
	unsigned int DumpBuffer(          // Dump the next buffer into the file
			char *nptr,               // Buffer that needs to be formatted
			FILE *outputFile);        // and then stored in this file
	// <<CHANGEME>>
	unsigned int PutOne(void);     // Log the values needed-add in arguments
};

// Write one buffer into memory
// <<CHANGEME>>
unsigned int GyroLog::PutOne(void)
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
unsigned int GyroLog::DumpBuffer(char *nptr, FILE *ofile)
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
Gyro166::Gyro166(void):AutoGyro(GYRO_INPUT)
{
	Start((char *)"166GyroTask", GYRO_CYCLE_TIME);
	// ^^^ Rename those ^^^
	// <<CHANGEME>>
	// Register the proxy
	proxy = Proxy::getInstance();
	return;
};
	
// task destructor
Gyro166::~Gyro166(void)
{
	return;
};
	
// Main function of the task
int Gyro166::Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10)
{
	GyroLog sl;                   // log
	
	// Let the world know we're in
	DPRINTF(LOG_DEBUG,"In the 166 Gyro task\n");
	
	// Wait for Robot go-ahead (e.g. entering Autonomous or Tele-operated mode)
	// lHandle = Robot::getInstance() MUST go after this, otherwise code breaks
	WaitForGoAhead();
	
	// Register our logger
	lHandle = Robot::getInstance();
	lHandle->RegisterLogger(&sl);
	proxy->add("GYROANGLE");
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
	
		if (proxy->get("AUTO_STATE") == 0)
		{
			AutoGyro.Reset();
		}
	
		proxy->set("GYROANGLE",AutoGyro.GetAngle());
		//printf("Gryo: %f \r", AutoGyro.GetAngle());
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
