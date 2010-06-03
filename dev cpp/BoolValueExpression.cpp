/*
 *  BoolValueExpression.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 25/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "BoolValueExpression.h"

BoolValueExpression::BoolValueExpression(const boost::function<const bool(const unsigned int)> dataSource) :
dataSource_(dataSource)
{
}
BoolValueExpression::~BoolValueExpression(){}
int BoolValueExpression::CurrentValue(const unsigned int time) const {
	return dataSource_(time);
}

