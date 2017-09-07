#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	/*
	on load we should find the nearest shape and its ideal verticie and start the shape from that. At the end of that path
	evaluate each path measuing distance from end point, total length, and the number of vertices nearby
	the end point.

	we should strip all except paths from the svg these are easily handled... its the paths that are difficult

	*/

	homePosition = ofPoint(30, 30);
	robot.setPosition(homePosition);

	robotPathFbo.allocate(ofGetWidth(), ofGetHeight());
	robotPathFbo.begin();
	ofClear(255);
	robotPathFbo.end();

	svg.load("diamond.svg");
	float scale = MIN(ofGetWidth() / svg.getWidth(), ofGetHeight() / svg.getHeight());
	svg.scale(scale);
	distanceMod = MAX(svg.getWidth(), svg.getHeight()) / 250;
	//svg.simplifySVG(2);

	strokeWidth = 5;

	svg.setSVGFilled(false);
	svg.setSVGStrokeColor(ofColor(0));
	svg.setSVGStrokeWidth(strokeWidth);

	debug = false;
	b_path = true;
	d_verticies = false;


	shivaRenderer = ofPtr<ofxShivaVGRenderer>(new ofxShivaVGRenderer);
	ofSetCurrentRenderer(shivaRenderer);

	shivaRenderer->setLineCapStyle(VG_CAP_SQUARE);
	shivaRenderer->setLineJoinStyle(VG_JOIN_MITER);

	ofBackground(255);
	ofEnableSmoothing();
	ofSetCurveResolution(100);

	ofPoint closestPoint = ofPoint(numeric_limits<float>::max(), numeric_limits<float>::max());
	ofPolyline bestPath;
	deque<ofPolyline> idealPath;
	vector<ofPolyline> remainingLines;
	ofPoint currentLoc = homePosition;

	for (auto tpaths : svg.getPaths()) {
		tpaths.setStrokeWidth(1);
		for (auto line : tpaths.getOutline()) {
			remainingLines.push_back(line);
		}
	}

	//load all the points into a vector
	while (remainingLines.size() > 0) {
		ofPolyline * path2delete;
		for (auto & line : remainingLines) {
			if (line.isClosed()) {
				//we have a shape, any verticie is a valid starting position
				for (auto point : line.getVertices()) {
					points.push_back(point);
					if (currentLoc.distance(point) < currentLoc.distance(closestPoint)) {
						closestPoint = point;
						path2delete = &line;
						if (point != line.getVertices().front()) {
							//are we starting in the middle?
							auto sit = find(line.begin(), line.end(), point);
							ofPolyline newPath = ofPolyline();
							for (auto it = sit; it != line.end(); it++) {
								newPath.addVertex(*it);
							}
							for (auto fit = line.begin(); fit != sit; fit++) {
								newPath.addVertex(*fit);
							}
							newPath.setClosed(true);
							bestPath = newPath;
						}
						else {
							bestPath = ofPolyline(line);
						}

					}
				}
			}
			else {
				//we have a line segment either the front or back are valid locations to start
				if (currentLoc.distance(line.getVertices().front()) < currentLoc.distance(closestPoint)) {
					closestPoint = line.getVertices().front();
					path2delete = &line;
					bestPath = ofPolyline(line);
				}
				if (currentLoc.distance(line.getVertices().back()) < currentLoc.distance(closestPoint)) {
					closestPoint = line.getVertices().back();
					path2delete = &line;
					ofPolyline newPath = ofPolyline(line);
					reverse(newPath.begin(), newPath.end());
					bestPath = ofPolyline(newPath);
				}
			}
		}
		currentLoc = closestPoint;
		idealPath.push_back(bestPath);
		int counter = 0;
		for (int i = 0; i < remainingLines.size(); i++) {
			if (path2delete == &remainingLines[i]) {
				counter = i;
			}
		}
		closestPoint = ofPoint(numeric_limits<float>::max(), numeric_limits<float>::max());
		remainingLines.erase(remainingLines.begin() + counter);
	}

	sendPathToRobot(idealPath);
	/*deque<ofPolyline> testPath;
	for (auto tpaths : svg.getPaths()) {
		tpaths.setStrokeWidth(1);
		for (auto line : tpaths.getOutline()) {
			testPath.push_back(line);
		}
	}
		
	
	sendPathToRobot(testPath);*/

}

