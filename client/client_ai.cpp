#include "client.h"

SeenPuck* ClientAi::findClosestPuck(OwnRobot* ownRobot) {
	if (ownRobot->seenPucks.size() == 0)
		return NULL;

	vector<SeenPuck*>::iterator closest;
	double minDistance = 9000.01; // Over nine thousand!
	double tempDistance;
	for (vector<SeenPuck*>::iterator it = ownRobot->seenPucks.begin(); it != ownRobot->seenPucks.end(); it++) {
		tempDistance = relDistance((*it)->relx, (*it)->rely);
		if (tempDistance < minDistance) {
			minDistance = tempDistance;
			closest = it;
		}
	}
	return *closest;
}

SeenPuck* ClientAi::findPickUpablePuck(OwnRobot* ownRobot) {
	if (ownRobot->seenPucks.size() == 0)
		return NULL;

	vector<SeenRobot*>::iterator closest;
	for (vector<SeenPuck*>::iterator it = ownRobot->seenPucks.begin(); it != ownRobot->seenPucks.end(); it++) {
		//cout << "relx= " << (*it)->relx << ", rely= " << (*it)->rely << endl;
		if (sameCoordinates((*it)->relx, (*it)->rely, 0.0, 0.0)) {
			return *it;
		}
	}
	return NULL; // Found nothing in the for loop.
}

double ClientAi::relDistance(double x1, double y1) {
	return (sqrt(x1 * x1 + y1 * y1));
}

// Check if the two coordinates are the same, compensating for
// doubleing-point errors.
bool ClientAi::sameCoordinates(double x1, double y1, double x2, double y2) {
	// From testing, it looks like doubleing point errors either add or subtract
	// 0.0001.
	double maxError = 0.1;
	if (abs(x1 - x2) > maxError) {
		return false;
	}
	if (abs(y1 - y2) > maxError) {
		return false;
	}
	return true;
}

SeenRobot* ClientAi::findClosestRobot(OwnRobot* ownRobot) {
	if (ownRobot->seenRobots.size() == 0)
		return NULL;

	vector<SeenRobot*>::iterator closest;
	double minDistance = 9000.01; // Over nine thousand!
	double tempDistance;
	for (vector<SeenRobot*>::iterator it = ownRobot->seenRobots.begin(); it != ownRobot->seenRobots.end(); it++) {
		tempDistance = relDistance((*it)->relx, (*it)->rely);
		if (tempDistance < minDistance) {
			minDistance = tempDistance;
			closest = it;
		}
	}
	return *closest;
}

bool ClientAi::hasCanPickUpPuckEvent(OwnRobot* ownRobot) {
	for (vector<EventType>::iterator it = ownRobot->eventQueue.begin(); it != ownRobot->eventQueue.end(); it++) {
		if (*it == EVENT_CAN_PICKUP_PUCK)
			return true;
	}
	return false;
}

bool ClientAi::hasNotMovingEvent(OwnRobot* ownRobot) {
	for (vector<EventType>::iterator it = ownRobot->eventQueue.begin(); it != ownRobot->eventQueue.end(); it++) {
		if (*it == EVENT_NOT_MOVING)
			return true;
	}
	return false;
}
