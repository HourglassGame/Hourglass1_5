#ifndef BOOL_VALUE_EXPRESSION_H
#define BOOL_VALUE_EXPRESSION_H
/*
 *  BoolValueExpression.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 25/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "ArithmeticExpression.h"
#include "boost/function.hpp"
class BoolValueExpression : public ArithmeticExpression {
public:
	BoolValueExpression(const boost::function<const bool(const unsigned int)> dataSource);
	~BoolValueExpression();
	int CurrentValue(const unsigned int time) const;
private:
	const boost::function<const bool(const unsigned int)> dataSource_;
	BoolValueExpression(const BoolValueExpression&);
	BoolValueExpression& operator=(const BoolValueExpression&);
};

#endif //BOOL_VALUE_EXPRESSION_H
