#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){


	//Start-up propagator
	st = new ofxSpacetrack();
	st->setFileTLE("SCD1_2LINE.DAT");
	st->processTLE();
    st->startPropagatorNow();

	//Start-up camera
	ofBackground(ofColor::black);
	cam.setFarClip(100000); 
	cam.setDistance(20000);

	// Visual interface
	ofTrueTypeFont::setGlobalDpi(72);
	verdana30.loadFont("verdana.ttf", 20, true, true);
	verdana30.setLineHeight(34.0f);
	verdana30.setLetterSpacing(1.035);


	glEnable(GL_DEPTH_TEST);


    glShadeModel(GL_SMOOTH); //some model / light stuff
	//light.enable();
    ofEnableSeparateSpecularLight();

	//
	City newyork = { "new york", 40+47/60., -73 + 58/60. };
	City tokyo = { "tokyo", 35 + 40./60, 139 + 45/60. };
	City london = { "london", 51 + 32/60., -5./60. };
	City shanghai = { "shanghai", 31 + 10/60., 121 + 28/60. };
	City buenosaires = { "buenos aires", -34 + 35/60., -58 + 22/60. };
	City melbourne = { "melbourne" , -37 + 47/60., 144 + 58/60. };	
	City detroit = { "detroit", 42 + 19/60., -83 + 2 / 60. };
	City sjc = { "sjc", -23 + 11/60, -45 + 42/60};

	cities.push_back( newyork );
	cities.push_back( tokyo );
	cities.push_back( london );
	cities.push_back( shanghai );
	cities.push_back( buenosaires );
	cities.push_back( melbourne );
	cities.push_back( detroit );
	cities.push_back( sjc );

	cout<<endl<<"SETUP OK";
}

//--------------------------------------------------------------
void testApp::update(){

	//Update the SpaceTrack propagator
	st->update();
}

//--------------------------------------------------------------
void testApp::draw(){


	cam.begin();

	ofScale(1,-1,1);

	
	//DRAW Satellite
	//Draw by lat/long
		ofPushMatrix();
			ofSetColor(ofColor::yellow);
			ofQuaternion latRot, longRot, spinQuat;
			latRot.makeRotate(st->getLatitude(), 1, 0, 0);
			longRot.makeRotate(st->getLongitude(), 0, 1, 0);
			//spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);
			ofVec3f center = ofVec3f(0,0,st->getAltitude() + 6378.135);
			ofVec3f worldPoint = latRot * longRot * spinQuat * center;
			ofSphere(worldPoint,200);
		ofPopMatrix();


	ofPushMatrix();
	ofSetColor(255);	
	for(unsigned int i = 0; i < cities.size(); i++){
		
		//three rotations
		//two to represent the latitude and lontitude of the city
		//a third so that it spins along with the spinning sphere 
		ofQuaternion latRot, longRot, spinQuat;
		latRot.makeRotate(cities[i].latitude, 1, 0, 0);
		longRot.makeRotate(cities[i].longitude, 0, 1, 0);
		//spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);
		
		//our starting point is 0,0, on the surface of our sphere, this is where the meridian and equator meet
		ofVec3f center = ofVec3f(0,0,6378.135);
		//multiplying a quat with another quat combines their rotations into one quat
		//multiplying a quat to a vector applies the quat's rotation to that vector
		//so to to generate our point on the sphere, multiply all of our quaternions together then multiple the centery by the combined rotation
		ofVec3f worldPoint = latRot * longRot * spinQuat * center;
		
		//draw it and label it
		ofLine(ofVec3f(0,0,0), worldPoint);

		//set the bitmap text mode billboard so the points show up correctly in 3d
		ofDrawBitmapString(cities[i].name, worldPoint );
	}
	ofPopMatrix();



	//DRAW EARTH
		//ofNoFill();
		ofPushMatrix();
			ofDrawAxis(8000);
			ofSetColor(ofColor::blue);
			ofNoFill();
			ofSphere(6378);		//Earth Radius
		ofPopMatrix();
						

    cam.end();




	//INTEFACE
	ofSetColor(225);
	//Testing interface
	verdana30.drawString("SATELLITE INFORMATION",10,20);									//SATELLITE INFORMATION
	verdana30.drawString(	"POSX: " + ofToString(st->getCurrentPoint()) ,10,60);		//POS X
	verdana30.drawString(	"VELX: " + ofToString(st->getCurrentVelocity()) ,10,80);		//VEL X

	verdana30.drawString(	"LAT: " + ofToString(st->getLatitude()) +
		" - LONG: " + ofToString(st->getLongitude()) +
		" - ALT: " + ofToString(st->getAltitude()),10,100);								//ECI

	verdana30.drawString("DATE: " ,10,120);					//DATE
	verdana30.drawString("JULIAN: " + ofToString(st->getCurrentJ2000()),10,140);				//JULIAN
	verdana30.drawString("MFE: " + ofToString(st->getCurrentMFE()),10,160);					//MFE

	verdana30.drawString("FPS: " + ofToString(ofGetFrameRate()),10,180);

	verdana30.drawString("PC Time: "  + ofToString(ofGetYear()) + " " + ofToString(ofGetMonth())   +  " " + ofToString(ofGetDay())
		  +  " " + ofToString(ofGetHours())   +  " " + ofToString(ofGetMinutes())   +  " " + ofToString(ofGetSeconds()),10,200);

    verdana30.drawString("Time multiplier: "  + ofToString(st->getTimeMultiplier()),10,220);

	verdana30.drawString("Simulation Time: "  +
                      ofToString(st->getCurrentYMD().year) + " " +
                      ofToString(st->getCurrentYMD().mon)   +  " " +
                      ofToString(st->getCurrentYMD().day)  +  " " +
                      ofToString(st->getCurrentYMD().hr)   +  " " +
                      ofToString(st->getCurrentYMD().minute)   +  " " +
                      ofToString(st->getCurrentYMD().sec),10,240);



}

//--------------------------------------------------------------
void testApp::exit(){

	// Free the propagator
	cout<<endl<<"Free propagator:";
	free(st);
	cout<<endl<<"ok";
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    double aux;
    switch(key){
    case OF_KEY_UP:
        st->setTimeMultiplier(st->getTimeMultiplier() + 0.01);
        break;
    case OF_KEY_DOWN:
        st->setTimeMultiplier(st->getTimeMultiplier() - 0.01);
        break;
    default: break;};
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch(key){
    case 's': //toogle simulated time
        if(st->isSimulatedTime()){st->doSimulatedTime(false);}else{st->doSimulatedTime(true);}
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}