#ifndef HOURGLASS_TIME_DIRECTION_OBJECT_H
#define HOURGLASS_TIME_DIRECTION_OBJECT_H
#include "Editor_Object.h"
#include "Editor_AbsoluteTimeDirectionEnum.h"
#include "Editor_DoubleField.h"

class TimeDirectionObject //: virtual public Object
{
public:
	TimeDirectionObject(const AbsoluteTimeDirectionEnum newATD = FORWARDS);
	~ TimeDirectionObject();
	void InitGui();
	void UpdateGui();
	std::string GetOutputStringParts();
private:
	AbsoluteTimeDirectionEnum direction;
	DoubleField startDirectionField; //TODO - do it properly, this is incredibly dodgy.
									 //ATM I just want to make compatibility and can't be bothered with coding 
									 //a drop-down menu or anything.
};
#endif //HOURGLASS_TIME_DIRECTION_OBJECT_H
