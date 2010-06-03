/*
 *  ArithmeticLiteralExpression.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "ArithmeticLiteralExpression.h"

ArithmeticLiteralExpression::ArithmeticLiteralExpression(const int value) :
value_(value)
{}

ArithmeticLiteralExpression::~ArithmeticLiteralExpression() {}

int ArithmeticLiteralExpression::CurrentValue(const unsigned int) const {
	return value_;
}
