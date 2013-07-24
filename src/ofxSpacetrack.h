#pragma once

#include "ofMain.h"

#include "aiaa/sgp4ext.h"
#include "aiaa/sgp4unit.h"
#include "aiaa/sgp4io.h"

// http://celestrak.com/publications/AIAA/2006-6753/

struct YMD{
    int year;
    int mon;
    int day;
    int hr;
    int minute;
    int sec;
};

class ofxSpacetrack{

    public:
		ofxSpacetrack();
		~ofxSpacetrack();

        //handle TLE
        void setFileTLE(string value){fileTLE = value;};
        string getFileTLE(){return fileTLE;};

		// Current Time
        YMD getCurrentYMD(){return currentYMD;};
        double getCurrentJ2000(){return currentJ2000;};
        double convertYMD2J200(YMD *value){}

		// tracking controls
		// sample
		// set geod
		// orbit elements
		// get position
		// get velocity

		//

    private:
        string fileTLE;         // TLE
        double currentJ2000;    // Julian date                    days from 4713 bc
        YMD currentYMD;         // year, month, day, and time
};



