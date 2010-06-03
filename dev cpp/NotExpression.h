#ifndef NOT_EXPRESSION_H
#define NOT_EXPRESSION_H
/*
 *  NotExpression.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
#include "ArithmeticExpression.h"
class NotExpression : public ArithmeticExpression {
public:
	NotExpression(const ArithmeticExpression& value);
	~NotExpression();
	int CurrentValue(const unsigned int time) const;
private:
	const ArithmeticExpression& value_;
	NotExpression(const NotExpression&);
	NotExpression& operator=(const NotExpression&);
};
#endif //NOT_EXPRESSION_H
