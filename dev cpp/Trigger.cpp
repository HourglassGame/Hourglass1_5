/*
 *  Trigger.cpp
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 30/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "Trigger.h"

Trigger::Trigger(ArithmeticExpression& condition, EventGroup& trueEvents, EventGroup& falseEvents) :
condition_(condition),
trueEvents_(trueEvents),
falseEvents_(falseEvents)
{
}

Trigger::~Trigger() {
	delete &condition_;
	delete &trueEvents_;
	delete &falseEvents_;
}
void Trigger::Execute(const int time) const {
	if (condition_.CurrentValue(time)) {
		trueEvents_.Execute(time);
	}
	else {
		falseEvents_.Execute(time);
	}
}
