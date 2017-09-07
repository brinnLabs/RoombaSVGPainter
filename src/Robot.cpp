#include "Robot.h"


//void getMagicBezierPath(Point2D ball, Point2D robot, Point2D goal, Point2D robotDirection, Point2D *robotControl, Point2D *ballEntryRobotControl, Point2D *ballEntry, Point2D *ballEntryBallControl, Point2D *ballControl) {
//	float magic_number = 0.390524292f; /* 2*(2-sqrt(2))/3 */     float robotBallGoalAngle = angle(direction(robot, ball), direction(ball, goal));
//
//	Point2D goalBall = direction(goal, ball);
//
//	Point2D ballEntryOrigin = translate(ball, scale(goalBall, normalizedSigmoid((0.5f + normalizedSigmoid(fabs(robotBallGoalAngle) / M_PI)) * sigmoid(distance(ball, robot), 0.2f, 0.4f)) * 0.1f));     Point2D ballEntryDirection = rotate(goalBall, robotBallGoalAngle / 2);     Point2D ballEntryOffset = scale(ballEntryDirection, normalizedSigmoid((0.5f + normalizedSigmoid(fabs(robotBallGoalAngle) / M_PI)) * sigmoid(distance(ball, robot), 0.2f, 0.4f)) * 0.1f);       *ballEntry = translate(ballEntryOrigin, ballEntryOffset);
//
//	*ballEntryRobotControl = translate(*ballEntry, scale(ballEntryDirection, magic_number * distance(*ballEntry, robot)));     *ballEntryBallControl = translate(*ballEntry,
//
//		scale(ballEntryDirection, -magic_number * distance(*ballEntry, ball)));
//
//	*ballControl = translate(ball, scale(direction(goal, ball), magic_number * distance(*ballEntry, ball)));     *robotControl = translate(robot, scale(direction(robot, *ballEntry), magic_number * distance(robot, *ballEntry)));
//}
//
//SpeedType getBezierSpeeds(PositionType origin, PositionType destination) {
//	const float magic_number = 0.390524292; /* 2*(2-sqrt(2))/3 */
//
//	SpeedType speed;
//
//	float direction;  float dx;  float dy;  float dt;  float l;  float theta;  float d;  float k;
//
//	direction = (destinationType & DST_REVERSE ? -1.0f : 1.0f);  dx = destination.x - origin.x;  dy = destination.y - origin.y;  dt = destination.phi - origin.phi;  l = distancept(origin, destination);  theta = atan2f(dy, dx) - origin.phi;    if (dt > M_PI) dt -= 2 * M_PI;  if (dt <-M_PI) dt += 2 * M_PI;
//
//	if (theta > M_PI) theta -= 2 * M_PI;  if (theta <-M_PI) theta += 2 * M_PI;
//
//	d = direction * magic_number * l;  k = (l*sinf(theta) - d * sinf(dt)) / (d*d);  speed.v = direction * speedLimit;  speed.w = k * speed.v;
//
//
//	if (fabs(speed.w) > angularSpeedLimit) {     /* Rotate on the spot */   speed.w = angularSpeedLimit * (speed.w > 0 ? 1 : -1);   speed.v = 0; }
//		return speed;
//}

Robot::Robot()
{
	heading = ofVec2f(0, 1);
	pos = ofPoint(0, 0);
	radius = 0;
	speed = 0;
	penDown = false;
	tolerance = 3.5;
}

void Robot::update()
{
	if (execute) {
		if (pos.squareDistance(target) < tolerance) {
			if (!path.empty()) {
					target = path.front().second;
					penDown = path.front().first;
					setFacingPosition(target);
					path.pop();
			} else {
				execute = false;
				speed = 0;
				penDown = false;
			}
		} else {
			ofLogNotice(ofToString(pos.squareDistance(target)) + ", " + ofToString(pos.angle(target)));
		}
	}
	pos.x += heading.x*speed;
	pos.y += heading.y*speed;
	distTraveled += abs(heading.x*speed) + abs(heading.y*speed);
}

void Robot::draw()
{
	ofPushStyle();
	ofFill();
	ofPushMatrix(); {
		ofTranslate(pos);
		ofRotateZ(getFacingAngle());
		ofSetColor(ofColor::lawnGreen);
		ofCircle(0, 0, 11);
		ofSetColor(ofColor::black);
		ofTriangle(ofPoint(0, 5), ofPoint(3, 0), ofPoint(-3, 0));
	} ofPopMatrix();
	ofPopStyle();
}
