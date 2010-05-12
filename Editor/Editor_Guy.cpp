#include "Editor_Guy.h" // class's header file
extern BITMAP* buffer;
extern BITMAP* guy_left_stop;
extern BITMAP* guy_right_stop;

Guy::Guy(const unsigned int newXPos, const unsigned int newYPos, const double newXSpeed, const double newYSpeed, const unsigned int newStartTime, const AbsoluteTimeDirectionEnum newATD) :
	timeDirectionObject(TimeDirectionObject(newATD)),
	mobileObject(MobileObject(newXSpeed,newYSpeed)),
	object(ObjectSkeleton(newXPos, newYPos, WIDTH, HEIGHT)),
	startTime(newStartTime),
	startTimeField(IntField(200,628,10,1))
{ }

Guy::~Guy()
{ }

void Guy::SetPos(const int newXPos,const int newYPos) {
	object.SetPos(newXPos, newYPos);
}
void Guy::DoGui() {
	InitGui();
	UpdateGui();
}
void Guy::SetSelected(const bool newSelected) {
	object.SetSelected(newSelected);
}
bool Guy::DoSelectionCheck() {
	return object.DoSelectionCheck();
}

void Guy::InitGui()
{
	timeDirectionObject.InitGui();
	object.InitGui();
	mobileObject.InitGui();
	startTimeField.Init(startTime);
}
void Guy::UpdateGui()
{
	timeDirectionObject.UpdateGui();
	object.UpdateGui();
	mobileObject.UpdateGui();
	startTime = startTimeField.Update();
}

void Guy::DoDraw()
{
    if(object.IsFacingRight())
    {
        draw_sprite(buffer, guy_right_stop, object.GetXPos(), object.GetYPos());
        if(object.IsSelected())
        {
            rect(buffer,object.GetXPos(),object.GetYPos(),object.GetXPos()+WIDTH,object.GetYPos()+HEIGHT,makecol(255,0,0));
        }
    }
    else
    {
        draw_sprite(buffer, guy_left_stop, object.GetXPos(), object.GetYPos());
        if(object.IsSelected())
        {
            rect(buffer,object.GetXPos(),object.GetYPos(),object.GetXPos()+WIDTH,object.GetYPos()+HEIGHT,makecol(255,0,0));
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
	returnString += object.GetOutputStringParts();
	returnString += mobileObject.GetOutputStringParts();
	returnString += timeDirectionObject.GetOutputStringParts();
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
