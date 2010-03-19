#ifndef HOURGLASS_BOX_H
#define HOURGLASS_BOX_H

#include "Editor_Object.h" // inheriting class's header file
#include "Editor_IntField.h"
#include "Editor_DoubleField.h"
#include <fstream>
/*
 * No description
 */
class Box : public Object
{
public:
	// class constructor
	Box(const int newXPos = 0, const int newYPos = 0, const double newXSpeed = 0, const double newYSpeed = 0);
	
	// class destructor
	~Box();
	
	void DoDraw();
	int GetType();
	int GetXSize();
	int GetYSize();
private:
	static const int WIDTH = 32;
	static const int HEIGHT = 32;
};

#endif // BOX_H
