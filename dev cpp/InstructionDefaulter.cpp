/*
 *  InstructionDefaulter.cpp
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 2/06/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "InstructionDefaulter.h"

InstructionDefaulter::InstructionDefaulter(TriggerEngine& engine, const unsigned int indexToSet) :
engine_(engine),
indexToSet_(indexToSet)
{}

InstructionDefaulter::~InstructionDefaulter()
{}

void InstructionDefaulter::Execute(const unsigned int time) const
{
	engine_.MakeInstructionBoring(time, indexToSet_);
}
