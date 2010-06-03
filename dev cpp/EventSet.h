/*
 *  EventSet.h
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 30/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
class EventSet;
#include "EventGroup.h"
#include <vector>
class EventSet : public EventGroup {
public:
	EventSet();
	~EventSet();
	void Add(const EventGroup& toAdd);
	void Execute(const int time) const;
private:
	std::vector<const EventGroup*> eventsInSet;
};
