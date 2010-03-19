/*
 *  Editor_InputField.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 18/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Editor_InputField.h"

/*
 *  TextField.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Editor_InputField.h"
extern BITMAP* buffer;
extern std::map<HourglassInput,SwitchingInput> inputs;
InputField::InputField(const int tLX,const int tLY,const int Width,const int Height) :
x0(tLX),
y0(tLY),
width(Width),
height(Height),
cursorPosition(0),
currentValue("0")
{}

InputField::~InputField()
{
}
/*
void InputField::Init(const int& initialValue)
{}
int InputField::Update()
{}
void InputField::Init(const double& initialValue)
{}
double InputField::Update()
{}
 */
void InputField::TranslateInputs() 
{
    for(std::map<HourglassInput,SwitchingInput>::iterator it = inputs.begin(); it != inputs.end(); it++)
    {
        it->second.Update();
    }
}
//std::string InputField::readVal(int input)
//{

//	std::string output("");
	/*
	if ((input & 0xff) == '0') {
		output.assign("0");
	}
	else if ((input & 0xff) == '1') {
		output.assign("1");
	}
	else if ((input & 0xff) == '2') {
		output.assign("2");
	}
	else if ((input & 0xff) == '3') {
		output.assign("3");
	}
	else if ((input & 0xff) == '4') {
		output.assign("4");
	}
	else if ((input & 0xff) == '5') {
		output.assign("5");
	}
	else if ((input & 0xff) == '6') {
		output.assign("6");
	}
	else if ((input & 0xff) == '7') {
		output.assign("7");
	}
	else if ((input & 0xff) == '8') {
		output.assign("8");
	}
	else if ((input & 0xff) == '9') {
		output.assign("9");
	}
	 */
//	return output;
	
//}
