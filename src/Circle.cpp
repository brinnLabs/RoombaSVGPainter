// Circle.cpp: implementation of the Circle class.
//
//////////////////////////////////////////////////////////////////////

#include "Circle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Circle::Circle()
{
	this->radius = -1;		// error checking 
}

Circle::~Circle()
{

}

Circle::Circle(ofPoint pt1, ofPoint pt2, ofPoint pt3)
{
	this->radius = -1;		// error checking 

	if (!this->IsPerpendicular(pt1, pt2, pt3))				this->CalcCircle(pt1, pt2, pt3);
	else if (!this->IsPerpendicular(pt1, pt3, pt2))		this->CalcCircle(pt1, pt3, pt2);
	else if (!this->IsPerpendicular(pt2, pt1, pt3))		this->CalcCircle(pt2, pt1, pt3);
	else if (!this->IsPerpendicular(pt2, pt3, pt1))		this->CalcCircle(pt2, pt3, pt1);
	else if (!this->IsPerpendicular(pt3, pt2, pt1))		this->CalcCircle(pt3, pt2, pt1);
	else if (!this->IsPerpendicular(pt3, pt1, pt2))		this->CalcCircle(pt3, pt1, pt2);
	else {
		ofLogError("\nThe three pts are perpendicular to axis\n");
		this->radius = -1;
		return;
	}

}

bool Circle::IsPerpendicular(ofPoint pt1, ofPoint pt2, ofPoint pt3)
// Check the given point are perpendicular to x or y axis 
{
	double yDelta_a = pt2.y - pt1.y;
	double xDelta_a = pt2.x - pt1.x;
	double yDelta_b = pt3.y - pt2.y;
	double xDelta_b = pt3.x - pt2.x;


	//	ofLogError(" yDelta_a: %f xDelta_a: %f",yDelta_a,xDelta_a);
	//	ofLogError(" yDelta_b: %f xDelta_b: %f",yDelta_b,xDelta_b);

	// checking whether the line of the two pts are vertical
	if (fabs(xDelta_a) <= 0.000000001 && fabs(yDelta_b) <= 0.000000001) {
		ofLogError("The points are pependicular and parallel to x-y axis\n");
		return false;
	}

	if (fabs(yDelta_a) <= 0.0000001) {
		//		ofLogError(" A line of two point are perpendicular to x-axis 1\n");
		return true;
	}
	else if (fabs(yDelta_b) <= 0.0000001) {
		//		ofLogError(" A line of two point are perpendicular to x-axis 2\n");
		return true;
	}
	else if (fabs(xDelta_a) <= 0.000000001) {
		//		ofLogError(" A line of two point are perpendicular to y-axis 1\n");
		return true;
	}
	else if (fabs(xDelta_b) <= 0.000000001) {
		//		ofLogError(" A line of two point are perpendicular to y-axis 2\n");
		return true;
	}
	else return false;
}

double Circle::CalcCircle(ofPoint pt1, ofPoint pt2, ofPoint pt3)
{
	double yDelta_a = pt2.y - pt1.y;
	double xDelta_a = pt2.x - pt1.x;
	double yDelta_b = pt3.y - pt2.y;
	double xDelta_b = pt3.x - pt2.x;

	if (fabs(xDelta_a) <= 0.000000001 && fabs(yDelta_b) <= 0.000000001) {
		ofLogError("Calc cirlce");
		this->center.x = 0.5*(pt2.x + pt3.x);
		this->center.y = 0.5*(pt1.y + pt2.y);
		this->center.z = pt1.z;
		this->radius = center.distance(pt1);		// calc. radius
														//		ofLogError(" Center: %f %f %f\n", m_Center.x, m_Center.y(), m_Center.z());
														//		ofLogError(" radius: %f %f %f\n", length(&m_Center,pt1), length(&m_Center,pt2),length(&m_Center,pt3));

		return this->radius;
	}

	// IsPerpendicular() assure that xDelta(s) are not zero
	double aSlope = yDelta_a / xDelta_a; // 
	double bSlope = yDelta_b / xDelta_b;
	if (fabs(aSlope - bSlope) <= 0.000000001) {	// checking whether the given points are colinear. 	
		ofLogError("The three pts are colinear\n");
		return -1;
	}

	// calc center
	this->center.x = (aSlope*bSlope*(pt1.y - pt3.y) + bSlope*(pt1.x + pt2.x)
		- aSlope*(pt2.x + pt3.x)) / (2 * (bSlope - aSlope));
	this->center.y = -1 * (center.x - (pt1.x + pt2.x) / 2) / aSlope + (pt1.y + pt2.y) / 2;
	this->center.z = pt1.z;

	this->radius = center.distance(pt1);		// calc. radius
													//	ofLogError(" Center: %f %f %f\n", m_Center.x, m_Center.y(), m_Center.z());
													//	ofLogError(" radius: %f %f %f\n", length(&m_Center,pt1), length(&m_Center,pt2),length(&m_Center,pt3));
	return this->radius;
}

ofPoint Circle::GetCenter()
{
	return center;

}

double Circle::GetRadius()
{
	return this->radius;
}