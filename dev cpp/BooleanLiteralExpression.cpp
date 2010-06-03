/*
 *  BooleanLiteralExpression.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "BooleanLiteralExpression.h"

BooleanLiteralExpression::BooleanLiteralExpression(const bool value) :
value_(value)
{
}
BooleanLiteralExpression::~BooleanLiteralExpression(){}

int BooleanLiteralExpression::CurrentValue(const unsigned int) const {
	return value_;
}
