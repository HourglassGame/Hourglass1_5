#ifndef INSTRUCTION_DEFAULTER_H
#define INSTRUCTION_DEFAULTER_H
/*
 *  InstructionDefaulter.h
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 2/06/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

class InstructionDefaulter;
#include "EventGroup.h"
#include "ArithmeticExpression.h"
#include "TriggerEngine.h"
class InstructionDefaulter : public EventGroup {
public:
	InstructionDefaulter(TriggerEngine& engine, const unsigned int indexToDefault);
	~InstructionDefaulter();
	void Execute(const unsigned int time) const;
private:
	TriggerEngine& engine_;
	const unsigned int indexToSet_;
};
#endif //INSTRUCTION_DEFAULTER_H
