#include "Editor_Guy.h" // class's header file
extern BITMAP* buffer;
extern BITMAP* guy_left_stop;
extern BITMAP* guy_right_stop;

Guy::Guy(const unsigned int newXPos, const unsigned int newYPos, const double newXSpeed, const double newYSpeed,const unsigned int newStartTime,const AbsoluteTimeDirectionEnum newATD) :
	TimeDirectionObject(newATD),
	MobileObject(newXSpeed,newYSpeed),
	Object(newXPos, newYPos),
	startTime(newStartTime),
	startTimeField(IntField(200,628,10,1))
{ }

Guy::~Guy()
{ }

void Guy::InitGui()
{
	TimeDirectionObject::InitGui();
	Object::InitGui();
	MobileObject::InitGui();
	startTimeField.Init(startTime);
}
void Guy::UpdateGui()
{
	TimeDirectionObject::UpdateGui();
	Object::UpdateGui();
	MobileObject::UpdateGui();
	startTime = startTimeField.Update();
}

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

int Guy::GetXSize()
{
    return(WIDTH);   
}

int Guy::GetYSize()
{
    return(HEIGHT);   
}

std::string Guy::GetOutputString()
{
	std::string returnString = "<GUY>";
	returnString += Object::GetOutputStringParts();
	returnString += MobileObject::GetOutputStringParts();
	returnString += TimeDirectionObject::GetOutputStringParts();
	returnString += Guy::GetOutputStringParts();
	returnString += "\n</GUY>";
	return(returnString);
}
std::string Guy::GetOutputStringParts()
{
	std::stringstream ss;
	ss << "\n" << "START_TIME=" << startTime;
	return(ss.str());
}
