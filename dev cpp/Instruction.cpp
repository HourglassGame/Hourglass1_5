/*
 *  Instruction.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "Instruction.h"
#include <stdexcept>
#include "HourglassAssert.h"
Instruction::Instruction() :
value_(0),
isToBeIgnored(true)
{
}

Instruction::~Instruction()
{
}

void Instruction::MakeBoring()
{
	isToBeIgnored = true;
}

void Instruction::SetValue(const int value)
{
	value_ = value;
	isToBeIgnored = false;
}

int Instruction::GetValue() const
{
	hg_assert(!isToBeIgnored);
	//in case assertions are disabled or something :P
	if (isToBeIgnored)
	{
		//TODO - reword
		throw std::logic_error("attempting to read value from instruction which is default");
	}
	return value_;
}
bool Instruction::IsToBeIgnored() const
{
	return isToBeIgnored;
}
