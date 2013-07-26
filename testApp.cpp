#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){


	ofBackground(ofColor::black);

	cam.setFarClip(100000);
	cam.setDistance(20000);

	////old OF default is 96 - but this results in fonts looking larger than in other programs.
	ofTrueTypeFont::setGlobalDpi(72);

	verdana30.loadFont("verdana.ttf", 20, true, true);
	verdana30.setLineHeight(34.0f);
	verdana30.setLetterSpacing(1.035);

	st = new ofxSpacetrack();

	st->setFileTLE("SCD1_2LINE.DAT");
	st->processTLE();


    st->startPropagatorNow();
	//cout<<"SETUP OK";
}

//--------------------------------------------------------------
void testApp::update(){

    //Update the SpaceTrack propagator
	st->update();


}

//--------------------------------------------------------------
void testApp::draw(){
//MODEL

    glEnable(GL_DEPTH_TEST);

	cam.begin();

		//DRAW Satellite
		ofPushMatrix();
			ofSetColor(ofColor::red);
			ofSphere(st->getCurrentPoint(),200);
		ofPopMatrix();

		//DRAW EARTH
		//ofNoFill();
		ofDrawAxis(8000);
		ofSetColor(ofColor::blue);
		ofSphere(6378);

    cam.end();

//




	//INTEFACE
	ofSetColor(225);
	//Testing interface
	verdana30.drawString("SATELLITE INFORMATION",10,20);									//SATELLITE INFORMATION

	//verdana30.drawString(	"POSX:" + ofToString(st.getCurrentPosition().x) +
	//						" - POSY:" + ofToString(st.getCurrentPosition().y) +
	//						" - POSZ:" + ofToString(st.getCurrentPosition().z),10,60);		//POS X
	//verdana30.drawString(	"VELX:" + ofToString(st.getCurrentVelocity().x) +
	//						" - VELY:" + ofToString(st.getCurrentVelocity().y) +
	//						" - VELZ:" + ofToString(st.getCurrentVelocity().z),10,80);		//VEL X

	verdana30.drawString(	"POSX: " + ofToString(st->getCurrentPoint()) ,10,60);		//POS X
	verdana30.drawString(	"VELX: " + ofToString(st->getCurrentVelocity()) ,10,80);		//VEL X

	double alt = st->getCurrentPoint().length() - 6378;
	cout<< alt;
	double vel = st->getCurrentVelocity().length();

	verdana30.drawString(	"LAT: " + ofToString(1) +
							" - LONG: " + ofToString(2) +
							" - ALT: " + ofToString(alt) +
							" - SuperV: " + ofToString(vel)	,10,100);								//ECI

	verdana30.drawString("DATE: " ,10,120);					//DATE
	verdana30.drawString("JULIAN: " + ofToString(st->getCurrentJ2000()),10,140);				//JULIAN
	verdana30.drawString("MFE: " + ofToString(st->getCurrentMFE()),10,160);					//MFE

	verdana30.drawString("FPS: " + ofToString(ofGetFrameRate()),10,180);

	verdana30.drawString("PC Time: "  + ofToString(ofGetYear()) + " " + ofToString(ofGetMonth())   +  " " + ofToString(ofGetDay())
		  +  " " + ofToString(ofGetHours())   +  " " + ofToString(ofGetMinutes())   +  " " + ofToString(ofGetSeconds()),10,200);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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
