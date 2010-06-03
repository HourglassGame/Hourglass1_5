#ifndef INT_VALUE_EXPRESSION_H
#define INT_VALUE_EXPRESSION_H
/*
 *  IntValueExpression.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 25/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "ArithmeticExpression.h"
#include "boost/function.hpp"
class IntValueExpression : public ArithmeticExpression {
public:
	IntValueExpression(const boost::function<const int(const unsigned int)> dataSource);
	~IntValueExpression();
	int CurrentValue(const unsigned int time) const;
private:
	const boost::function<const int(const unsigned int)> dataSource_;
	IntValueExpression(const IntValueExpression&);
	IntValueExpression& operator=(const IntValueExpression&);
};

#endif //INT_VALUE_EXPRESSION_H
