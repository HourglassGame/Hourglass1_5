#include "Editor_InputField.h"
#include "Editor_InputMap.h"
extern BITMAP* buffer;
//std::map<HourglassInput,SwitchingInput> inputs = InputMap::GetMap();
InputField::InputField(const int tLX,const int tLY,const int Width,const int Height) :
inputs(InputMap::GetMap()),
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
