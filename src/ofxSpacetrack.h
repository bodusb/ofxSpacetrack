#pragma once

#include "ofMain.h"

#include "aiaa/sgp4ext.h"
#include "aiaa/sgp4unit.h"
#include "aiaa/sgp4io.h"

// http://celestrak.com/publications/AIAA/2006-6753/

typedef struct YMD{
    int year;
    int mon;
    int day;
    int hr;
    int minute;
    double sec;
}YMD;

typedef struct orbital{
}orbital;

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
        double      convertYMDtoJ2000( YMD *value );
        YMD         convertJ2000toYMD( double value );

        bool        setTimeMultiplier(double value) {this->timeMultiplier = value;};
        double      getTimeMultiplier() { return this->timeMultiplier;};

		// Propagator control
        bool        startPropagatorNow();
        bool        startPropagatorFromMFE( double fromMFE );
        bool		startPropagatorFromJ2000( double fromJ2000 );
        bool        startPropagatorFromYMD( YMD fromYMD );

        bool        stopPropagatorNow(); //stop propagator
        bool        stopPropagatorInMFE( double toMFE );
        bool        stopPropagatorInJ2000( double toJ2000 );
        bool        stopPropagatorInYMD( YMD toYMD );

        void        setPropagationStep( double step ){this->propagationStep = step;};
        double      getPropagationStep( ) { return this->propagationStep;};

       // void        update();

		// Positions
		ofPoint     *getCurrentPoint(){return &this->currentPoint;};
		ofVec3f     *getCurrentVelocity(){return &this->currentVelocity;};


    private:
        // Handle TLE
        string      fileTLE;            // TLE
        // Time
        double      currentJ2000;       // Julian date                    days from 4713 bc
        YMD         currentYMD;         // year, month, day, and time
        double      currentMFE;          // Minutes from epoch
        double      timeMultiplier;

        double      startMFE;
        double      stopMFE;

        //Propagator control
        elsetrec    satrec;             // Satellite informations (From the library)
        double      propagationStep;    // Propagation step

        //Position from Earth Central Body - TEME
        ofPoint     currentPoint;       // get position - geocentric equatorial position
        ofVec3f     currentVelocity;    // get velocity - velocity vectors
        orbital     currentOrbital;     // orbit elements


};



