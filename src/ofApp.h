#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxShivaVGRenderer.h"
#include "Circle.h"
#include "Robot.h"

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

		void sendPathToRobot(deque<ofPolyline> paths);
		
		ofxSVG svg, mutableSvg;

		float strokeWidth, distanceMod;

		vector<ofPoint> points;
		ofPoint homePosition;

		bool debug, d_verticies, b_path;
		Robot robot;

		ofPtr<ofxShivaVGRenderer> shivaRenderer;

		ofFbo robotPathFbo;
};
