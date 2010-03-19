#include "Editor_Guy.h" // class's header file
extern BITMAP* buffer;
extern BITMAP* guy_left_stop;
extern BITMAP* guy_right_stop;
// class constructor

Guy::Guy(const int newXPos, const int newYPos, const double newXSpeed, const double newYSpeed):
    Object(newXPos,newYPos,newXSpeed,newYSpeed)
{ }
// class destructor
Guy::~Guy()
{ }

void Guy::DoDraw()
{
    if(drawFacing)
    {
        draw_sprite(buffer, guy_right_stop, xPos, yPos);
        if(selected)
        {
            rect(buffer,xPos,yPos,xPos+WIDTH,yPos+HEIGHT,makecol(255,0,0));
        }
    }
    else
    {
        draw_sprite(buffer, guy_left_stop, xPos, yPos);
        if(selected)
        {
            rect(buffer,xPos,yPos,xPos+WIDTH,yPos+HEIGHT,makecol(255,0,0));
        }
    }   
}
int Guy::GetType()
{
    return(0);
}

int Guy::GetXSize()
{
    return(WIDTH);   
}

int Guy::GetYSize()
{
    return(HEIGHT);   
}
