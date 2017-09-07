#include "ofMain.h"

class Robot
{
public:
	Robot();
	void update();
	void draw();

	void turn(float angle) {
		heading.rotate(angle);
	}

	void setSpeed(float speed) {
		this->speed = speed;
	}

	void setFacingPosition(ofPoint target) {
		float angle = heading.angle(target-pos);
		//if (target.x < pos.x || target.y < pos.y) { angle += 180; }
		turn(angle);
	}

	void setFacing(float angle) {
		ofVec3f newHeading = ofVec3f(0, 1);
		newHeading.rotate(angle, ofVec3f(0, 0, 1));
		heading = newHeading;
	}

	void setRadius(float radius) {
		this->radius = radius;
	}

	float getSpeed() {
		return speed;
	}

	float getFacingAngle() {
		return ofVec2f(0, 1).angle(heading);
	}

	void setPosition(ofPoint pos) {
		this->pos = pos;
	}

	ofPoint getPosition() { return pos; }

	bool isPenDown() {
		return penDown;
	}

	void setPenDown(bool down) {
		penDown = down;
	}

	void addPathPoint(bool shouldDraw, ofPoint dest) {
		path.emplace(shouldDraw, dest);
	}

	void clearPath() {
		queue<pair<bool, ofPoint>> emptyPath;
		path.swap(emptyPath);
	}

	void executePath() {
		if (!path.empty()) {
			target = pos;
			speed = .4;
			execute = true;
			distTraveled = 0;
		}
	}

	void setTolerance(float tolerance) {
		this->tolerance = tolerance;
	}

	float getTolerance() {
		return tolerance;
	}

	float getDistanceTraveled() {
		return distTraveled;
	}

private:
	ofVec2f heading;
	ofPoint pos, target;
	float radius, speed, tolerance, distTraveled;
	bool penDown, execute;

	queue<pair<bool, ofPoint>> path;

};