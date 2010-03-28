#include "Editor_MobileObject.h"

MobileObject::MobileObject(const double newXSpeed,const double newYSpeed) :
xSpeed(newXSpeed),
ySpeed(newYSpeed),
xSpeedField(DoubleField(100,668,10,1)),
ySpeedField(DoubleField(100,688,10,1))
{ }

// class destructor
MobileObject::~MobileObject()
{ }

void MobileObject::InitGui()
{
	xSpeedField.Init(xSpeed);
	ySpeedField.Init(ySpeed);
}
void MobileObject::UpdateGui()
{
	xSpeed = xSpeedField.Update();
	ySpeed = ySpeedField.Update();
}

std::string MobileObject::GetOutputStringParts()
{
	std::stringstream ss;
	ss << "\n" << "X_SPEED=" << xSpeed << "\n" << "Y_SPEED=" << ySpeed;
	return(ss.str());
}
