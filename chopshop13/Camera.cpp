/*******************************************************************************
*  Project   		: Chopshop13
*  File Name  		: Camera.cpp     
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
};

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
	unsigned int PutOne(void);     // Log the values needed-add in arguments
};

unsigned int CameraTaskLog::PutOne(void)
{
	struct abuf *ob;               // Output buffer
	
	// Get output buffer
	if ((ob = (struct abuf *)GetNextBuffer(sizeof(struct abuf)))) {
		
		// Fill it in.
		clock_gettime(CLOCK_REALTIME, &ob->tp);
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
	);
	
	// Done
	return (sizeof(struct abuf));
}


// task constructor
CameraTask::CameraTask(void):camera(AxisCamera::GetInstance("10.1.66.11"))
		
		
{
	Start((char *)"166Camera", CAMERA_CYCLE_TIME);
	// Register the prox
	proxy = Proxy::getInstance();

	camera.WriteResolution(AxisCamera::kResolution_160x120);//Set the rez of the camera
	camera.WriteBrightness(30);//Set the init brightness of the camera
	camera.WriteExposureControl(AxisCameraParams::kExposure_Hold); //Keep the exposure of the camera constant
	camera.WriteWhiteBalance(AxisCameraParams::kWhiteBalance_Hold); //Keep the whitebalence of the camera constant
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
	}; //Define the required size of a particle 
	
	iwidth = 0;
	float ar = 0; //ar is aspect-ratio
	proxy->add("VALID_IMAGE"); //Add valid image to proxy, a variable used to broadcast whether the camera has a valid target
	proxy->add("TARGETOFFSET");//The proxy variable used to tell other tasks the offset of the chosen value from the center of the image with a value of -1 to 1
	TARGET_HEIGHT = 0;
	TARGET_WIDTH = 0;
	ParticleAnalysisReport *target;//Define custom set of final particles used to decide the target
	
	particle_id = 0;
	
    // General main loop (while in Autonomous or Tele mode)
	while (true) {
		
		if (lHandle->IsAutonomous() == 0){ //Raise brightness for Tele-op so that humans can see stuff as well
			camera.WriteBrightness(50);
		}
		else
		{
			camera.WriteBrightness(30);
		}
		
		VALID_IMAGE = 0;
		ColorImage *image = new ColorImage(IMAQ_IMAGE_HSL);//Create a new image variable of type ColorImage in HSL format
		
		camera.GetImage(image);//Get the image from the camera and save it into the image variable.
		
		iwidth = image->GetWidth();//The the image of the image we just got.
		
		//printf("Width: %d\r",iwidth);//Print the width of the image, DEBUG ONLY
		
		if (iwidth != 0)//Check to see if an image exists by checking width
		{
		entered_loop = 0;
		BinaryImage *thresholdImage = image->ThresholdHSV(threshold);	// get just the red target pixels
//   consider adding histogram to set lower end of V threshold
		
		
		//OPTIONAL//BinaryImage *thresholdImage = image->ThresholdRGB(threshold);	// get just the green target pixels
		
		BinaryImage *bigObjectsImage = thresholdImage->RemoveSmallObjects(false, 2);  // remove small objects (noise)
		BinaryImage *convexHullImage = bigObjectsImage->ConvexHull(false);  // fill in partial and full rectangles
		BinaryImage *filteredImage = convexHullImage->ParticleFilter(criteria, 2);  // find the rectangles
		vector<ParticleAnalysisReport> *reports = filteredImage->GetOrderedParticleAnalysisReports();  // get the results
		
		//IMAGE WRITES, USE FOR DEBUGGING
		//image->Write("Original.png");
		//bigObjectsImage->Write("Threshold.png");
		//filteredImage->Write("Filtered.png");
		
		
		for (unsigned i = 0; i < reports->size(); i++) {//This loop goes through all final particles
				ParticleAnalysisReport *r = &(reports->at(i));
				TARGET_HEIGHT = float(r->boundingRect.height);//get individual particle height
				TARGET_WIDTH = float(r->boundingRect.width);//get individual particle width
				ar = TARGET_WIDTH / TARGET_HEIGHT;//Calculate aspect ratio
				if ((ar > .5) && (ar < 2.0))//Check if the shape is square-ish...
				{
					if (entered_loop == 1)
					{
						if (r->center_mass_y > target->center_mass_y)//Choose the lowest target
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
				//USE THIS PRINTF FOR DEBUG
				//printf("particle: %d  (%d,%d)\r\n", i, r->center_mass_x, r->center_mass_y);
				
			}
				
			
			if ((reports->size() > 0) && (entered_loop == 1))
			{
				TARGET_OFFSET =(target->center_mass_x - 80) / 80.0;//calculate offset by subtracting half the width of the camera rez
				TARGET_HEIGHT = float(target->boundingRect.height);//get the final targets height
				TARGET_WIDTH = float(target->boundingRect.width);//get the final targets width
				ar = TARGET_WIDTH / TARGET_HEIGHT;//calculate the final target's a-s
				
				//USE THIS PRINTF TO DEBUG CHOOSING CORRECT TARGET
				DPRINTF("PARTICLE CHOSEN: %d   ALTITUDE: %d   %f BY %f RATIO: %f  OFFSET: %f\r", particle_id,target->center_mass_y,TARGET_WIDTH,TARGET_HEIGHT, ar, TARGET_OFFSET );
				
				VALID_IMAGE = 1;//tell everyone we have a target
				
				proxy->set("VALID_IMAGE",VALID_IMAGE);
				proxy->set("TARGETOFFSET",TARGET_OFFSET);
			}
			else
			{
				VALID_IMAGE = 0;//tell everyone we wont have an image
				proxy->set("VALID_IMAGE",VALID_IMAGE);
				
				//USE FOR DEBUGGING WHETHER A TARGET IS RECONIZED AND COMIC RELIEF
				//printf("DANGER WILL ROBINSON!DANGER!");
			}
			
		
		
		//DELETE IMAGES STUFF TO AVOID ANGRY ROBOTS, VERRYYY IMPORTANT
		delete reports;
		delete filteredImage;
		delete convexHullImage;
		delete bigObjectsImage;
		delete thresholdImage;
		delete image;
		}
		else
		{
			//USE FOR DEBUGGING WHETHER CAMERA IS GETTING AN IMAGE
			//printf("Waiting for Image \r");	
		}
		
		sl.PutOne();
		
		// Wait for our next lap
		WaitForNextLoop();		
	}
	return (0);
	
};
