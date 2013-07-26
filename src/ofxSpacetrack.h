#pragma once

#include "ofMain.h"

#include "aiaa/SGP4ext.h"
#include "aiaa/SGP4UNIT.h"
#include "aiaa/SGP4io.h"

// http://celestrak.com/publications/AIAA/2006-6753/

typedef struct YMD{
    int year;
    int mon;
    int day;
    int hr;
    int minute;
    double sec;
}YMD;

typedef struct satInfo{
}satInfo;

class ofxSpacetrack{

    public:
		ofxSpacetrack();
		~ofxSpacetrack();

        // Handle TLE
        void        setFileTLE(string value){this->fileTLE = value;};
        string      getFileTLE(){return this->fileTLE;};
        bool        processTLE();
        bool        validTLE();

		// Time
        YMD         getCurrentYMD()     {return this->currentYMD;};
        double      getCurrentJ2000()   {return this->currentJ2000;};
        double		getCurrentMFE()		{return this->currentMFE;};
        double      convertYMDtoJ2000( YMD *value );
        YMD         convertJ2000toYMD( double value );

        void        setTimeMultiplier(double value) {this->timeMultiplier = value;};
        double      getTimeMultiplier() { return this->timeMultiplier;};
        void        doSimulatedTime(bool value) {this->simulatedTime = value;};
        bool        getSimulatedTime(){return this->simulatedTime;};

		// Propagator control
        bool        startPropagatorNow(); //start propator
        bool        startPropagatorFromMFE( double fromMFE );
        bool		startPropagatorFromJ2000( double fromJ2000 );
        bool        startPropagatorFromYMD( YMD fromYMD );

        bool        stopPropagatorNow(); {this->doPropagation = false;} //stop propagator
        bool        stopPropagatorInMFE( double toMFE );
        bool        stopPropagatorInJ2000( double toJ2000 );
        bool        stopPropagatorInYMD( YMD toYMD );

        void        setPropagationStep( double step ){this->propagationStep = step;};
        double      getPropagationStep( ) { return this->propagationStep;};

        void        update();


		// Positions
		ofPoint     getCurrentPoint(){return this->currentPoint;};
		ofVec3f     getCurrentVelocity(){return this->currentVelocity;};


    private:
        // Handle TLE
        string      fileTLE;            // TLE

        // Time
        double      currentJ2000;       // Julian date                    days from 4713 bc
        YMD         currentYMD;         // year, month, day, and time
        double      currentMFE;          // Minutes from epoch

        //Propagator control

        double      propagationStep;    // Propagation step
        bool        simulatedTime;      // Set to simulation time
        bool        doPropagation;      // do the propagation

        double      timeMultiplier;
        double      startMFE;
        double      stopMFE;

         elsetrec    satrec;             // Satellite informations (From the library)

        //Position from Earth Central Body - TEME
        ofPoint     currentPoint;       // get position - geocentric equatorial position
        ofVec3f     currentVelocity;    // get velocity - velocity vectors
        satInfo     currentOrbital;     // orbit elements

		//Gravitational Constants
		double tumin; // minutes in one time unit
		double mu; //earth gravitational parameter
		double radiusearthkm; //radius of the earth in km
		double xke; //reciprocal of tumin
		double j2;
		double j3;
		double j4;
		double j3oj2; // j3/j2
};



