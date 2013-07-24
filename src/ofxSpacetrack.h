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
    int sec;
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
        bool        initPropagator(); //re-starts TLE, inits sgp4, call propagator
        bool        initPropagator( double atMFE );
        bool        initPropagator( double atJ2000 );
        bool        initPropagator( YMD atYMD );

        bool        stopPropagator(); //stop propagator
        bool        stopPropagator( double atMFE );
        bool        stopPropagator( double atJ2000 );
        bool        stopPropagator( YMD atYMD );


		// Positions
		ofPoint     *getCurrentPoint(){return *this->currentPoint;};
		ofVec3f     *getCurrentVelocity(){return *this->currentVelocity;};


    private:
        // Handle TLE
        string      fileTLE;            // TLE
        // Time
        double      currentJ2000;       // Julian date                    days from 4713 bc
        YMD         currentYMD;         // year, month, day, and time
        double      currentMFE          // Minutes from epoch
        double      timeMultiplier;

        //Propagator control
        elsetrec    satrec;             // Satellite informations (From the library)


        //Position
        ofPoint     currentPoint;       // get position
        ofVec3f     currentVelocity;    // get velocity
                                        // orbit elements


};



