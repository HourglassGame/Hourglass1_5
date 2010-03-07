// Class automatically generated by Dev-C++ New Class wizard

#include "exceptions.h"
#include "object.h" // class's header file

//extern BITMAP* guy_left_stop;
//extern BITMAP* guy_right_stop;
//extern BITMAP* box_sprite;

//extern BITMAP* buffer;


// class constructor
Object::Object()
{
   xPos = 0;
   yPos = 0;
   xSpeed = 0;
   ySpeed = 0;
   drawFacing = false;
   type = 0;
}

// class destructor
Object::~Object()
{
	// insert your code here
}

void Object::SetData(int newXPos, int newYPos, double newXSpeed, double newYSpeed, int newType) //type - 0 = not yet defined, 1 = Guy, 2 = Box
{
     xPos = newXPos;
     yPos = newYPos;
     xSpeed = newXSpeed;
     ySpeed = newYSpeed;
     type = newType;
}
void Object::SetPos(int newXPos, int newYPos)
{
     xPos = newXPos;
     yPos = newYPos;
}

/*
void Object::DoDraw()
{
     switch(type)
     {
         case (1):
         {
             if(drawFacing)
             {
                 draw_sprite(buffer, guy_right_stop, xPos, yPos);
                 if(selected)
                 {
                    rectfill(buffer,xPos,yPos,xPos+GetXSize(),yPos+GetYSize(),makecol(255,0,0));
                 }
             }
             else
             {
                 draw_sprite(buffer, guy_left_stop, xPos, yPos);
                 if(selected)
                 {
                    rectfill(buffer,xPos,yPos,xPos+GetXSize(),yPos+GetYSize(),makecol(255,0,0));
                 }
             }
             break;
         }
         case (2):
         {
              draw_sprite(buffer, box_sprite, xPos, yPos);
              if(selected)
              {
                 rectfill(buffer,xPos,yPos,xPos+GetXSize(),yPos+GetYSize(),makecol(255,0,0));
              }
              break;
         }
         default:
         {
              allegro_message("Trying to draw Object which has not yet been defined");
              throw UndefinedObjectException();
         }
     }
     
}
*/
void Object::GetData(int& outputXPos, int& outputYPos, double& outputXSpeed, double& outputYSpeed)
{
    outputXPos = xPos;
    outputYPos = yPos;
    outputXSpeed = xSpeed;
    outputYSpeed = ySpeed;
}

int Object::GetType()
{
    return(type);
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

int Object::GetXSize()
{
    switch (type)
    {
       case(1):
       {
          return(GUY_WIDTH);
       }
       case(2):
       {
          return(BOX_WIDTH);
       }   
    }
}
int Object::GetYSize()
{
    switch (type)
    {
       case(1):
       {
          return(GUY_HEIGHT);
       }
       case(2):
       {
          return(BOX_HEIGHT);
       }   
    }
}