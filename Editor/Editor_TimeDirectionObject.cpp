#include "Editor_TimeDirectionObject.h"

TimeDirectionObject::TimeDirectionObject(AbsoluteTimeDirectionEnum newATD) :
	direction(newATD),
	startDirectionField(DoubleField(200,648,10,1))
{ }

TimeDirectionObject::~TimeDirectionObject()
{ }

void TimeDirectionObject::InitGui()
{
	startDirectionField.Init(static_cast<double>(direction));	
}
void TimeDirectionObject::UpdateGui()
{
	direction = static_cast<AbsoluteTimeDirectionEnum>(static_cast<int>(startDirectionField.Update()));
}
std::string TimeDirectionObject::GetOutputStringParts()
{
	std::stringstream ss;
	ss << "\n" << "START_DIRECTION=" << (direction == FORWARDS ? 1 : (direction == BACKWARDS ? -1 : 0));
	return(ss.str());
}
