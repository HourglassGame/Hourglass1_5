#ifndef HOURGLASS_MOBILE_OBJECT_H
#define HOURGLASS_MOBILE_OBJECT_H
#include "Editor_Object.h"
#include "Editor_DoubleField.h"
class MobileObject : virtual public Object
{
protected:
	MobileObject(double newXSpeed = 0, double newYSpeed = 0);
	virtual~ MobileObject();
	void InitGui();
	void UpdateGui();
	std::string GetOutputStringParts();
private:
	double xSpeed;
	double ySpeed;
	DoubleField xSpeedField;
	DoubleField	ySpeedField;
};

#endif //HOURGLASS_MOBILE_OBJECT_H
