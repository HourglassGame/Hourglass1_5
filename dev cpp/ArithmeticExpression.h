#ifndef ARITHMETIC_EXPRESSION_H
#define ARITHMETIC_EXPRESSION_H
/*
 *  ArithmeticExpression.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
class ArithmeticExpression {
public:
	ArithmeticExpression();
	virtual ~ArithmeticExpression() = 0;
	virtual int CurrentValue(const unsigned int time) const = 0;
};
#endif //ARITHMETIC_EXPRESSION_H
