/*******************************************************************************
*  Project   		: chopshop13
*  File Name  		: CameraTask.cpp     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 27, 2013
*  File Description	: CameraTask Stuffs
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2013.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Find & Replace "Template" with the name you would like to give this task     */
/* Find & Replace "Testing" with the name you would like to give this task      */
/* Find & Replace "CameraTask" with the name you would like to give this task */
/*------------------------------------------------------------------------------*/

#include "WPILib.h"
#include "Camera.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"


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
class CameraTaskLog : public MemoryLog
{
public:
	CameraTaskLog() : MemoryLog(
			sizeof(struct abuf), CAMERA_CYCLE_TIME, "CameraTask",
			"Seconds,Nanoseconds,Elapsed Time\n" // Put the names of the values in here, comma-seperated
			) {
		return;
	};
	~CameraTaskLog() {return;};
	unsigned int DumpBuffer(          // Dump the next buffer into the file
			char *nptr,               // Buffer that needs to be formatted
			FILE *outputFile);        // and then stored in this file
	// <<CHANGEME>>
	unsigned int PutOne(void);     // Log the values needed-add in arguments
};

// Write one buffer into memory
// <<CHANGEME>>
unsigned int CameraTaskLog::PutOne(void)
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
unsigned int CameraTaskLog::DumpBuffer(char *nptr, FILE *ofile)
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
CameraTask::CameraTask(void):camera(AxisCamera::GetInstance("10.1.66.11"))
		
		
{
	Start((char *)"166Camera", CAMERA_CYCLE_TIME);
	// ^^^ Rename those ^^^
	// <<CHANGEME>>
	// Register the prox
	proxy = Proxy::getInstance();

	camera.WriteResolution(AxisCamera::kResolution_320x240);
	camera.WriteBrightness(30);
	camera.WriteExposureControl(AxisCameraParams::kExposure_Hold);
	camera.WriteWhiteBalance(AxisCameraParams::kWhiteBalance_Hold);
	return;
};
	
// task destructor
CameraTask::~CameraTask(void)
{
	return;
};
	
// Main function of the task
int CameraTask::Main(int a2, int a3, int a4, int a5,
			int a6, int a7, int a8, int a9, int a10)
{
	CameraTaskLog sl;                   // log
	
	// Let the world know we're in
	DPRINTF(LOG_DEBUG,"In the 166 Template task\n");
	
	// Wait for Robot go-ahead (e.g. entering Autonomous or Tele-operated mode)
	// lHandle = Robot::getInstance() MUST go after this, otherwise code breaks
	WaitForGoAhead();
	
	// Register our logger
	lHandle = Robot::getInstance();
	lHandle->RegisterLogger(&sl);
	Threshold threshold(60, 110, 90, 255, 20, 255);	//HSV threshold criteria, ranges are in that order ie. Hue is 60-100
	ParticleFilterCriteria2 criteria[] = {
										{IMAQ_MT_BOUNDING_RECT_WIDTH, 30, 400, false, false},
										{IMAQ_MT_BOUNDING_RECT_HEIGHT, 30, 400, false, false}
	};
	iwidth = 0;
	float ar = 0;
	proxy->add("VALID_IMAGE");
	proxy->add("TARGETOFFSET");
	TARGET_HEIGHT = 0;
	TARGET_WIDTH = 0;
	ParticleAnalysisReport *target;
	
	particle_id = 0;
	
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
		
		if (lHandle->IsAutonomous() == 0){
		camera.WriteBrightness(50);}
		
		VALID_IMAGE = 0;
		ColorImage *image = new ColorImage(IMAQ_IMAGE_HSL);
		
		camera.GetImage(image);
		iwidth = image->GetWidth();
		//printf("Width: %d\r",iwidth);
		if (iwidth != 0)
		{
		entered_loop = 0;
		BinaryImage *thresholdImage = image->ThresholdHSV(threshold);	// get just the red target pixels
//   consider adding histogram to set lower end of V threshold
		
		
		//OPTIONAL//BinaryImage *thresholdImage = image->ThresholdRGB(threshold);	// get just the green target pixels
		
		BinaryImage *bigObjectsImage = thresholdImage->RemoveSmallObjects(false, 2);  // remove small objects (noise)
		BinaryImage *convexHullImage = bigObjectsImage->ConvexHull(false);  // fill in partial and full rectangles
		BinaryImage *filteredImage = convexHullImage->ParticleFilter(criteria, 2);  // find the rectangles
		vector<ParticleAnalysisReport> *reports = filteredImage->GetOrderedParticleAnalysisReports();  // get the results
		
		//IMAGE WRIGHTS, USE FOR DEBUGGING
		//image->Write("Original.png");
		//bigObjectsImage->Write("Threshold.png");
		//filteredImage->Write("Filtered.png");
		
		
		for (unsigned i = 0; i < reports->size(); i++) {
				ParticleAnalysisReport *r = &(reports->at(i));
				TARGET_HEIGHT = float(r->boundingRect.height);
				TARGET_WIDTH = float(r->boundingRect.width);
				ar = TARGET_WIDTH / TARGET_HEIGHT;
				if ((ar > .5) && (ar < 2.0))
				{
					if (entered_loop == 1)
					{
						if (r->center_mass_y > target->center_mass_y)
						{
							target = &(reports->at(i));
							particle_id = i;
						}
					}
					else
					{
						target = &(reports->at(i));
						particle_id = i;
						entered_loop = 1;
					}
				
				}
				printf("particle: %d  (%d,%d)\r\n", i, r->center_mass_x, r->center_mass_y);
				
			}
				
			
			if ((reports->size() > 0) && (entered_loop == 1))
			{
				TARGET_OFFSET =(target->center_mass_x - 160) / 160.0;
				TARGET_HEIGHT = float(target->boundingRect.height);
				TARGET_WIDTH = float(target->boundingRect.width);
				ar = TARGET_WIDTH / TARGET_HEIGHT;
				printf("PARTICLE CHOSEN: %d   ALTITUDE: %d   %f BY %f RATIO: %f  OFFSET: %f\r", particle_id,target->center_mass_y,TARGET_WIDTH,TARGET_HEIGHT, ar, TARGET_OFFSET );
				VALID_IMAGE = 1;
				proxy->set("VALID_IMAGE",VALID_IMAGE);
				proxy->set("TARGETOFFSET",TARGET_OFFSET);
				//ENABLE FOR DEBUGGING//
				//printf("Status:Image Aquired");
			}
			else
			{
				VALID_IMAGE = 0;
				proxy->set("VALID_IMAGE",VALID_IMAGE);
				//printf("DANGER WILL ROBINSON!DANGER!");
			}
			
		
		
		//DELETE IMAGES STUFF TO AVOID ANGRY ROBOTS
		delete reports;
		delete filteredImage;
		delete convexHullImage;
		delete bigObjectsImage;
		delete thresholdImage;
		delete image;
		}
		else
		{
			//USE FOR DEBUGGING CAMERA
		//printf("Waiting for Image \r");	
		}
		
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
