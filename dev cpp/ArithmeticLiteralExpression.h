#ifndef ARITHMETIC_LITERAL_EXPRESSION_H
#define ARITHMETIC_LITERAL_EXPRESSION_H
/*
 *  ArithmeticLiteralExpression.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
#include "ArithmeticExpression.h"
class ArithmeticLiteralExpression : public ArithmeticExpression {
public:
	ArithmeticLiteralExpression(const int value);
	~ArithmeticLiteralExpression();
	int CurrentValue(const unsigned int time) const;
private:
	const int value_;
	ArithmeticLiteralExpression(const ArithmeticLiteralExpression&);
	ArithmeticLiteralExpression& operator=(const ArithmeticLiteralExpression&);
};
#endif //ARITHMETIC_LITERAL_EXPRESSION_H
