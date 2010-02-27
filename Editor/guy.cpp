// Class automatically generated by Dev-C++ New Class wizard

#include "guy.h" // class's header file

extern BITMAP* guy_left_stop;
extern BITMAP* guy_right_stop;

extern BITMAP* buffer;

// class constructor
Guy::Guy()
{
   drawFacing = false;
   int xPos = 0;
   int yPos = 0;
   double xSpeed = 0;
   double ySpeed = 0;

}

// class destructor
Guy::~Guy()
{
	// insert your code here
}

void Guy::SetData(int newXPos, int newYPos, double newXSpeed, double newYSpeed)
{
     xPos = newXPos;
     yPos = newYPos;
}
void Guy::DoDraw()
{
     if(drawFacing)
     {
        draw_sprite(buffer, guy_right_stop, xPos, yPos);
     }
     else
     {
        draw_sprite(buffer, guy_left_stop, xPos, yPos);
     }
}
void Guy::GetData(int& outputXPos, int& outputYPos, double& outputXSpeed, double& outputYSpeed)
{
    outputXPos = xPos;
    outputYPos = yPos;
    outputXSpeed = xSpeed;
    outputYSpeed = ySpeed;
}
