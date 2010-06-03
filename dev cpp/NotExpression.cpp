/*
 *  NotExpression.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "NotExpression.h"

NotExpression::NotExpression(const ArithmeticExpression& value) :
value_(value)
{}

NotExpression::~NotExpression(){
	delete &value_;
}

int NotExpression::CurrentValue(const unsigned int time) const {
	return !value_.CurrentValue(time);
}
