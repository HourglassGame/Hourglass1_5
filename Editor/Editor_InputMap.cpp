/*
 *  Editor_InputMap.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 12/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "Editor_InputMap.h"
std::map<HourglassInput,SwitchingInput> InputMap::inputs;

std::map<HourglassInput,SwitchingInput>& InputMap::GetMap() {
	return inputs;
}

InputMap::InputMap() {
	
}
InputMap::~InputMap() {

}
