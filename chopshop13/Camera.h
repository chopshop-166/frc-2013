
#pragma once


#include "WPILib.h"
#include "Robot.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
//
// This constant defines how often we want this task to run in the form
// of miliseconds. Max allowed time is 999 miliseconds.
// You should rename this when you copy it into a new file
// <<CHANGEME>>
#define CAMERA_CYCLE_TIME (100) // 10ms

// Rename this, too, or you'll run into collisions
// <<CHANGEME>>
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
};
	
