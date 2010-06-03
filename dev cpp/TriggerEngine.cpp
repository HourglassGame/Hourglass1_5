/*
 *  TriggerEngine.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "TriggerEngine.h"
#include "HourglassAssert.h"
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
TriggerEngine::TriggerEngine(const std::vector<const Trigger*>& triggers, const unsigned int noOfSteps, const unsigned int noOfInstructions) :
triggers_(triggers),
instructions(boost::multi_array<Instruction,2>(boost::extents[noOfSteps][noOfInstructions])),
defaultInstruction(Instruction()),
noOfInstructions_(noOfInstructions),
noOfSteps_(noOfSteps)
{}
//Destroys all triggers and instructions
TriggerEngine::~TriggerEngine()
{
	foreach(const Trigger* trigger, triggers_) {
		delete trigger;
	}
	while (triggers_.size() != 0) {
		triggers_.pop_back();
	}
	delete &triggers_;
}

//Instructions contain both a value, and an indication of defaultness.
const Instruction& TriggerEngine::GetInstruction(const int time, const unsigned int instructionIndex) const {
	hg_assert(instructionIndex >= 0 && instructionIndex < noOfInstructions_);
	if (time < 0 || time >= noOfSteps_) {
		return defaultInstruction;
	}
	else {
		return instructions[time][instructionIndex];
	}
}

void TriggerEngine::UpdateInstructions(const unsigned int time) {
	foreach(Instruction instr, instructions[time]) {
		instr.MakeBoring();
	}
	foreach(const Trigger* trigger, triggers_) {
		trigger->Execute(time);
	}
}

void TriggerEngine::SetInstruction(const unsigned int time, const unsigned int instructionIndex, const int valueToSetTo) {
	hg_assert(instructionIndex >= 0 && instructionIndex < noOfInstructions_);
	hg_assert(time >= 0 && time < noOfSteps_);
	instructions[time][instructionIndex].SetValue(valueToSetTo);
}

void TriggerEngine::MakeInstructionBoring(const unsigned int time, const unsigned int instructionIndex) {
	hg_assert(instructionIndex >= 0 && instructionIndex < noOfInstructions_);
	hg_assert(time >= 0 && time < noOfSteps_);
	instructions[time][instructionIndex].MakeBoring();
}
