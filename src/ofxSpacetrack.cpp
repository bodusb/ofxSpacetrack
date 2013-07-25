#include "ofxSpacetrack.h"

ofxSpacetrack::ofxSpacetrack(){
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
    //getgravconst(wgs72, tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2 );

	// OPENS AND READ FILETLE
	ofBuffer file = ofBufferFromFile(this->fileTLE);

	string firstLine,secondLine;

	do{
		string line = file.getNextLine();
		if( line.front() == '1') firstLine = line;
		if( line.front() == '2') secondLine = line;
	}while(  !file.isLastLine());
	// do just one sat -- later fix to set a vector of sats

	//char fl[130] = firstLine;
	//adapts string to char[130]
	vector<char> fl(firstLine.size() +1);
	copy(firstLine.begin(),firstLine.end(),fl.begin());
	vector<char> sl(secondLine.size() +1);
	copy(secondLine.begin(),secondLine.end(),sl.begin());

	twoline2rv(&*fl.begin(),&*sl.begin(),'m','m',wgs72,this->startMFE,this->stopMFE,this->propagationStep,satrec);

	return true;
}
//-----------------------------------------------------------
bool ofxSpacetrack::startPropagatorNow(){
	double jdstart;
    jday( ofGetYear(),ofGetMonth(),ofGetDay(),ofGetHours(),ofGetMinutes(),ofGetSeconds(), jdstart );
	this->startMFE = (jdstart - satrec.jdsatepoch) * 1440.0;
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

