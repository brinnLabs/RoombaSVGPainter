from svgpathtools import *
import math

paths, attributes, svg_attributes = svg2paths('Elephant.svg', return_svg_attributes=True)

def is_empty(any_structure):
	if any_structure:
		return False
	else:
		return True

viewbox = svg_attributes["viewBox"]
x,y,w,h = viewbox.split(' ')

scale = 250.0/ float(max(w, h))

allSegments = []

for path in paths:
	for segment in path:
		'''
		Since shapes share points we can't scale every point or they
		begin to stack and dont scale properly. There are some exceptions
		like a single line path
		'''
		if(path[0] == segment):
			segment.start.scale(scale);
		if(len(path) == 1 or not path[-1] == segment or (not path.isclosed() and path[-1] == segment)):
			segment.end.scale(scale);
		if isinstance(segment, Arc):
			segment.radius.scale(scale)
		if isinstance(segment, QuadraticBezier):
			segment.control.scale(scale)
		if isinstance(segment, CubicBezier):
			segment.control1.scale(scale)
			segment.control2.scale(scale)
		allSegments.append(segment)

idealPath = []
currentPos = Point(0,0)

while(not is_empty(allSegments)):
	closestPoint = Point(1000, 1000)
	bestPath = allSegments[0]
	segment2del = allSegments[0]
	for segment in allSegments:
		if (currentPos.distance(segment.start) < currentPos.distance(closestPoint)):
			closestPoint = segment.start
			segment2del = segment
			bestPath = segment
		if (currentPos.distance(segment.end) < currentPos.distance(closestPoint)):
			closestPoint = segment.end
			segment2del = segment
			bestPath = segment.reversed()
	if(currentPos == bestPath.start):
		idealPath.append({'path': bestPath, 'pen': True})
	else:
		idealPath.append({'path': Line(currentPos, bestPath.start), 'pen': False})
		idealPath.append({'path': bestPath, 'pen': True})
	currentPos = bestPath.end
	allSegments.remove(segment2del)

idealPath.append({'path': Line(idealPath[-1]['path'].end, Point(0,0)), 'pen': False})

pathlen = 0

for segment in idealPath:
	pathlen += segment['path'].start.distance(segment['path'].end)
	print(segment)

print("Length of Total Path = " + str(pathlen))