/*
 *  EventSet.cpp
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 30/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "EventSet.h"
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
EventSet::EventSet() :
eventsInSet(std::vector<const EventGroup*>())
{}

EventSet::~EventSet() {
	foreach(const EventGroup* group, eventsInSet) {
		delete group;
	}
}
void EventSet::Add(const EventGroup& toAdd) {
	eventsInSet.push_back(&toAdd);
}
void EventSet::Execute(const int time) const {
	foreach(const EventGroup* group, eventsInSet) {
		group->Execute(time);
	}
}