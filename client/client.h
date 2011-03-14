/*/////////////////////////////////////////////////////////////////////////////////////////////////
 Client program
 This program communications with controllers.
 //////////////////////////////////////////////////////////////////////////////////////////////////*/
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <cmath>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <sys/time.h>

#include "../common/claimteam.pb.h"
#include "../common/clientrobot.pb.h"
#include "../common/puckstack.pb.h"
#include "../common/serverrobot.pb.h"
#include "../common/timestep.pb.h"
#include "../common/worldinfo.pb.h"
#include "../common/ports.h"
#include "../common/net.h"
#include "../common/messagequeue.h"
#include "../common/messagereader.h"
#include "../common/except.h"

#include <gtk/gtk.h>

#include "../common/helper.h"
#include "../common/globalconstants.h"
#include "../worldviewer/drawer.h"

using namespace std;
using namespace google;
using namespace protobuf;

enum EventType {
	EVENT_CLOSEST_ROBOT_STATE_CHANGE,
	EVENT_NEW_CLOSEST_ROBOT,
	EVENT_START_SEEING_PUCKS,
	EVENT_END_SEEING_PUCKS,
	EVENT_CAN_PICKUP_PUCK,
	EVENT_NEAR_PUCK,
	EVENT_NOT_MOVING,
	EVENT_MAX
};

class SeenPuck {
public:
	double relx;
	double rely;
	int stackSize;

	SeenPuck() :
		relx(0.0), rely(0.0), stackSize(1) {
	}
};

class Robot {
public:
	double vx;
	double vy;
	double angle;
	bool hasPuck;
	bool hasCollided;

	Robot() :
		vx(0.0), vy(0.0), angle(0.0), hasPuck(false), hasCollided(false) {
	}
};

class SeenRobot: public Robot {
public:
	unsigned int id;
	int lastTimestepSeen;
	bool viewable;
	double relx;
	double rely;

	SeenRobot() :
		Robot(), id(-1), lastTimestepSeen(-1), viewable(true), relx(0.0), rely(0.0) {
	}
};

class OwnRobot: public Robot {
public:
	bool pendingCommand;
	int whenLastSent;
	int closestRobotId;
	int behaviour;
	vector<SeenRobot*> seenRobots;
	vector<SeenPuck*> seenPucks;
	double homeRelX;
	double homeRelY;
	vector<EventType> eventQueue;

	OwnRobot() :
		Robot(), pendingCommand(false), whenLastSent(-1), closestRobotId(-1), behaviour(-1), homeRelX(0.0), homeRelY(
				0.0) {
	}
};

class ClientRobotCommand {
public:
	bool sendCommand;
	bool changeVx;
	double vx;
	bool changeVy;
	double vy;
	bool changeAngle;
	double angle;
	bool changePuckPickup;
	bool puckPickup;

	ClientRobotCommand() :
		sendCommand(false), changeVx(false), vx(0.0), changeVy(false), vy(0.0), changeAngle(false), angle(0.0),
				changePuckPickup(false), puckPickup(false) {
	}
};

class ClientViewer {
private:
	ofstream debug;
	int viewedRobot, *drawFactor;
	string builderPath;
	GtkBuilder *builder;
	GtkDrawingArea *drawingArea;
	bool draw;
	OwnRobot ownRobotDraw;

public:
	void initClientViewer(int, int, int);
	void updateViewer(OwnRobot* ownRobot);
	int getViewedRobot() {
		return viewedRobot;
	}

	ClientViewer(char*);
	~ClientViewer();
};

struct passToRun {
	bool runClientViewer;

	net::connection controller;
	ClientViewer* viewer;

	passToRun(bool _runClientViewer, net::connection _controller, ClientViewer* _viewer) :
		runClientViewer(_runClientViewer), controller(_controller), viewer(_viewer) {
	}
	;
};

struct passToDrawingAreaExpose {
	int myTeam;
	int numberOfRobots;
	int *drawFactor;
	bool *draw;
	OwnRobot* ownRobotDraw;
	GtkToggleToolButton *info;
	GtkBuilder *builder;

	passToDrawingAreaExpose(int _myTeam, int _numberOfRobots, int *_drawFactor,  bool* _draw,
			OwnRobot* _ownRobotDraw, GtkToggleToolButton *_info,	GtkBuilder *_builder) :
		myTeam(_myTeam), numberOfRobots(_numberOfRobots), drawFactor(_drawFactor),
				draw(_draw), ownRobotDraw(_ownRobotDraw), info(_info), builder(_builder) {
	}
	;
};

struct passToZoom {
	int *drawFactor;
	GtkWidget *mainWindow;
	GtkDrawingArea *drawingArea;
	GtkToolButton *zoomIn;
	GtkToolButton *zoomOut;

	passToZoom( int *_drawFactor, GtkWidget *_mainWindow, GtkDrawingArea *_drawingArea, GtkToolButton *_zoomIn, GtkToolButton *_zoomOut) :
		 drawFactor(_drawFactor),  mainWindow(_mainWindow), drawingArea(_drawingArea), zoomIn(_zoomIn), zoomOut(_zoomOut)  {
	}
	;
};

#endif
