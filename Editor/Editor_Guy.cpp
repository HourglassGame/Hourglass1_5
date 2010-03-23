#include "Editor_Guy.h" // class's header file
extern BITMAP* buffer;
extern BITMAP* guy_left_stop;
extern BITMAP* guy_right_stop;
// class constructor

Guy::Guy(const int newXPos, const int newYPos, const double newXSpeed, const double newYSpeed, unsigned int newStartTime, AbsoluteTimeDirectionEnum newATD) :
    Object(newXPos,newYPos,newXSpeed,newYSpeed),
	startTime(newStartTime),
	direction(newATD),
	startTimeField(IntField(200,628,10,1)),
	startDirectionField(DoubleField(200,648,10,1))
{ }
// class destructor
Guy::~Guy()
{ }

void Guy::DoGui()
{
	xPositionField.Init(xPos);
	yPositionField.Init(yPos);
	xSpeedField.Init(xSpeed);
	ySpeedField.Init(ySpeed);
	startTimeField.Init(startTime);
	startDirectionField.Init(double((direction ==  FORWARDS) ? 1 : ((direction == BACKWARDS) ? -1 : 0)));
	xPos = xPositionField.Update();
	yPos = yPositionField.Update();
	xSpeed = xSpeedField.Update();
	ySpeed = ySpeedField.Update();
	startTime = startTimeField.Update();
	switch (int(startDirectionField.Update())) {
		case 1:
			direction = FORWARDS;
			break;
		case 0:
			direction = PAUSED;
			break;
		case -1:
			direction = BACKWARDS;
			break;
		default:
			//allegro_message("startDirectionField.Update() fell through to default");
			break;
	}
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

void Guy::GetTimeData(unsigned int& startingTime, AbsoluteTimeDirectionEnum& atd)
{
	startingTime = startTime;
	atd = direction;
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
