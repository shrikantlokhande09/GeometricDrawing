#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    int centerX = ofGetWidth() / 2;
    int centerY = ofGetHeight() / 2;
    
    outerCircle.ptCenter.x = centerX;
    outerCircle.ptCenter.y = centerY;
    
    innerCircle.ptCenter.x = centerX;
    innerCircle.ptCenter.y = centerY;
    
    outerCircle.radius = 300;
    innerCircle.radius = 200;
    
    ofSetBackgroundColor(255, 255, 255);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofNoFill();
    ofSetLineWidth(3);
    ofSetCircleResolution(100);
    ofSetColor(255, 0, 0);
    ofDrawCircle(outerCircle.ptCenter, outerCircle.radius);
    ofDrawCircle(innerCircle.ptCenter, innerCircle.radius);
    
    for(int i=0; i<lines.size(); i++)
    {
        ofSetColor(100, 100, 100);
        ofNoFill();
        ofDrawLine(lines[i].ptStart, lines[i].ptEnd);
        
        ofSetColor(0, 255, 0);
        ofFill();
        
        ofDrawCircle(lines[i].innerIntersection.pt, 5);
        ofDrawCircle(lines[i].outerIntersection.pt, 5);
    }

    for(int i=0; i<polygons.size(); i++)
    {
        ofSetColor(0, 255, 255);
        ofSetLineWidth(5);
        
        for(int j=0; j<(polygons[i].points.size()-1); j++)
        {
            ofDrawLine(polygons[i].points[j], polygons[i].points[j+1]);
        }
    }
    
    ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    clickStart.x = x;
    clickStart.y = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    clickEnd.x = x;
    clickEnd.y = y;
    
    if(getDistance(clickStart, clickEnd) > LINE_CREATION_THRESHOLD)
    {
        slLine line = slLine(); line.ptStart = clickStart; line.ptEnd = clickEnd;
        
        checkLineIntersectingBothCircles(line);
    }
}

float ofApp::getDistance(ofPoint pt1, ofPoint pt2)
{
    float distance = sqrt(pow((pt2.x - pt1.x), 2.0) + pow((pt2.y - pt1.y), 2.0));
    return distance;
}

