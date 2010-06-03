#ifndef EVENT_GROUP_H
#define EVENT_GROUP_H
/*
 *  EventGroup.h
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 30/05/10.
 *  Copyright 2010 Team Casality. All rights reserved.
 *
 */
/**
 * Parent Class for all things which set instructions (low level).
 *
 */
class EventGroup;
class EventGroup {
public:
	EventGroup();
	virtual ~EventGroup();
	virtual void Execute(const int time) const = 0;
};
#endif //EVENT_GROUP_H
