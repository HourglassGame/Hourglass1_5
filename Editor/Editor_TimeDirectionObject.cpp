#include "Editor_TimeDirectionObject.h"

TimeDirectionObject::TimeDirectionObject(AbsoluteTimeDirectionEnum newATD) :
	direction(newATD),
	startDirectionField(DoubleField(200,648,10,1))
{ }

// class destructor
TimeDirectionObject::~TimeDirectionObject()
{ }

void TimeDirectionObject::InitGui()
{
	startDirectionField.Init(double((direction ==  FORWARDS) ? 1 : ((direction == BACKWARDS) ? -1 : 0)));	
}
void TimeDirectionObject::UpdateGui()
{
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
std::string TimeDirectionObject::GetOutputStringParts()
{
	std::stringstream ss;
	ss << "\n" << "START_DIRECTION=" << (direction == FORWARDS ? 1 : (direction == BACKWARDS ? -1 : 0));
	return(ss.str());
}
