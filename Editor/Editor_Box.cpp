#include "Editor_Box.h" // class's header file
extern BITMAP* buffer;
extern BITMAP* box_sprite;

Box::Box(const int newXPos, const int newYPos, const double newXSpeed, const double newYSpeed):
    Object(newXPos,newYPos,newXSpeed,newYSpeed)
{

}

Box::~Box()
{

}
void Box::DoDraw()
{
    draw_sprite(buffer, box_sprite, xPos, yPos);
    if(selected)
    {
        rect(buffer,xPos,yPos,xPos+WIDTH,yPos+HEIGHT,makecol(255,0,0));
    }
}
int Box::GetType()
{
    return(1);
}

int Box::GetXSize()
{
    return(WIDTH);   
}
int Box::GetYSize()
{
    return(HEIGHT);   
}

