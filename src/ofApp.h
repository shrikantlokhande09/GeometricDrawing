#pragma once

#include "ofMain.h"

#define LINE_CREATION_THRESHOLD 50

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    struct slIntersection
    {
        ofPoint pt;
        float angle;
    };
    
    struct slLine
    {
        ofPoint ptStart;
        ofPoint ptEnd;
        slIntersection outerIntersection;
        slIntersection innerIntersection;
    };
    
    struct slCircle{
        ofPoint ptCenter;
        float radius;
    };
    
    struct slPoly
    {
        vector<ofPoint> points;
    };
    
    float getDistance(ofPoint pt1, ofPoint pt2);
    bool checkLineIntersectingBothCircles(slLine line);
    bool checkPointIsOnLine(slLine line, ofPoint pt);
    void checkAndCalculatePolgons();
    
    vector<slLine> lines;
    vector<slPoly> polygons;
    
    slCircle outerCircle, innerCircle;
    
    ofPoint clickStart;
    ofPoint clickEnd;
};
