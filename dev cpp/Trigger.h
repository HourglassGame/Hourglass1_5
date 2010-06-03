#ifndef TRIGGER_H
#define TRIGGER_H
/*
 *  Trigger.h
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 30/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
class Trigger;
#include "EventGroup.h"
#include "ArithmeticExpression.h"
class Trigger : public EventGroup {
public:
	Trigger(ArithmeticExpression& condition, EventGroup& trueEvents, EventGroup& falseEvents);
	~Trigger();
	void Execute(const int time) const;
private:
	ArithmeticExpression& condition_;
	EventGroup& trueEvents_;
	EventGroup& falseEvents_;
};
#endif //TRIGGER_H