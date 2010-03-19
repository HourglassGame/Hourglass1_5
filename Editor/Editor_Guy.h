#ifndef HOURGLASS_GUY_H
#define HOURGLASS_GUY_H

#include "Editor_Object.h" // inheriting class's header file
#include "Editor_IntField.h"
#include "Editor_DoubleField.h"
/*
 * No description
 */
class Guy : public Object
{
public:
	// class constructor
	Guy(int newXPos = 0, int newYPos = 0, double newXSpeed = 0, double newYSpeed = 0);
	// class destructor
	~Guy();
	void DoDraw();
	int GetType();
	int GetXSize();
	int GetYSize();
private:
	static const int WIDTH = 24;
	static const int HEIGHT = 32;

};

#endif // GUY_H
