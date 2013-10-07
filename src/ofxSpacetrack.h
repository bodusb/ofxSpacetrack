#pragma once

#include "ofMain.h"

// SATELLITE LIBRARIES
#include "aiaa/SGP4ext.h"
#include "aiaa/SGP4UNIT.h"
#include "aiaa/SGP4io.h"

// ASTRO LIBRARY
#include "aaplus/AA+.h"

// http://celestrak.com/publications/AIAA/2006-6753/

// TO DO

// PROVIDE EARTH ROTATION METHODS

#define deg2rad			(PI / 180.0)
#define rad2deg			(180.0 / PI)
#define omega_E			1.00273791	// Earth rotations per sidereal day
#define secondsDay		86400.0	// 24 * 60 * 60
#define minutesDay		1440.0	// 24 * 60
#define e_R				6378.135	// Radius of the Earth (km)
#define f				(1.0 / 298.26)	// Ellipticity of the Earth
#define dAstronomicalUnit    149597870.7	// In km

typedef struct astro{
	string name;
	double latitude;		// degrees
	double longitude;		// degrees
	double altitude;		// km
	double radius;			// km
	ofPoint geoPosition;	// km
	ofVec3f rotation;		// degrees
	double sizeAdaptation;		//coeficient to adjust a novel size
	double distanceAdaptation;	//coeficient to adjust a novel ecliptic distance

	bool available;
}astro;

typedef struct groundStation{
	int			id;
	string		name;
	double		latitude;
	double		longitude;
	double		altitude;
	ofPoint		geoPosition;
	bool		enabled;
}groundStation;


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
		void		setTimezone(int value){this->timezone = value;};
		int			getTimezone() {return this->timezone;};

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

		double		getLatitude() { return this->latitude;};
		double		getLongitude() { return this->longitude;};
		double		getAltitude() {return this->altitude;};

		bool		isOnEclipse() {return this->onEclipse;};

		// Astros positions

		ofPoint		getSunPosition() { return this->sun.geoPosition;};
		double		getSunRadius() {return this->sun.radius;};
		void		setSunCalc(bool value) {this->sunCalc = value;};
		bool		getSunCalc() { return this->sunCalc;};
		bool		isSunPositionAvailable() {return this->sun.available;};

		ofPoint		getMoonPosition() { return this->moon.geoPosition;};
		double		getMoonRadius() { return this->moon.radius;};
		void		setMoonCalc(bool value) {this->moonCalc = value;};
		bool		getMoonCalc() { return this->moonCalc;};
		bool		isMoonPositionAvailable() { return this->moon.available;};

		// Ground station
		vector<groundStation*>	gsList;
		void					addGroundStation(int id, string v_name, double v_lat, double v_long, double v_alt = 0);
		groundStation*			getGroundStation(string v_name);
		groundStation*			getGroundStation(int v_id);
		void					setGroundStationStatus(string v_name,bool v_state);
		void					setGroundStationStatus(int v_id,bool v_state);


		// Info - http://en.wikipedia.org/wiki/Orbital_elements
		

		//shape of the ellipse, describing how much it is elongated compared to a circle.
		void		setEccentricity(double v_ecc) { this->satrec.ecco = v_ecc; satrec.alta = satrec.a*(1.0 + satrec.ecco) - 1.0;  satrec.altp = satrec.a*(1.0 - satrec.ecco) - 1.0;}; 
		double		getEccentricity() { return this->satrec.ecco;}

		// the sum of the periapsis and apoapsis distances divided by two. For circular orbits, the semimajor axis is the distance between the centers of the bodies, not the distance of the bodies from the center of mass.
		void		setSemimajorAxis(double v_a) { this->satrec.a = v_a; satrec.alta = satrec.a*(1.0 + satrec.ecco) - 1.0;  satrec.altp = satrec.a*(1.0 - satrec.ecco) - 1.0;}; 
		double		getSemimajorAxis() { return this->satrec.a;};

		// rad - vertical tilt of the ellipse with respect to the reference plane, measured at the ascending node (where the orbit passes upward through the reference plane)
		void		setInclination(double v_incl_in_rad) { this->satrec.inclo = v_incl_in_rad;}; 
		double		getInclination() { return this->satrec.inclo;};

		// rad - horizontally orients the ascending node of the ellipse (where the orbit passes upward through the reference plane) with respect to the reference frame's vernal point
		void		setLongAscendingNode(double v_ascNode_in_rad) { this->satrec.nodeo = v_ascNode_in_rad; };
		double		getLongAscendingNode() { return this->satrec.nodeo;};

		// rad - defines the orientation of the ellipse in the orbital plane, as an angle measured from the ascending node to the periapsis (the closest point the second body comes to the first during an orbit).
		void		setPeriapsisArgument(double v_argp_in_rad) { this->satrec.argpo = v_argp_in_rad;};
		double		getPeriapsisArgument() { return this->satrec.argpo;};

		// rad - defines the position of the orbiting body along the ellipse at a specific time
		void		setMeanAnomaly(double v_mean_in_rad) { this->satrec.mo = v_mean_in_rad;};
		double		getMeanAnomaly() { return this->satrec.mo;};

		// sat number
		long int	getSatNumber() { return this->satrec.satnum;};
    

    private:
        // Handle TLE
        string      fileTLE;            // TLE
		string		firstLine;
		string		secondLine;

        // Time
        double      currentJ2000;       // Julian date                    days from 4713 bc
        YMD         currentYMD;         // year, month, day, and time
        double      currentMFE;          // Minutes from epoch
		int			timezone;

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

		double		latitude;
		double		longitude;
		double		altitude;
		bool		onEclipse;

		double		rotz;

		double		ThetaG_JD( double v_jd);
		void TEMEtoLLA(ofPoint r, double v_jd, double &lat, double &lon, double &alt);

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


		//earth
		//astro earth;
		//bool calcEarth;

		//moon
		astro moon;
		bool moonCalc;

		//sun
		astro sun;
		bool sunCalc;

		//star list
		//vector<astro> starList;
		//bool calcStars;

		// Ground stations
		

};



