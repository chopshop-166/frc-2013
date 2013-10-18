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
	ShooterPID.SetInputRange(-20000, 0);
	//Set start state waiting for input
	state = WaitForGO;
	//We aren't shooting at the beginning
	shots_remaining = 0;
	countdown = 0;
	//Init Shooter Speed to its correct value
	//Init PID values to their defaults
	P = 0.0009;
	I = 0.00004;
	D = 0;
	F = 0.00003;
	ShooterPID.SetPID(P, I, D, F);
	SetPoint = 0;
	EncoderVal = 0;
	PistonCount = 0;
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
	proxy->TrackNewpress(JOY_COPILOT_SPINUP_TRACK);
	
	
	
	/* Happy Values
	   ShooterSpeed = 11000;
	   P = 0.0009;
	   I = 0.00004;
	   D = 0;
	   F = 0.00005;
	 */
	/*
	proxy->TrackNewpress("joy1b4");
	proxy->TrackNewpress("joy1b5");
	proxy->TrackNewpress("joy2b4");
	proxy->TrackNewpress("joy2b5");
	proxy->TrackNewpress("joy2b8");
	proxy->TrackNewpress("joy2b9");
	*/
	float ShooterArray[AVGSIZE];
	for (int i=0; i<AVGSIZE;i++) {//Use a rolling average to eliminate any temporary freaky readings from shooter
		ShooterArray[i]= 0;
	}
	int i = 0;
	
	float AvgSpeed = 0;
	
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
		
		// Setting the PID values using joystick buttons
		/*
		if(proxy->get("joy1b4n")){
			P = P + JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy1b5n")){
			P = P - JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy2b4n")){
			I = I + JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy2b5n")){
			I = I - JOYINC;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy2b8n")){
			F = F + .00001;
			ShooterPID.SetPID(P, I, D, F);
		}
		if(proxy->get("joy2b9n")){
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
		*/
		//Average values read in
		// Any value that is more than 2000 counts off from average
		// Will be put in as .05 of the difference subtracted from the current average
		EncoderVal = -ShooterEncoder.GetRate();
		
		FilterDelta = AvgSpeed - EncoderVal;
		
		i++;
		if(FilterDelta > 3000){
			EncoderVal = AvgSpeed - 0.05 * FilterDelta;
			ShooterArray[(i%AVGSIZE)] = EncoderVal;
			AvgSpeed = 0;
		}
			else{
				ShooterArray[(i%AVGSIZE)] = EncoderVal;
				AvgSpeed = 0;
		}
		
		for(unsigned j = 0;j<AVGSIZE;j++) {
				AvgSpeed += ShooterArray[j];
		}
		AvgSpeed/=AVGSIZE;
		
		//ShooterPID.SetSetpoint(-ShooterSpeed);
		
		//printf("SetPoint: %5.f AvgSpeed: %5.f RealSpeed: %.5f P: %f I: %f D: %f F: %f\r"
		//		, ShooterPID.GetSetpoint(), AvgSpeed, ShooterEncoder.GetRate(), P, I, D, F); 
		
		//Fire the piston no matter what
        if(proxy->get(JOY_COPILOT_FIRE)){
        	PistonState = true;
       	} else {
       		PistonState = false;
       	}

        
        //Automatically Fire
      
        switch (state) {
        case WaitForGO:
        	  if(proxy->get(JOY_COPILOT_FIRE_AUTO) < -.1) {
        		  shots_remaining = 4;
        		  state = StartShooter;
        	  }
        	//Wait until we actually need to spin
        	break;
        case StartShooter:
        	//Check conditions
        	if (SetPoint == 0) {
        		state = SpinUp;
        	} else {
        		state = CheckTime;
        		countdown = 0;
        	}
        	break;
        case SpinUp:
        	//We weren't spinning, lets start!
        	SetPoint = -SHOOTSPEED;
        	state = CheckTime;
        	break;
        case CheckTime:
        	//Wait 1 Sec
        	countdown++;
        	if (countdown > 20) {
        		state = CheckSpeed;
        		countdown = 0;
        	}
        	break;
        case CheckSpeed:
        	//Are we at speed?
        	if (AvgSpeed >= (.975*SHOOTSPEED)) {
        		state = Fire;
        	}
        	break;
        case Fire:
        	//Extend Piston, fire frisbee
        	countdown++;
        	PistonState = 1;
        	if (countdown > 5) {
        		state = Retract;
        	}
        	break;
        case Retract:
        	//Bring the piston back in
        	PistonState = 0;
        	shots_remaining--;
        	state = IsDone;
        	break;
        case IsDone:
        	//We're done, Spin down
        	if(shots_remaining == 0) {
        		SetPoint = 0;
        		state = WaitForGO;
        	} else {
        		state = StartShooter;
        	}
        }
    	ShooterPiston.Set(PistonState);
        
        //Spinup shooter
        if ((proxy->get(JOY_COPILOT_SPINUP)) && (SetPoint == 0)) {
        	SetPoint = -SHOOTSPEED;
        } else if (proxy->get(JOY_COPILOT_SPINUP)) {
        	SetPoint = 0;
        }
        printf("GO: %f",proxy->get(JOY_COPILOT_FIRE_AUTO));
        ShooterPID.SetSetpoint(SetPoint);
        //printf("SetPoint %5.f Speed %5.f State: %d\r", SetPoint, ShooterEncoder.GetRate(), state);
        // <<CHANGEME>>
		// Make this match the declaraction above
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
