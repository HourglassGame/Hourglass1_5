/*
 *  Editor_InputMap.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 12/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
#include <map>
#include "Editor_HourglassInputsEnum.h"
#include "Editor_SwitchingInput.h"
class InputMap {
public:
	static std::map<HourglassInput,SwitchingInput>& GetMap();
	
private:
	InputMap();
	~InputMap();
	
	static std::map<HourglassInput,SwitchingInput> inputs;
};
