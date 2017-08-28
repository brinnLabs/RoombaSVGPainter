#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255);
	svg.load("diamond1.svg");

	currentLoc = ofPoint(0, 0);

	tuple<ofPath, ofPolyline, ofPoint> startPath = make_tuple(ofPath(), ofPolyline(), ofPoint(1000000, 1000000));

	//load all the points into a vector
	for (auto path : svg.getPaths()) {
		path.setStrokeWidth(1);
		for (auto line : path.getOutline()) {
			for (auto point : line.getVertices()) {
				points.push_back(point);
				if (currentLoc.distance(point) < currentLoc.distance(get<2>(startPath))) {
					startPath = make_tuple(path, line, point);
				}
			}
		}
	}

	
	ofLogNotice(ofToString(currentLoc.distance(get<2>(startPath))));

	for (auto point : points) {
		ofLogNotice(ofToString(currentLoc.distance(point)));
	}

	/*
	on load we should find the nearest shape and its ideal verticie and start the shape from that. At the end of that path
	evaluate each path measuing distance from end point, total length, and the number of vertices nearby
	the end point. 
	*/
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	ofPushMatrix(); {
		float scale = MIN(ofGetWidth() / svg.getWidth(), ofGetHeight() / svg.getHeight());
		ofScale(scale, scale);
		float counter = 0;
		for (auto path : svg.getPaths()) {
			path.setFilled(false);
			path.setStrokeColor(ofColor(0));
			path.setStrokeWidth(1);
			path.draw();
			/*for (auto poly : path.getOutline()) {
				for (auto point : poly.getVertices()) {
					ofSetColor(ofColor::blue);
					ofCircle(point, .75);
				}
				ofSetColor(ofColor::yellow);
				ofCircle(poly.getVertices().back(), .5);
				ofSetColor(ofColor::red);
				ofCircle(poly.getVertices().front(), .5);
				ofSetColor(0);
				ofDrawBitmapString(ofToString(poly.isClosed()) + " : " + ofToString(poly.getPerimeter()), poly.getCentroid2D());
			}*/
			counter++;
		}
		ofSetColor(0);
		ofPoint startingPoint = currentLoc;
		startingPoint.y -= 1;
		for (auto point : points) {
			auto distPoint = point;
			distPoint *= scale;
			point.y -= 1;
			string dist = ofToString(startingPoint.distance(distPoint));
			ofDrawBitmapString(dist, point);
		}
	} ofPopMatrix();

	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	currentLoc = ofPoint(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
