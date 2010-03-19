#include "Editor_Exceptions.h"
#include "Editor_Object.h" // class's header file

// class constructor
Object::Object(const int newXPos,const int newYPos,const double newXSpeed,const double newYSpeed):
    xPos(newXPos),
    yPos(newYPos),
    xSpeed(newXSpeed),
    ySpeed(newYSpeed),
    drawFacing(false),
    selected(false),
	xPositionField(IntField(100,628,10,1)),
	yPositionField(IntField(100,648,10,1)),
	xSpeedField(DoubleField(100,668,10,1)),
	ySpeedField(DoubleField(100,688,10,1))
{ }
// class destructor
Object::~Object()
{ }
void Object::DoGui()
{
	xPositionField.Init(xPos);
	yPositionField.Init(yPos);
	xSpeedField.Init(xSpeed);
	ySpeedField.Init(ySpeed);
	xPos = xPositionField.Update();
	yPos = yPositionField.Update();
	xSpeed = xSpeedField.Update();
	ySpeed = ySpeedField.Update();
}
void Object::SetData(const int newXPos,const int newYPos,const double newXSpeed,const double newYSpeed) //type - 0 = not yet defined, 1 = Guy, 2 = Box
{
     xPos = newXPos;
     yPos = newYPos;
     xSpeed = newXSpeed;
     ySpeed = newYSpeed;
}
void Object::SetPos(int newXPos, int newYPos)
{
     xPos = newXPos;
     yPos = newYPos;
}

void Object::GetData(int& outputXPos, int& outputYPos, double& outputXSpeed, double& outputYSpeed)
{
    outputXPos = xPos;
    outputYPos = yPos;
    outputXSpeed = xSpeed;
    outputYSpeed = ySpeed;
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
