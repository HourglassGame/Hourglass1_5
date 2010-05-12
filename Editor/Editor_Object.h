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
	Object(const int newXPos = 0, const int newYPos = 0, const int newXSize = 0, const int newYSize = 0);
	virtual ~Object() = 0;
	
	virtual void SetPos(const int newXPos, const int newYPos) = 0;
	virtual void DoDraw() = 0;
	virtual void DoGui() = 0;
	virtual void SetSelected(const bool newSelected) = 0;
	virtual bool DoSelectionCheck() = 0;
	virtual std::string GetOutputString() = 0;
	virtual int GetXSize() = 0;
	virtual int GetYSize() = 0;
};
#endif // OBJECT_H
