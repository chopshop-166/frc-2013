/*******************************************************************************
*  Project   		: Framework
*  File Name  		: FrontSonar.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2010
*  File Description	: Template source file for tasks, with template functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2010.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Find & Replace "Template" with the name you would like to give this task     */
/* Find & Replace "Testing" with the name you would like to give this task      */
/* Find & Replace "FrontSonar" with the name you would like to give this task */
/*------------------------------------------------------------------------------*/

#include "WPILib.h"
#include "FrontSonar.h"

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
class FrontSonarLog : public MemoryLog
{
public:
	FrontSonarLog() : MemoryLog(
			sizeof(struct abuf), SONAR_CYCLE_TIME, "FrontSonar",
			"Seconds,Nanoseconds,Elapsed Time\n" // Put the names of the values in here, comma-seperated
			) {
		return;
	};
	~FrontSonarLog() {return;};
	unsigned int DumpBuffer(          // Dump the next buffer into the file
			char *nptr,               // Buffer that needs to be formatted
			FILE *outputFile);        // and then stored in this file
	// <<CHANGEME>>
	unsigned int PutOne(void);     // Log the values needed-add in arguments
};

// Write one buffer into memory
// <<CHANGEME>>
unsigned int FrontSonarLog::PutOne(void)
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
unsigned int FrontSonarLog::DumpBuffer(char *nptr, FILE *ofile)
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
FrontSonar166::FrontSonar166(void):Sonar(SONAR_INPUT)
{
	Start((char *)"166FrontSonarTask", SONAR_CYCLE_TIME);
	// ^^^ Rename those ^^^
	// <<CHANGEME>>
	// Register the proxy
	proxy = Proxy::getInstance();
	return;
};
	
// task destructor
FrontSonar166::~FrontSonar166(void)
{
	return;
};
	
// Main function of the task
int FrontSonar166::Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10)
{
	FrontSonarLog sl;                   // log
	
	// Let the world know we're in
	DPRINTF(LOG_DEBUG,"In the 166 FrontSonar task\n");
	
	// Wait for Robot go-ahead (e.g. entering Autonomous or Tele-operated mode)
	// lHandle = Robot::getInstance() MUST go after this, otherwise code breaks
	WaitForGoAhead();
	
	// Register our logger
	lHandle = Robot::getInstance();
	lHandle->RegisterLogger(&sl);
	
	proxy->add("SONAR_DISTANCE");
	float sonar_array[AVERAGESIZE];
	for (int i=0; i<AVERAGESIZE;i++) {
		sonar_array[i]= 0;
	}
	int i = 0;
	
	float sonar_volts=0;
	float sonar_distance = 0;
	
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
		sonar_volts = Sonar.GetVoltage();
		sonar_array[(i%AVERAGESIZE)] = sonar_volts;
		sonar_distance = 0;
		
		for(unsigned j = 0;j<AVERAGESIZE;j++) {
                sonar_distance += sonar_array[j];
        }
		sonar_distance/=AVERAGESIZE;
		sonar_distance*=SONARINPERVOLT;
		//sonar_distance = SONARINPERVOLT * sonar_volts;
		
		proxy->set("SONAR_DISTANCE", sonar_distance);
		i++;
		printf("Distance: %f  Raw: %f\r", sonar_distance, sonar_volts);
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
