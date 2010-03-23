#ifndef HOURGLASS_GUY_H
#define HOURGLASS_GUY_H

#include "Editor_Object.h" // inheriting class's header file
#include "Editor_IntField.h"
#include "Editor_DoubleField.h"
#include "Editor_AbsoluteTimeDirectionEnum.h"
/*
 * No description
 */
class Guy : public Object
{
public:
	// class constructor
	Guy(int newXPos = 0, int newYPos = 0, double newXSpeed = 0, double newYSpeed = 0, unsigned int newStartTime = 2700, AbsoluteTimeDirectionEnum newATD = FORWARDS);
	// class destructor
	~Guy();
	void DoDraw();
	int GetType();
	int GetXSize();
	int GetYSize();
	void GetTimeData(unsigned int& startingTime, AbsoluteTimeDirectionEnum& atd);
	void DoGui();
private:
	static const int WIDTH = 24;
	static const int HEIGHT = 32;
	unsigned int startTime; //When the guy spawns
	AbsoluteTimeDirectionEnum direction;
	IntField startTimeField;
	DoubleField startDirectionField; //TODO - do it properly, this is incredibly dodgy.
									 //ATM I just want to make compatibility and can't be bothered with coding 
									 //a drop-down menu or anything.
};

#endif // GUY_H
