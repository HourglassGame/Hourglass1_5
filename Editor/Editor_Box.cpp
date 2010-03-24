#include "Editor_Box.h" // class's header file
extern BITMAP* buffer;
extern BITMAP* box_sprite;

Box::Box(const int newXPos, const int newYPos, const double newXSpeed, const double newYSpeed, AbsoluteTimeDirectionEnum newATD) :
TimeDirectionObject(newATD),
MobileObject(newXSpeed,newYSpeed),
Object(newXPos,newYPos)
{ }

Box::~Box()
{ }

void Box::DoGui()
{
	InitGui();
	UpdateGui();
}

void Box::InitGui()
{
	TimeDirectionObject::InitGui();
	Object::InitGui();
	MobileObject::InitGui();
}
void Box::UpdateGui()
{
	TimeDirectionObject::UpdateGui();
	Object::UpdateGui();
	MobileObject::UpdateGui();
}

void Box::DoDraw()
{
    draw_sprite(buffer, box_sprite, xPos, yPos);
    if(selected)
    {
        rect(buffer,xPos,yPos,xPos+WIDTH,yPos+HEIGHT,makecol(255,0,0));
    }
}

int Box::GetXSize()
{
    return(WIDTH);   
}
int Box::GetYSize()
{
    return(HEIGHT);   
}

std::string Box::GetOutputString()
{
	std::string returnString = "<BOX>";
	returnString += Object::GetOutputStringParts();
	returnString += MobileObject::GetOutputStringParts();
	returnString += TimeDirectionObject::GetOutputStringParts();
	returnString += "\n</BOX>";
	return(returnString);
}
