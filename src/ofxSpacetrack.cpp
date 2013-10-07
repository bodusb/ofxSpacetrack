#include "ofxSpacetrack.h"

ofxSpacetrack::ofxSpacetrack(){

	this->doPropagation = false;
	this->simulatedTime = false;
	this->propagationStep = 0;
	this->timeMultiplier = 0;
	this->fileTLE = "";


	// Setup - astros
	// moon
	moon.name = "Moon";
	moon.radius = 1737.10;
	moon.available = false;
	moonCalc = false;

	// sun
	sun.name = "Sun";
	sun.radius = 695500;
	sun.available = false;
	sunCalc = false;

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



// Process the TLE

bool ofxSpacetrack::processTLE(){


	// TRIES TO READ THE TLE
	if(this->fileTLE == "") return false;

	////SETs initial GRAV constant
	getgravconst(wgs72, this->tumin, this->mu, this->radiusearthkm, this->xke, this->j2, this->j3, this->j4, this->j3oj2 );

	// OPENS AND READ FILETLE
	ofBuffer file = ofBufferFromFile(this->fileTLE);


	// -------------- ON VISUAL STUDIO

	do{
		string line = file.getNextLine();
		//cout<<endl<<line;
		if( line.front() == '1') firstLine = line;
		if( line.front() == '2') secondLine = line;
	}while(  !file.isLastLine());
	// do just one sat -- later fix to set a vector of sats

	// -------------- Using GCC (Linx or Codeblocks/MinGW at Windows)

	//do{
	//	string line = file.getNextLine();

	//       vector<char> l(firstLine.size() +1);
	//       copy(line.begin(),line.end(),l.begin());
	//		cout<<endl<<line;
	//	if( l.front() == '1') firstLine = line;
	//	if( l.front() == '2') secondLine = line;
	//}while(  !file.isLastLine());



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



	this->info.number = this->satrec.satnum;


	if(this->satrec.error != 0) ofLogVerbose() << "Error to start TLE" << this->satrec.error;

	return true;
}
//-----------------------------------------------------------
bool ofxSpacetrack::startPropagatorNow(){
	double jdstart;
	jday( ofGetYear(),ofGetMonth(),ofGetDay(),ofGetHours(),ofGetMinutes(),ofGetSeconds(), jdstart );
	this->startMFE = (jdstart - satrec.jdsatepoch) * 1440.0;

	this->currentMFE = this->startMFE;
	this->doPropagation = true;
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

	if((this->doPropagation == false) && (this->satrec.error==0)) return;

	// Update times
	if(!this->simulatedTime){
		jday( ofGetYear(),ofGetMonth(),ofGetDay(),ofGetHours() + this->timezone,ofGetMinutes(),ofGetSeconds(), this->currentJ2000);	//YMD -> JD
		this->currentMFE = ( this->currentJ2000 - satrec.jdsatepoch)*1440.0; // JD -> MFE
	} else {
		this->currentMFE += timeMultiplier;
		this->currentJ2000 = satrec.jdsatepoch + this->currentMFE/1440.0;                                         //update JD

	}

		invjday(this->currentJ2000, this->currentYMD.year, this->currentYMD.mon, this->currentYMD.day,
			this->currentYMD.hr, this->currentYMD.minute, this->currentYMD.sec);    //update YMD

	//if( (this->currentMFE < this->startMFE  && this->startMFE != 0) || 
	//	(this->currentMFE > this->stopMFE && this->stopMFE != 0) ) {
	//	this->doPropagation = false;
	//	return;
	//}

	//------------------------------------------------------------------
	//                  PROPAGATOR

	sgp4(wgs72, satrec, this->currentMFE, &r[0], &v[0]);
	//------------------------------------------------------------------

	// Update positions


	this->currentPoint.set(r[0],r[1],r[2]);
	this->currentVelocity.set(v[0],v[1],v[2]);

	//update lat,long,alt
	this->altitude = this->getCurrentPoint().length();
	this->TEMEtoLLA(currentPoint,this->currentJ2000,this->latitude,this->longitude,this->altitude);

	ofQuaternion latRot, longRot, spinQuat;
	latRot.makeRotate(getLatitude(), 1, 0, 0);
	longRot.makeRotate(getLongitude(), 0, 1, 0);
	ofVec3f center = ofVec3f(0,0,getAltitude() + 6378.135);
	currentPoint = latRot * longRot * spinQuat * center;

	// get updated orbital
	rv2coe(r,v,this->mu,
		this->info.semilatusRectum,
		this->info.semimajorAxis,
		this->info.eccentricity,
		this->info.inclination,
		this->info.omega,
		this->info.argp,
		this->info.nu,
		this->info.m,
		this->info.arglat,
		this->info.truelon,
		this->info.lonper);


	// update astro positions

	////moon
	//moon.available = false;
	//if(moonCalc){
	//	moon.longitude = CAAMoon::EclipticLongitude(this->currentJ2000);
	//	moon.latitude = CAAMoon::EclipticLatitude(this->currentJ2000);
	//	moon.altitude = CAAMoon::RadiusVector(this->currentJ2000);

	//	CAA2DCoordinate temp = CAACoordinateTransformation::Ecliptic2Equatorial(
	//		CAAMoon::EclipticLongitude(this->currentJ2000) + CAAMoon::TrueLongitudeAscendingNode(this->currentJ2000),
	//		CAAMoon::EclipticLatitude(this->currentJ2000),
	//		CAANutation::TrueObliquityOfEcliptic(this->currentJ2000));

	//	ofLogVerbose() << "Moon ECLIP: " << moon.longitude << " - " << moon.latitude;
	//	ofLogVerbose() << "true moon: " << CAAMoon::TrueLongitudeAscendingNode(this->currentJ2000);
	//	ofLogVerbose() << "Moon EQ: " << temp.X<< " - " << temp.Y;

	//	ofQuaternion latRot, longRot;
	//	latRot.makeRotate(temp.X, 1, 0, 0); // inverse because of the openGL
	//	longRot.makeRotate(temp.Y, 0, 1, 0); // inverse because of the openGL
	//	ofVec3f center = ofVec3f(0,0,moon.altitude);
	//	moon.geoPosition = latRot * longRot * center;

	//	//CAAEllipticalPlanetaryDetails MoonDetails = this->moonRA_D_helper(this->currentJ2000);

	//	//ofLogVerbose() << "Moon";
	//	//ofLogVerbose() << "ApparentGeocentricLongitude : " << MoonDetails.ApparentGeocentricLongitude;
	//	//ofLogVerbose() << "ApparentGeocentricLatitude : " << MoonDetails.ApparentGeocentricLatitude;
	//	//ofLogVerbose() << "ApparentGeocentricDistance : " << MoonDetails.ApparentGeocentricDistance;
	//	//ofLogVerbose() << "ApparentLightTime : " << MoonDetails.ApparentLightTime;
	//	//ofLogVerbose() << "ApparentGeocentricRA : " << MoonDetails.ApparentGeocentricRA;
	//	//ofLogVerbose() << "ApparentGeocentricDeclination : " << MoonDetails.ApparentGeocentricDeclination;


	//	//// adjust to rectangular
	//	//ofQuaternion latRot, longRot;
	//	//latRot.makeRotate(MoonDetails.ApparentGeocentricDeclination, 1, 0, 0);
	//	//longRot.makeRotate(MoonDetails.ApparentGeocentricRA, 0, 1, 0);
	//	//ofVec3f center = ofVec3f(0,0,CAAMoon::RadiusVector(this->currentJ2000));
	//	//moon.geoPosition= latRot * longRot * center;


	//	moon.available = true;
	//}

	//sun
	sun.available = false;
	this->onEclipse = true;
	if(sunCalc){

		CAAEllipticalPlanetaryDetails SunDetails = CAAElliptical::Calculate(this->currentJ2000, CAAElliptical::SUN);

		//ofLogVerbose() << "SUN";
		//ofLogVerbose() << "ApparentGeocentricLongitude : " << SunDetails.ApparentGeocentricLongitude;
		//ofLogVerbose() << "ApparentGeocentricLatitude : " << SunDetails.ApparentGeocentricLatitude;
		//ofLogVerbose() << "ApparentGeocentricDistance : " << SunDetails.ApparentGeocentricDistance;
		//ofLogVerbose() << "ApparentLightTime : " << SunDetails.ApparentLightTime;
		//ofLogVerbose() << "ApparentGeocentricRA : " << SunDetails.ApparentGeocentricRA;
		//ofLogVerbose() << "ApparentGeocentricDeclination : " << SunDetails.ApparentGeocentricDeclination;


				//rotation during the day
		double aux = (this->currentYMD.hr + ((double)this->currentYMD.minute)/60 + (this->currentYMD.sec)/3600 )*360/24 + 180;

		sun.longitude = CAACoordinateTransformation::MapTo0To360Range(SunDetails.ApparentGeocentricRA - aux);
		if( sun.longitude > 180 ){
			sun.longitude -= 360;
		}

		sun.latitude = SunDetails.ApparentGeocentricDeclination;

		// adjust to rectangular
		ofQuaternion latRot, longRot;
		latRot.makeRotate(sun.latitude, 1, 0, 0);
		longRot.makeRotate(sun.longitude, 0, 1, 0);



		//ofLogVerbose() << "Simulation Time: "  +
  //                    ofToString(getCurrentYMD().year) + " " +
  //                    ofToString(getCurrentYMD().mon)   +  " " +
  //                    ofToString(getCurrentYMD().day)  +  " " +
  //                    ofToString(getCurrentYMD().hr)   +  " " +
  //                    ofToString(getCurrentYMD().minute)   +  " " +
  //                    ofToString(getCurrentYMD().sec);

		//ofLogVerbose() << "Current aux: " << aux << " - sunlat: " << sun.latitude << " - sunlong: " << sun.longitude;
		
		ofVec3f center = ofVec3f(0,0,1);
		sun.geoPosition= latRot * longRot * center;

		sun.geoPosition *= dAstronomicalUnit;

		sun.available = true;


		// calc eclipse

		double maxDistanceToEclipse = 2.0*(e_R + altitude)*tan((asin((e_R)/(e_R + altitude)))/2.0);
		//ofLogVerbose() << "Max distance: " << maxDistanceToEclipse;

		ofVec3f ShadowDirection = - getSunPosition().getNormalized();
		ShadowDirection *= (e_R + altitude);

		ShadowDirection -= currentPoint;

		//ofLogVerbose() << "shadVector->sat distance: " << ShadowDirection.length();

		if( ShadowDirection.length() < maxDistanceToEclipse) { this->onEclipse = true;
		} else {
			this->onEclipse = false;
		}


	}





	//ofLogVerbose() << " SAT lat:" << latitude << " long: " << longitude;

}


double			ofxSpacetrack::ThetaG_JD( double v_jd){
	// got from glSat http://glsat.sourceforge.net/

	long double UT_L, TU_L, GMST_L, temp8;

	// cout << "jd_L = " << jd_L << endl;
	UT_L = modf(v_jd + 0.5, &temp8);
	// cout << "UT_L = " << UT_L << endl;
	// UT_L = temp8;
	v_jd = v_jd - UT_L;
	TU_L = (v_jd - 2451545.0)/36525;
	GMST_L = 24110.54841 + TU_L * (8640184.812866 + TU_L * (0.093104 - TU_L * 6.2e-6));
	GMST_L = GMST_L + secondsDay * omega_E * UT_L;

	modf(GMST_L / secondsDay, &temp8);
	temp8 = GMST_L - temp8 * secondsDay;
	if (temp8 < 0.0)
	{
		temp8 += secondsDay;
	}
	return (2.0 * PI * temp8 / secondsDay);


}

void			ofxSpacetrack::TEMEtoLLA(ofPoint r, double v_jd, double &lat, double &lon, double &alt){
	// got from glSat http://glsat.sourceforge.net/

	long double	theta_mine = 0;
	long double	r_mine = 0;
	long double	e2 = 0;
	long double	phi = 0;
	long double	c = 0;

	theta_mine = atan2(r.y, r.x);

	lon = fmod(theta_mine - ThetaG_JD(v_jd), (2.0 * PI));

	r_mine = sqrt((r.x * r.x) + (r.y * r.y));
	e2 = f * (2.0 - f);
	lat = atan2(r.z, r_mine);
	while (fabs(lat - phi) > 1.0e-10)
	{
		phi = lat;
		c = 1.0 / sqrt(1 - e2 * pow(sin(phi), 2));
		lat = atan2(r.z + e_R * c * e2 * sin(phi), r_mine);
	}

	alt = r_mine / cos(lat) - e_R * c;

	if (lon > PI)
	{
		lon -= (2.0 * PI);
	}
	else if (lon < -PI)
	{
		lon += (2.0 * PI);
	}

	lat *= rad2deg;
	lon *= rad2deg;


}


// Ground station
void			ofxSpacetrack::addGroundStation(int v_id, string v_name,double v_lat, double v_long, double v_alt){

	groundStation *aux = new groundStation();

	aux->id = v_id;
	aux->name = v_name;
	aux->latitude = v_lat;
	aux->longitude = v_long;
	aux->altitude = v_alt;

	ofQuaternion latRot, longRot;
	latRot.makeRotate(aux->latitude, 1, 0, 0);
	longRot.makeRotate(aux->longitude, 0, 1, 0);
	ofVec3f center = ofVec3f(0,0,aux->altitude + e_R);
	
	aux->geoPosition = latRot * longRot *  center;

	this->gsList.push_back(aux);

}

groundStation*	ofxSpacetrack::getGroundStation(string v_name){

	vector<groundStation*>::iterator itGS;

	for(itGS = this->gsList.begin(); itGS != this->gsList.end(); itGS++){
		if( (*itGS)->name == v_name) return (*itGS);
	}
	return NULL;
}

groundStation*	ofxSpacetrack::getGroundStation(int v_id){

	vector<groundStation*>::iterator itGS;

	for(itGS = this->gsList.begin(); itGS != this->gsList.end(); itGS++){
		if( (*itGS)->id == v_id) return (*itGS);
	}
	return NULL;
}

void	ofxSpacetrack::setGroundStationStatus(string v_name,bool v_state){
	groundStation *aux = this->getGroundStation(v_name);
	if(aux != NULL) aux->enabled = v_state;
}

void	ofxSpacetrack::setGroundStationStatus(int v_id,bool v_state){
	groundStation *aux = this->getGroundStation(v_id);
	if(aux != NULL) aux->enabled = v_state;
}