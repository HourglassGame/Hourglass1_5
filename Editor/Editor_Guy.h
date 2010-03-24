#ifndef HOURGLASS_GUY_H
#define HOURGLASS_GUY_H

#include "Editor_TimeDirectionObject.h" // inheriting class's header file
#include "Editor_MobileObject.h"
#include "Editor_IntField.h"
#include "Editor_AbsoluteTimeDirectionEnum.h"
#include <string>
/*
 * No description
 */
class Guy : public TimeDirectionObject, public MobileObject
{
public:
	// class constructor
	Guy(int newXPos = 0, int newYPos = 0, double newXSpeed = 0, double newYSpeed = 0, unsigned int newStartTime = 2700, AbsoluteTimeDirectionEnum newATD = FORWARDS);
	// class destructor
	~Guy();
	void DoDraw();
	int GetXSize();
	int GetYSize();
	std::string GetOutputString();
	void DoGui();
private:
	std::string GetOutputStringParts();
	static const int WIDTH = 24;
	static const int HEIGHT = 32;
	unsigned int startTime; //When the guy spawns
	void InitGui();
	void UpdateGui();
	IntField startTimeField;	
};

#endif // GUY_H
