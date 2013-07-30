#pragma once

#include "ofMain.h"

#include "aiaa/SGP4ext.h"
#include "aiaa/SGP4UNIT.h"
#include "aiaa/SGP4io.h"

// http://celestrak.com/publications/AIAA/2006-6753/

// TO DO

// PROVIDE EARTH ROTATION METHODS
// PROVIDE SAT LAT AND LONG

typedef struct YMD{
    int year;
    int mon;
    int day;
    int hr;
    int minute;
    double sec;
}YMD;

typedef struct satInfo{
    int number;
    double semilatusRectum;                         // semilatus rectum               km
    double semimajorAxis;                           // km
    double eccentricity;
    double inclination;
    double omega;                                   // longitude of ascending node - 0.0  to 2pi rad
    double argp;                                    // argument of perigee - 0.0  to 2pi rad
    double nu;                                      // true anomaly - 0.0  to 2pi rad
    double m;                                       // mean anomaly - 0.0  to 2pi rad
    double arglat;                                  // argument of latitude      (ci) 0.0  to 2pi rad - circular inclined
    double truelon;                                 // true longitude            (ce) 0.0  to 2pi rad - circular equatorial
    double lonper;                                  // longitude of periapsis    (ee) 0.0  to 2pi rad - elliptical equatorial
}satInfo;

class ofxSpacetrack{

    public:
		ofxSpacetrack();
		~ofxSpacetrack();

        // Handle TLE
        void        setFileTLE(string value){this->fileTLE = value;};
        string      getFileTLE(){return this->fileTLE;};

        bool        processTLE();

		// Time
        YMD         getCurrentYMD()     {return this->currentYMD;};
        double      getCurrentJ2000()   {return this->currentJ2000;};
        double		getCurrentMFE()		{return this->currentMFE;};
        double      convertYMDtoJ2000( YMD *value );
        YMD         convertJ2000toYMD( double value );

        void        setTimeMultiplier(double value) {this->timeMultiplier = value;};
        double      getTimeMultiplier() { return this->timeMultiplier;};
        void        doSimulatedTime(bool value) {this->simulatedTime = value;};
        bool        isSimulatedTime(){return this->simulatedTime;};

		// Propagator control
        bool        startPropagatorNow(); //start propator
        bool        startPropagatorFromMFE( double fromMFE );
        bool		startPropagatorFromJ2000( double fromJ2000 );
        bool        startPropagatorFromYMD( YMD fromYMD );

        bool        stopPropagatorNow() {this->doPropagation = false; return true;} //stop propagator
        bool        stopPropagatorInMFE( double toMFE );
        bool        stopPropagatorInJ2000( double toJ2000 );
        bool        stopPropagatorInYMD( YMD toYMD );

        void        setPropagationStep( double step ){this->propagationStep = step;};
        double      getPropagationStep( ) { return this->propagationStep;};

        void        update();


		// Positions
		ofPoint     getCurrentPoint(){return this->currentPoint;};
		ofVec3f     getCurrentVelocity(){return this->currentVelocity;};

		// Info
		satInfo     getSatInfo(){return this->info;};


    private:
        // Handle TLE
        string      fileTLE;            // TLE
		string		firstLine;
		string		secondLine;

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
		double		v[3], r[3];
        ofPoint     currentPoint;       // get position - geocentric equatorial position
        ofVec3f     currentVelocity;    // get velocity - velocity vectors

        satInfo     info;     // orbit elements e info

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



