#ifndef NEGATION_EXPRESSION_H
#define NEGATION_EXPRESSION_H
/*
 *  NegationExpression.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
#include "ArithmeticExpression.h"
class NegationExpression : public ArithmeticExpression {
public:
	NegationExpression(const ArithmeticExpression& value);
	~NegationExpression();
	int CurrentValue(const unsigned int time) const;
private:
	const ArithmeticExpression& value_;
	NegationExpression(const NegationExpression&);
	NegationExpression& operator=(const NegationExpression&);
};
#endif //NEGATION_EXPRESSION_H
