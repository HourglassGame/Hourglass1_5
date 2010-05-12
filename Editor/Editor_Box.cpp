#include "Editor_Box.h" // class's header file
extern BITMAP* buffer;
extern BITMAP* box_sprite;

Box::Box(const int newXPos, const int newYPos, const double newXSpeed, const double newYSpeed,const AbsoluteTimeDirectionEnum newATD) :
timeDirectionObject(TimeDirectionObject(newATD)),
mobileObject(MobileObject(newXSpeed,newYSpeed)),
object(ObjectSkeleton(newXPos,newYPos,WIDTH,HEIGHT))
{ }

Box::~Box()
{ }

void Box::SetPos(const int newXPos, const int newYPos) {
	object.SetPos(newXPos, newYPos);
}
void Box::DoGui() {
	object.DoGui();
}
bool Box::DoSelectionCheck() {
	return object.DoSelectionCheck();
}
void Box::SetSelected(const bool newSelected) {
	object.SetSelected(newSelected);
}
void Box::InitGui()
{
	timeDirectionObject.InitGui();
	object.InitGui();
	mobileObject.InitGui();
}
void Box::UpdateGui()
{
	timeDirectionObject.UpdateGui();
	object.UpdateGui();
	mobileObject.UpdateGui();
}

void Box::DoDraw()
{
    draw_sprite(buffer, box_sprite, object.xPos, object.yPos);
    if(object.selected)
    {
        rect(buffer,object.xPos,object.yPos,object.xPos+WIDTH,object.yPos+HEIGHT,makecol(255,0,0));
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
	returnString += object.GetOutputStringParts();
	returnString += mobileObject.GetOutputStringParts();
	returnString += timeDirectionObject.GetOutputStringParts();
	returnString += "\n</BOX>";
	return(returnString);
}
