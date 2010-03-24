#ifndef HOURGLASS_BOX_H
#define HOURGLASS_BOX_H

#include "Editor_TimeDirectionObject.h" // inheriting class's header file
#include "Editor_MobileObject.h"
#include "Editor_AbsoluteTimeDirectionEnum.h"
#include <string>
/*
 * No description
 */
class Box : public TimeDirectionObject, public MobileObject
{
public:
	// class constructor
	Box(const int newXPos = 0, const int newYPos = 0, const double newXSpeed = 0, const double newYSpeed = 0, AbsoluteTimeDirectionEnum atd = FORWARDS);
	
	// class destructor
	~Box();
	std::string GetOutputString();
	void DoDraw();
	int GetXSize();
	int GetYSize();
	void DoGui();
private:
	static const int WIDTH = 32;
	static const int HEIGHT = 32;
	void InitGui();
	void UpdateGui();
};

#endif // BOX_H
