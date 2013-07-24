#include "ofSpacetrack.h"

ofxSpacetrack::ofxSpacetrack(){
}

ofxSpacetrack::~ofxSpacetrack(){
}

// Convert from YMD to Julian date
double ofxSpacetrack::convertYMD2J2000(YMD *value){
    double j2000;
    //call function
    jday(value.year,value.mon,value.minute,value.day,value.hr,value.minute,value.sec,&j2000)
    return j2000;
}

// Convert from Julian to YMD date
YMD ofxSpacetrack::convertJ2000toYMD(double value){
    YMD ymd;
    invjday(value,&ymd.year,&ymd.mon,&ymd.day,&ymd.hr,&ymd.minute,&ymd.sec)
    return ymd;
}

// Process the TLE
bool ofxSpacetrack::processTLE(){

    if(this->fileTLE != NULL){

    }


}

// Verify if the TLE read is valid
bool ofxSpacetrack::validTLE(){

}
