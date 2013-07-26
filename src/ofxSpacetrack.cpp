#include "ofxSpacetrack.h"

ofxSpacetrack::ofxSpacetrack(){

    this->doPropagation = false;
    this->simulatedTime = false;
    this->propagationStep = 0;

}

ofxSpacetrack::~ofxSpacetrack(){
}

// Convert from YMD to Julian date
double ofxSpacetrack::convertYMDtoJ2000(YMD *value){
    double j2000;
    //call function
    jday(value->year,value->mon,value->day,value->hr,value->minute,value->sec,j2000);
    return j2000;
}

// Convert from Julian to YMD date
YMD ofxSpacetrack::convertJ2000toYMD(double value){
    YMD ymd;
    invjday(value,ymd.year,ymd.mon,ymd.day,ymd.hr,ymd.minute,ymd.sec);
    return ymd;
}


// Verify if the TLE read is valid
bool ofxSpacetrack::validTLE(){
	return true;
}




// Process the TLE

bool ofxSpacetrack::processTLE(){


	// TRIES TO READ THE TLE
	if(this->fileTLE == "") return false;

	////SETs initial GRAV constant
    getgravconst(wgs72, tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2 );

	// OPENS AND READ FILETLE
	ofBuffer file = ofBufferFromFile(this->fileTLE);

	string firstLine,secondLine;

	do{
		string line = file.getNextLine();

        vector<char> l(firstLine.size() +1);
        copy(line.begin(),line.end(),l.begin());

		if( l.front() == '1') firstLine = line;
		if( l.front() == '2') secondLine = line;
	}while(  !file.isLastLine());
	// do just one sat -- later fix to set a vector of sats

	//adapts string to char[130]
	vector<char> fl(firstLine.size() +1);
	copy(firstLine.begin(),firstLine.end(),fl.begin());
	vector<char> sl(secondLine.size() +1);
	copy(secondLine.begin(),secondLine.end(),sl.begin());

	twoline2rv(&*fl.begin(),&*sl.begin(),'m','m',wgs72,this->startMFE,this->stopMFE,this->propagationStep,satrec);

    //Setup starting time
    double jdcurrent;
    jday( ofGetYear(),ofGetMonth(),ofGetDay(),ofGetHours(),ofGetMinutes(),ofGetSeconds(), jdcurrent);	//YMD -> JD
    this->currentMFE = (jdcurrent - satrec.jdsatepoch)*1440.0;
    this->currentJ2000 = satrec.jdsatepoch;                                                               //update JD
    invjday(this->currentJ2000, this->currentYMD.year, this->currentYMD.mon, this->currentYMD.day,
                                this->currentYMD.hr, this->currentYMD.minute, this->currentYMD.sec);    //update YMD


	return true;
}
//-----------------------------------------------------------
bool ofxSpacetrack::startPropagatorNow(){
	double jdstart;
    jday( ofGetYear(),ofGetMonth(),ofGetDay(),ofGetHours(),ofGetMinutes(),ofGetSeconds(), jdstart );
	this->startMFE = (jdstart - satrec.jdsatepoch) * 1440.0;

	this->currentMFE = this->startMFE;
	this->doPropagation = 1;
    return true;
}

//-----------------------------------------------------------
bool ofxSpacetrack::startPropagatorFromMFE( double fromMFE ){
	this->startMFE = fromMFE;
	return true;
}

//-----------------------------------------------------------
bool ofxSpacetrack::startPropagatorFromJ2000( double fromJ2000 ){
	this->startMFE = (fromJ2000 - satrec.jdsatepoch) * 1440.0;
	return true;
}

//-----------------------------------------------------------
bool ofxSpacetrack::startPropagatorFromYMD( YMD fromYMD ){
    double jdstart;
    jday( fromYMD.year, fromYMD.mon, fromYMD.day,fromYMD.hr,fromYMD.minute,fromYMD.sec, jdstart );
    this->startMFE = (jdstart - satrec.jdsatepoch) * 1440.0;
    return true;
}

//-----------------------------------------------------------

bool ofxSpacetrack::stopPropagatorNow(){
	double jdstop;
    jday( ofGetYear(),ofGetMonth(),ofGetDay(),ofGetHours(),ofGetMinutes(),ofGetSeconds(), jdstop);
	this->stopMFE = (jdstop- satrec.jdsatepoch) * 1440.0;
    return true;
}

//-----------------------------------------------------------
bool ofxSpacetrack::stopPropagatorInMFE( double toMFE ){
	this->stopMFE = toMFE;
	return true;
}

//-----------------------------------------------------------
bool ofxSpacetrack::stopPropagatorInJ2000( double toJ2000 ){
	this->stopMFE = (toJ2000 - satrec.jdsatepoch) * 1440.0;
	return true;
}

//-----------------------------------------------------------
  bool ofxSpacetrack::stopPropagatorInYMD( YMD toYMD ){
    double jdstop;
    jday( toYMD.year, toYMD.mon, toYMD.day, toYMD.hr, toYMD.minute, toYMD.sec, jdstop );
    this->stopMFE = (jdstop - satrec.jdsatepoch) * 1440.0;
    return true;
}

//-----------------------------------------------------------

void ofxSpacetrack::update(){
    if(!this->doPropagation) return;

    if(!this->simulatedTime){
        double jdcurrent;
        jday( ofGetYear(),ofGetMonth(),ofGetDay(),ofGetHours(),ofGetMinutes(),ofGetSeconds(), jdcurrent);	//YMD -> JD
        this->currentMFE = (jdcurrent - satrec.jdsatepoch)*1440.0; // JD -> MFE
    } else {
        this->currentMFE += timeMultiplier;
    }

    //------------------------------------------------------------------
    //                  PROPAGATOR
    double		v[3], r[3];
    sgp4(wgs72, satrec, this->currentMFE, &r[0], &v[0]);
    //------------------------------------------------------------------

    // Update positions
    this->currentPoint.set(r[0],r[1],r[2]);
    this->currentVelocity.set(v[0],v[1],v[2]);

    // Update times
    this->currentJ2000 = satrec.jdsatepoch + this->currentMFE/1440.0;                                         //update JD
    invjday(this->currentJ2000, this->currentYMD.year, this->currentYMD.mon, this->currentYMD.day,
                                this->currentYMD.hr, this->currentYMD.minute, this->currentYMD.sec);    //update YMD

    // get updated orbital
    //rv2coe()


}


