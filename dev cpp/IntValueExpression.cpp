/*
 *  IntValueExpression.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 25/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "IntValueExpression.h"

IntValueExpression::IntValueExpression(const boost::function<const int(const unsigned int)> dataSource) :
dataSource_(dataSource)
{
}
IntValueExpression::~IntValueExpression(){}
int IntValueExpression::CurrentValue(const unsigned int time) const {
	return dataSource_(time);
}
