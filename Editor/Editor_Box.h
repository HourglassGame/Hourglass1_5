#ifndef HOURGLASS_BOX_H
#define HOURGLASS_BOX_H
#include "Editor_Object.h"
#include "Editor_ObjectSkeleton.h"
#include "Editor_MobileObject.h"
#include "Editor_TimeDirectionObject.h"
#include "Editor_AbsoluteTimeDirectionEnum.h"
#include <string>

class Box : public Object
{
public:
	Box(const int newXPos, const int newYPos, const double newXSpeed = 0, const double newYSpeed = 0, AbsoluteTimeDirectionEnum atd = FORWARDS);
	~Box();
	std::string GetOutputString();
	void SetPos(const int newXPos, const int newYPos);
	void SetSelected(const bool newSelected);
	void DoGui();
	bool DoSelectionCheck();
	void DoDraw();
	int GetXSize();
	int GetYSize();
	static const int WIDTH = 32;
	static const int HEIGHT = 32;
private:
	void InitGui();
	void UpdateGui();
	TimeDirectionObject timeDirectionObject;
	MobileObject mobileObject;
	ObjectSkeleton object;
};
#endif // BOX_H