bool ofApp::checkLineIntersectingBothCircles(slLine line)
{
    float dx, dy, A, B, C, det, t;
    bool withOuterCircle = false, withInnerCircle = false;
    
     /// Outer Circle
    ofPoint ptOuter;
    
    dx = line.ptEnd.x - line.ptStart.x;
    dy = line.ptEnd.y - line.ptStart.y;
    
    A = (dx * dx) + (dy * dy);
    B = 2 * (dx * (line.ptStart.x - outerCircle.ptCenter.x) + dy * (line.ptStart.y - outerCircle.ptCenter.y));
    C = (line.ptStart.x - outerCircle.ptCenter.x) * (line.ptStart.x - outerCircle.ptCenter.x) +
    (line.ptStart.y - outerCircle.ptCenter.y) * (line.ptStart.y - outerCircle.ptCenter.y) - (outerCircle.radius * outerCircle.radius);
    
    det = (B * B) - (4 * (A * C));
    
    if ((A <= 0.0000001) || (det < 0))
    {
        //printf("No Interaction");
        withOuterCircle = false;
    }
    else if (det == 0)
    {
        //printf("one Interaction");
        
        // One solution.
        t = -B / (2 * A);
        ofPoint pt;
        pt.x = line.ptStart.x + t * dx;
        pt.y = line.ptStart.y + t * dy;
        
        if(checkPointIsOnLine(line, pt))
        {
            withOuterCircle = true;
            ptOuter = pt;
        }
    }
    else
    {
        //printf("two Interaction");
        // Two solutions.
        
        t = (float)((-B + sqrt(det)) / (2 * A));
        ofPoint pt1;
        pt1.x = line.ptStart.x + t * dx;
        pt1.y = line.ptStart.y + t * dy;
        
        t = (float)((-B - sqrt(det)) / (2 * A));
        ofPoint pt2;
        pt2.x = line.ptStart.x + t * dx;
        pt2.y = line.ptStart.y + t * dy;
        
        if(checkPointIsOnLine(line, pt1) && checkPointIsOnLine(line, pt2))
        {
            withOuterCircle = false;
        }
        else if(checkPointIsOnLine(line, pt1))
        {
            withOuterCircle = true;
             ptOuter = pt1;
        }
        else if(checkPointIsOnLine(line, pt2))
        {
            withOuterCircle = true;
             ptOuter = pt2;
        }
    }
    
    /// Inner Circle
    ofPoint ptInner;
    
    dx = line.ptEnd.x - line.ptStart.x;
    dy = line.ptEnd.y - line.ptStart.y;
    
    A = (dx * dx) + (dy * dy);
    B = 2 * (dx * (line.ptStart.x - innerCircle.ptCenter.x) + dy * (line.ptStart.y - innerCircle.ptCenter.y));
    C = (line.ptStart.x - innerCircle.ptCenter.x) * (line.ptStart.x - innerCircle.ptCenter.x) +
    (line.ptStart.y - innerCircle.ptCenter.y) * (line.ptStart.y - innerCircle.ptCenter.y) - (innerCircle.radius * innerCircle.radius);
    
    det = (B * B) - (4 * (A * C));
    
    if ((A <= 0.0000001) || (det < 0))
    {
        //printf("No Interaction");
        withInnerCircle = false;
    }
    else if (det == 0)
    {
        //printf("one Interaction");
        
        // One solution.
        t = -B / (2 * A);
        ofPoint pt;
        pt.x = line.ptStart.x + t * dx;
        pt.y = line.ptStart.y + t * dy;
        
        if(checkPointIsOnLine(line, pt))
        {
            withInnerCircle = true;
             ptInner = pt;
        }
    }
    else
    {
        //printf("two Interaction");
        // Two solutions.
        
        t = (float)((-B + sqrt(det)) / (2 * A));
        ofPoint pt1;
        pt1.x = line.ptStart.x + t * dx;
        pt1.y = line.ptStart.y + t * dy;
        
        t = (float)((-B - sqrt(det)) / (2 * A));
        ofPoint pt2;
        pt2.x = line.ptStart.x + t * dx;
        pt2.y = line.ptStart.y + t * dy;
        
        if(checkPointIsOnLine(line, pt1) && checkPointIsOnLine(line, pt2))
        {
            withInnerCircle = false;
        }
        else if(checkPointIsOnLine(line, pt1))
        {
            withInnerCircle = true;
             ptInner = pt1;
        }
        else if(checkPointIsOnLine(line, pt2))
        {
            withInnerCircle = true;
             ptInner = pt2;
        }
    }
    
    if(withInnerCircle && withOuterCircle)
    {
        //intersectionPoints.push_back(ptInner);
        //intersectionPoints.push_back(ptOuter);
        
        float angle = (atan2(ptInner.y - innerCircle.ptCenter.y, ptInner.x - innerCircle.ptCenter.x))*(57.29);
        
        line.innerIntersection.pt = ptInner;
        line.innerIntersection.angle = angle;
        
        angle = (atan2(ptOuter.y - outerCircle.ptCenter.y, ptOuter.x - outerCircle.ptCenter.x))*(57.29);
        
        line.outerIntersection.pt = ptOuter;
        line.outerIntersection.angle = angle;
        
        //printf("inner angles = %f, outer angle %f\n", line.innerIntersection.angle, line.outerIntersection.angle);
        lines.push_back(line);
        checkAndCalculatePolgons();
    }
    
    return (withInnerCircle && withOuterCircle);
}

void ofApp::checkAndCalculatePolgons()
{
    vector<ofPoint> iPoly;
    vector<ofPoint> oPoly;
    
    if(lines.size() >= 2)
    {
        int min = -180;
        for(int j=min; j<180; j++)
        {
            for(int i=0; i<lines.size(); i++)
            {
                if(((int)lines[i].innerIntersection.angle) == j)
                {
                    min = j;
                    iPoly.push_back(lines[i].innerIntersection.pt);
                }
            }
        }
        
        min = -180;
        for(int j=min; j<180; j++)
        {
            for(int i=0; i<lines.size(); i++)
            {
                if(((int)lines[i].outerIntersection.angle) == j)
                {
                    min = j;
                    oPoly.push_back(lines[i].outerIntersection.pt);
                }
            }
        }
        
        polygons.clear();
        
        for(int i=0; i<(lines.size()-1);i++)
        {
            slPoly poly;
            poly.points.push_back(oPoly[i]);
            poly.points.push_back(oPoly[i+1]);
            poly.points.push_back(iPoly[i+1]);
            poly.points.push_back(iPoly[i]);
            polygons.push_back(poly);
        }
    }
}

bool ofApp::checkPointIsOnLine(slLine line, ofPoint pt)
{
    float lineLength = getDistance(line.ptStart, line.ptEnd);
    float dist1 = getDistance(line.ptStart, pt);
    float dist2 = getDistance(line.ptEnd, pt);
    
    if(abs((dist1 + dist2) - lineLength) < 5.0)
    {
        return true;
    }
    
    return false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
