/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Shooter.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2013
*  File Description	: Template source file for tasks, with template functions
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2013.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Find & Replace "Template" with the name you would like to give this task     */
/* Find & Replace "Testing" with the name you would like to give this task      */
/* Find & Replace "Shooter" with the name you would like to give this task */
/*------------------------------------------------------------------------------*/

#include "WPILib.h"
#include "Shooter.h"

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
class ShooterLog : public MemoryLog
{
public:
	ShooterLog() : MemoryLog(
			sizeof(struct abuf), Shooter_CYCLE_TIME, "Shooter",
			"Seconds,Nanoseconds,Elapsed Time\n" // Put the names of the values in here, comma-seperated
			) {
		return;
	};
	~ShooterLog() {return;};
	unsigned int DumpBuffer(          // Dump the next buffer into the file
			char *nptr,               // Buffer that needs to be formatted
			FILE *outputFile);        // and then stored in this file
	// <<CHANGEME>>
	unsigned int PutOne(void);     // Log the values needed-add in arguments
};

// Write one buffer into memory
// <<CHANGEME>>
unsigned int ShooterLog::PutOne(void)
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
unsigned int ShooterLog::DumpBuffer(char *nptr, FILE *ofile)
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
Shooter166::Shooter166(void):
	ShooterVic (SHOOTER_ID),
	ShooterEncoder (SHOOTER_ENCODER_A, SHOOTER_ENCODER_B, true, Encoder::k1X),
	ShooterPID (P, I, D, F, &ShooterEncoder, &ShooterVic),
	ShooterPiston(SHOOTER_PISTON)
	

{
	Start((char *)"166ShooterTask", Shooter_CYCLE_TIME);
	// ^^^ Rename those ^^^
	ShooterPID.SetOutputRange(-1, 0);
	ShooterPID.SetInputRange(-13000, 0);
	// Register the proxy
	proxy = Proxy::getInstance();
	return;
};
	
// task destructor
Shooter166::~Shooter166(void)
{
	return;
};
	
// Main function of the task
int Shooter166::Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10)
{
	ShooterLog sl;                   // log
	
	// Let the world know we're in
	DPRINTF(LOG_DEBUG,"In the 166 Shooter task\n");
	
	// Wait for Robot go-ahead (e.g. entering Autonomous or Tele-operated mode)
	// lHandle = Robot::getInstance() MUST go after this, otherwise code breaks
	WaitForGoAhead();
	
	// Register our logger
	lHandle = Robot::getInstance();
	lHandle->RegisterLogger(&sl);
	ShooterEncoder.SetPIDSourceParameter(Encoder::kRate);
	ShooterPID.Enable();
	ShooterEncoder.Start();
	proxy->TrackNewpress(SHOOT_SPEED_UP_TRACK);
	proxy->TrackNewpress(SHOOT_SPEED_DOWN_TRACK);
	
	ShooterSpeed = 11000;
	P = 0.0009;
	I = 0.00004;
	D = 0;
	F = 0.00005;
	
	/* Happy Values
	   ShooterSpeed = 11000;
	   P = 0.0009;
	   I = 0.00004;
	   D = 0;
	   F = 0.00005;
	 */
	
	proxy->TrackNewpress("joy2b3");
	proxy->TrackNewpress("joy2b4");
	proxy->TrackNewpress("joy3b3");
	proxy->TrackNewpress("joy3b4");
	proxy->TrackNewpress("joy4b3");
	proxy->TrackNewpress("joy4b4");
	proxy->TrackNewpress("joy2b6");
	proxy->TrackNewpress("joy2b7");
	
	proxy->TrackNewpress(JOY_COPILOT_FIRE);
	
	float shooter_array[AVGSIZE];
		for (int i=0; i<AVGSIZE;i++) {//Use a rolling average to eliminate any temporary freaky readings from shooter
			shooter_array[i]= 0;
		}
		int i = 0;
		
		float shooter_distance = 0;
	
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
		
		// Setting the PID values using joystick buttons
		if(proxy->get("joy2b3n")){
			P = P + JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy2b4n")){
			P = P - JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy3b3n")){
			I = I + JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy3b4n")){
			I = I - JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy4b3n")){
			D = D + JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy4b4n")){
			D = D - JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy2b6n")){
			F = F + .00001;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy2b7n")){
			F = F - .00001;
			ShooterPID.SetPID(P, I, D, F);
		}
		
		
		// Setting setpoint using buttons
		if(proxy->get(SHOOT_SPEED_UP)){
			ShooterSpeed = ShooterSpeed + 50;
		}
		if(proxy->get(SHOOT_SPEED_DOWN)){
			ShooterSpeed = ShooterSpeed - 50;
		}
		
		i++;
		shooter_array[(i%AVGSIZE)] = ShooterEncoder.GetRate();
		shooter_distance = 0;
		
		for(unsigned j = 0;j<AVGSIZE;j++) {
				shooter_distance += shooter_array[j];
		}
		shooter_distance/=AVGSIZE;
		
		ShooterPID.SetSetpoint(-ShooterSpeed);
		
		printf("SetPoint: %5.f AvgSpeed: %5.f RealSpeed: %.5f P: %f I: %f D: %f F: %f\r", ShooterSpeed, shooter_distance, ShooterEncoder.Get(), P, I, D, F); 
		
		//Firing the piston
        if(proxy->get(JOY_COPILOT_FIRE)){
        	PistonState = true;
        } else{
        		PistonState = false;
        	}
        
        ShooterPiston.Set(PistonState);
        
		// <<CHANGEME>>
		// Make this match the declaraction above
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
