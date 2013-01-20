/*******************************************************************************
*  Project   		: Framework
*  File Name  		: Proxy166.h     
*  Owner		   	: Software Group (FIRST Chopshop Team 166)
*  Creation Date	: January 18, 2010
*  File Description	: Header for Proxy class
*******************************************************************************/ 
/*----------------------------------------------------------------------------*/
/*  Copyright (c) MHS Chopshop Team 166, 2010.  All Rights Reserved.          */
/*----------------------------------------------------------------------------*/
#pragma once

#include "WPILib.h"
#include "Utility.h"
#include "Team166Task.h"
#include "FrcError.h"
#include <map>

#define NUMBER_OF_JOYSTICKS (4)
#define NUMBER_OF_SWITCHES (8)
#define NUMBER_OF_ANALOG_IN (4)
#define NUMBER_OF_JOY_BUTTONS (12)

//
//This is how often the Driver Station sends back data.
//
#define PROXY_CYCLE_TIME (50) // 50ms

/**
 * @brief Proxy class to store cached values for joysticks and switches.
 * 
 * This class will store the cached values for joysticks and switches. It will also
 * serve as a interface between the joysticks and switches, and any code that wants to 
 * access them like Drive, Autonomous, etc. 
 */

class Proxy : public Team166Task{
	private:
		static map<string,pair<float, SEM_ID> > data;
	public:
		// Proxy 2.0 functions
		bool add(string);
		float get(string, bool=false);
		float set(string, float);
		bool del(string);
		bool exists(string);
		bool reset(void);
		
		bool RegisterCounter(string);
		bool UnregisterCounter(string);
		int GetPendingCount(string);
		bool IsRegistered(string);
		
		bool TrackNewpress(string);
		bool StopTrackingNewpress(string);
		
		~Proxy(void);
		
		bool AreSettingJoysticks();
		void ToggleSettingJoysticks(bool);
		void UseUserJoystick(int,bool);
		bool IsSettingJoystick(int);
		
		void DisableJoystickButtonsByButton(int,int);
		void DisableJoystickAxesByButton(int,int);
		bool JoystickAxesDisabled(int);
		bool JoystickButtonsDisabled(int);
		
		void DisableDSIO(bool);
		bool AreSettingDSIO();
		
		static Proxy *getInstance(void);
		
		virtual int Main(int a2, int a3, int a4, int a5,
					int a6, int a7, int a8, int a9, int a10);
	private:
		Proxy(void);
		// Handle to the proxy
		static Proxy *ProxyHandle;
		
		
		
		// internal method to get values from real joystick
		void SetJoystick(int,Joystick&);
		void SetEnhancedIO(void);
		void setNewpress(void);
		
		/**
		 * A list of all tracked variables and newpresses
		*/
		static map<string,int> tracker;
		static map<string,bool> newpress_list;

		// Refernces to the physical joysticks
		Joystick stick1;
		Joystick stick2;
		Joystick stick3;
		Joystick stick4;
		
		bool wasEnabled;
		bool manualJoystick[4];
		int disableButtons[NUMBER_OF_JOYSTICKS];
		int disableAxes[NUMBER_OF_JOYSTICKS];
		int manualDSIO;
};