//--------------------------------------------------------------
void ofApp::update() {
	robot.update();
	robotPathFbo.begin();
	robot.isPenDown() ? ofSetColor(ofColor::cornflowerBlue) : ofSetColor(ofColor::orangeRed);
	ofCircle(robot.getPosition(), 2);
	robotPathFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(255);
	ofSetColor(0);
	ofDrawBitmapString("Distance Traveled: "+ofToString(robot.getDistanceTraveled()/ distanceMod) + " cm", 5, 15);
	ofSetColor(255);
	ofPushMatrix(); {
		ofTranslate(ofGetWidth() / 2 - svg.getWidth() / 2, 0);
		robotPathFbo.draw(0, 0);

		if (b_path) {
			svg.draw();
		}
		if (d_verticies) {
			for (auto path : svg.getPaths()) {
				if (path.getStrokeWidth() == 0) {
					path.setStrokeWidth(1);
				}
				for (auto poly : path.getOutline()) {
					for (auto point : poly.getVertices()) {
						ofSetColor(ofColor::blue);
						ofCircle(point, 5);
					}
					ofSetColor(ofColor::yellow);
					ofCircle(poly.getVertices().back(), 4);
					ofSetColor(ofColor::red);
					ofCircle(poly.getVertices().front(), 4);
					ofSetColor(0);
					ofDrawBitmapString(ofToString(poly.isClosed()) + " : " + ofToString(poly.getPerimeter()), poly.getCentroid2D());
				}
			}
		}
		if (debug) {
			ofSetColor(0);
			ofPoint startingPoint = robot.getPosition();
			startingPoint.x -= (ofGetWidth() / 2 - svg.getWidth() / 2);
			for (auto point : points) {
				auto distPoint = point;
				point.y -= 2;
				string dist = ofToString(startingPoint.distance(distPoint));
				ofDrawBitmapString(dist, point);
			}
		}
		robot.draw();
	} ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'p':
		b_path = !b_path;
		break;
	case 'v':
		d_verticies = !d_verticies;
		break;
	case 'd':
		debug = !debug;
		break;
	case '1':
		robot.setSpeed(.2);
		break;
	case '2':
		robot.setSpeed(.4);
		break;
	case '3':
		robot.setSpeed(.6);
		break;
	case '4':
		robot.setSpeed(.8);
		break;
	case '5':
		robot.setSpeed(1);
		break;
	case '6':
		robot.setSpeed(1.2);
		break;
	case '7':
		robot.setSpeed(1.4);
		break;
	case '8':
		robot.setSpeed(1.6);
		break;
	case '9':
		robot.setSpeed(1.8);
		break;
	case '0':
		robot.setSpeed(0);
		break;
	case OF_KEY_LEFT:
		robot.turn(15);
		break;
	case OF_KEY_RIGHT:
		robot.turn(-15);
		break;
	case OF_KEY_UP:
		strokeWidth += .25;
		strokeWidth = (strokeWidth >= 20 ? 20 : strokeWidth);
		svg.setSVGStrokeWidth(strokeWidth);
		break;
	case OF_KEY_DOWN:
		strokeWidth -= .25;
		strokeWidth = (strokeWidth <= 0 ? .25 : strokeWidth);
		svg.setSVGStrokeWidth(strokeWidth);
		break;
	case ' ':
		robot.setPenDown(!robot.isPenDown());
		break;
	case OF_KEY_RETURN:
		robot.executePath();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	robotPathFbo.allocate(ofGetWidth(), ofGetHeight());
	robotPathFbo.begin();
	ofClear(255);
	robotPathFbo.end();

	float scale = MIN(ofGetWidth() / svg.getWidth(), ofGetHeight() / svg.getHeight());
	svg.scale(scale);
	//svg.simplifySVG(2);

	//sendPathToRobot();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::sendPathToRobot(deque<ofPolyline> paths)
{
	robot.clearPath();
	for (auto line : paths) {
		robot.addPathPoint(false, line.getVertices().front());
		for (auto point : line.getVertices()) {
			if (point != line.getVertices().front()) {
				robot.addPathPoint(true, point);
			}
		}
		if (line.isClosed()) {
			robot.addPathPoint(true, line.getVertices().front());
		}
	}
	robot.addPathPoint(false, homePosition);
	/*for (auto path : svg.getPaths()) {
		if (path.getStrokeWidth() == 0) {
			path.setStrokeWidth(1);
		}
		for (auto poly : path.getOutline()) {
			robot.addPathPoint(false, poly.getVertices().front());
			for (auto point : poly.getVertices()) {
				if (point != poly.getVertices().front()) {
					robot.addPathPoint(true, point);
				}
			}
		}
	}*/
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
