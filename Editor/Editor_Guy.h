#ifndef HOURGLASS_GUY_H
#define HOURGLASS_GUY_H

#include "Editor_Object.h"
#include "Editor_ObjectSkeleton.h"
#include "Editor_TimeDirectionObject.h"
#include "Editor_MobileObject.h"
#include "Editor_IntField.h"
#include "Editor_AbsoluteTimeDirectionEnum.h"
#include <string>

class Guy : public Object
{
public:
	Guy(const unsigned int newXPos,const unsigned int newYPos,const double newXSpeed = 0,const double newYSpeed = 0,const unsigned int newStartTime = 2700,const AbsoluteTimeDirectionEnum newATD = FORWARDS);
	~Guy();
	
	void SetPos(const int newXPos,const int newYPos);
	void DoGui();
	void SetSelected(const bool newSelected);
	bool DoSelectionCheck();
	void DoDraw();
	static const int WIDTH = 24;
	static const int HEIGHT = 32;
	int GetXSize();
	int GetYSize();
	std::string GetOutputString();
private:
	std::string GetOutputStringParts();
	unsigned int startTime; //When the guy spawns
	void InitGui();
	void UpdateGui();
	IntField startTimeField;
	TimeDirectionObject timeDirectionObject;
	MobileObject mobileObject;
	ObjectSkeleton object;
};
#endif // GUY_H
