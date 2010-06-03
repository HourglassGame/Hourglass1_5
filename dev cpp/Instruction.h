#ifndef INSTRUCTION_H
#define INSTRUCTION_H
/*
 *  Instruction.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 24/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

class Instruction {
public:
	Instruction();
	~Instruction();
	void MakeBoring();
	void SetValue(const int value);
	
	int GetValue() const;
	bool IsToBeIgnored() const;
private:
	int value_; 
	bool isToBeIgnored;
};
#endif //INSTRUCTION_H
