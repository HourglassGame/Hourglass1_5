#ifndef EXPRESSION_TEMPLATE_H
#define EXPRESSION_TEMPLATE_H
/*
 *  ExpressionTemplate.h
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 2/06/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
//I got kind of annoyed at having to change the same thing 
//everywere whenever I wanted to change the implementation of expressions
//so I wrote these helpful macros for the most common ones

//Gives class declaration for a standard binary expression
#define mEXPRESSION_HEADER(name)\
class name : public ArithmeticExpression {\
public:\
name(const ArithmeticExpression& lhs, const ArithmeticExpression& rhs);\
~name();\
int CurrentValue(const unsigned int time) const;\
private:\
const ArithmeticExpression& lhs_;\
const ArithmeticExpression& rhs_;\
name(const name&);\
name& operator=(const name&);\
};

//Gives class definition for a slightly less standard binary expression
#define mEXPRESSION_DEFINITION_EX(name, before, between, after)\
name::name(const ArithmeticExpression& lhs, const ArithmeticExpression& rhs) :\
lhs_(lhs),\
rhs_(rhs)\
{}\
\
name::~name() {\
delete &lhs_;\
delete &rhs_;\
}\
\
int name::CurrentValue(const unsigned int time) const {\
return before lhs_.CurrentValue(time) between rhs_.CurrentValue(time) after;\
}

//Suitably long macro names -- I don't want them causing conflicts |;
#define mHOURGLASS_EXPRESSION_TEMPLATE_NOTBRACKET_BEFORE__ !(
#define mHOURGLASS_EXPRESSION_TEMPLATE_NOTBRACKET_AFTER__ )

//Gives class definition for a standard binary expression with an extra negtion
#define mEXPRESSION_DEFINITION_NOT(name, between)\
mEXPRESSION_DEFINITION_EX(name,mHOURGLASS_EXPRESSION_TEMPLATE_NOTBRACKET_BEFORE__,between,mHOURGLASS_EXPRESSION_TEMPLATE_NOTBRACKET_AFTER__)

//Gives class definition for a standard binary expression
#define mEXPRESSION_DEFINITION(name, between) mEXPRESSION_DEFINITION_EX(name,,between,)

#endif //EXPRESSION_TEMPLATE_H
