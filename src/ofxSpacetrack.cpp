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
    double tumin; // minutes in one time unit
    double mu; //earth gravitational parameter
    double radiusearthkm; //radius of the earth in km
    double xke; //reciprocal of tumin
    double j2;
    double j3;
    double j4;
    double j3oj2; // j3/j2
    double ro[3]; //position vector
    double vo[3]; //velocity vector
    double startmfe, stopmfe, deltamin;
    File infile;

    getgravconst( "wgs72", tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2 );

    //input 2-line element set file
    infile = fopen(this->fileTLE, "r");
    if (infile == NULL){
        cout<< "Failed to open file"<<endl;
        return 1;
    }

    while (feof(infile) == 0)
    {
        do
        {
            fgets( longstr1,130,infile);
            strncpy(str, &longstr1[0], 1);
            str[1] = '\0';
        }while ((strcmp(str, "#")==0)&&(feof(infile) == 0));

        if (feof(infile) == 0)
        {
            fgets( longstr2,130,infile);
            twoline2rv( longstr1, longstr2, "m", "m", "wgs72", startmfe, stopmfe, deltamin, satrec );
            sgp4("wgs72", satrec, 0.0, ro, vo); //function of propagation
        }
    }

}

// Verify if the TLE read is valid
bool ofxSpacetrack::validTLE(){

}
