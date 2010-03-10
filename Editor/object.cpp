#include "Exceptions.h"
#include "Object.h" // class's header file

// class constructor
Object::Object(const int newXPos,const int newYPos,const double newXSpeed,const double newYSpeed):
    xPos(newXPos),
    yPos(newYPos),
    xSpeed(newXSpeed),
    ySpeed(newYSpeed),
    drawFacing(false),
    selected(false)
{
}
// class destructor
Object::~Object()
{
	// insert your code here
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
