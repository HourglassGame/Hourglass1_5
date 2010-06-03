#ifndef BOOLEAN_LITERAL_EXPRESSION_H
#define BOOLEAN_LITERAL_EXPRESSION_H
/*
 *  BooleanLiteralExpression.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
#include "ArithmeticExpression.h"
class BooleanLiteralExpression : public ArithmeticExpression {
public:
	BooleanLiteralExpression(const bool value);
	~BooleanLiteralExpression();
	int CurrentValue(const unsigned int time) const;
private:
	const bool value_;
};
#endif //BOOLEAN_LITERAL_EXPRESSION_H
