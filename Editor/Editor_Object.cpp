#include "Editor_Exceptions.h"
#include "Editor_Object.h" // class's header file

// class constructor
Object::Object(const int newXPos,const int newYPos):
    xPos(newXPos),
    yPos(newYPos),
    drawFacing(false),
    selected(false),
	xPositionField(IntField(100,628,10,1)),
	yPositionField(IntField(100,648,10,1))
{ }
// class destructor
Object::~Object()
{ }

void Object::DoGui()
{
	InitGui();
	UpdateGui();
}

void Object::InitGui()
{
	xPositionField.Init(xPos);
	yPositionField.Init(yPos);
}
void Object::UpdateGui()
{
	xPos = xPositionField.Update();
	yPos = yPositionField.Update();
}

void Object::SetPos(int newXPos, int newYPos)
{
     xPos = newXPos;
     yPos = newYPos;
}
std::string Object::GetOutputStringParts()
{
	std::stringstream ss;
	ss << "\n" << "X_POS=" << yPos << "\n" <<  "Y_POS=" << yPos;
	return(ss.str());
}
void Object::SetSelected(bool newSelected)
{
     selected = newSelected;    
}

bool Object::DoSelectionCheck()
{
     if(mouse_x > xPos && mouse_x < xPos + GetXSize() && mouse_y > yPos && mouse_y < yPos + GetYSize())
     {
         selected = true;
         return(true);
     }
     return(false);
}
