/*
 *  NegationExpression.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "NegationExpression.h"

NegationExpression::NegationExpression(const ArithmeticExpression& value) :
value_(value)
{}

NegationExpression::~NegationExpression() {
	delete &value_;
}

int NegationExpression::CurrentValue(const unsigned int time) const {
	return 0 - value_.CurrentValue(time);
}
