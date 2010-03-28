#ifndef HOURGLASS_OBJECT_H
#define HOURGLASS_OBJECT_H
#include "Hourglass_Allegro.h"
#include "Editor_IntField.h"
#include <string>
#include <sstream>
/*
 *     Parent class for things which can be added to levels by the editor.
 */

class Object
{
public:
	Object(const int newXPos = 0,const int newYPos = 0);
	virtual ~Object();
	
	void SetPos(const int newXPos = 0,const int newYPos = 0);
	virtual void DoDraw() = 0;
	void DoGui();
	void SetSelected(const bool newSelected = 0);
	bool DoSelectionCheck();
	virtual std::string GetOutputString() = 0;
	virtual int GetXSize() = 0;
	virtual int GetYSize() = 0;
protected:
	std::string GetOutputStringParts();
	virtual void InitGui();
	virtual void UpdateGui();
	bool drawFacing;
	bool selected;
	int xPos;
	int yPos;
private:
	IntField xPositionField;
	IntField yPositionField;
};
#endif // OBJECT_H
