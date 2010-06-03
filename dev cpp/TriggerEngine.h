#ifndef TRIGGER_ENGINE_H
#define TRIGGER_ENGINE_H
/*
 *  TriggerEngine.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
class TriggerEngine;
#include "Instruction.h"
#include "ArithmeticExpression.h"
#include <vector>
#include "boost/multi_array.hpp"
#include "Trigger.h"
#include "EventGroup.h"
#include "InstructionSetter.h"

class TriggerEngine {
public:
	TriggerEngine(const std::vector<const Trigger*>& triggers, const unsigned int noOfSteps, const unsigned int noOfInstructions);
	~TriggerEngine(); //Destroys all triggers and instructions, only call when loading level.

	//Instructions contain both a value, and an indication of defaultness.
	const Instruction& GetInstruction(const int time, const unsigned int instructionIndex) const;
	void UpdateInstructions(const unsigned int time);
	friend class InstructionSetter;
	friend class InstructionDefaulter;
private:
	void SetInstruction(const unsigned int time, const unsigned int instructionIndex, const int valueToSetTo);
	void MakeInstructionBoring(const unsigned int time, const unsigned int instructionIndex);
	const Instruction defaultInstruction;
	//[time][instructionindex]
	boost::multi_array<Instruction,2> instructions;
	std::vector <const Trigger*> triggers_;
	const unsigned int noOfInstructions_;
	const unsigned int noOfSteps_;
};
#endif //TRIGGER_ENGINE_H
