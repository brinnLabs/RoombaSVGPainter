// Circle.h: interface for the Circle class.
// Circle class.
// Purpose : Represent the circle object
// Input : 3 different points
// Process : Calcuate the radius and center
// Output : Circle
//           
// This class originally designed for representation of discretized curvature information 
// of sequential pointlist  
// KJIST CAD/CAM     Ryu, Jae Hun ( ryu@geguri.kjist.ac.kr)
// Last update : 1999. 7. 4

#include "ofMain.h"

class Circle
{
public:
	double GetRadius();
	ofPoint GetCenter();
	Circle(ofPoint p1, ofPoint p2, ofPoint p3);	// p1, p2, p3 are co-planar
	Circle();
	virtual ~Circle();

private:
	double CalcCircle(ofPoint pt1, ofPoint pt2, ofPoint pt3);
	bool IsPerpendicular(ofPoint pt1, ofPoint pt2, ofPoint pt3);
	double radius;
	ofPoint center;
};