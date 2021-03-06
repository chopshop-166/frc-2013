
#pragma once


#include "WPILib.h"
#include "Robot.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
//
// This constant defines how often we want this task to run in the form
// of miliseconds. Max allowed time is 999 miliseconds.
#define CAMERA_CYCLE_TIME (50) // This is the max rate before you cause major lagging and unhappy robots



class CameraTask : public Team166Task
{
	
public:
	
	// task constructor
	CameraTask(void);

	// task destructor
	virtual ~CameraTask(void);

	// Main function of the task
	virtual int Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10);
	
private:
	//Declare Proxy and Robot handles
	Proxy *proxy;				// Handle to proxy
	Robot *lHandle;            // Local handle
	AxisCamera &camera;
	int VALID_IMAGE;
	int iwidth;
	float TARGET_OFFSET;
	float TARGET_HEIGHT;
	float TARGET_WIDTH;
	int particle_id;
	float ar;
	int entered_loop;
	int set_brightness;
	int Flip;
	ColorImage *image;
};
	
