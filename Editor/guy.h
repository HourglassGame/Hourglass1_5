// Class automatically generated by Dev-C++ New Class wizard

#ifndef GUY_H
#define GUY_H

#include <allegro.h>
/*
 * No description
 */
class Guy
{
	public:
		// class constructor
		Guy();
		// class destructor
		~Guy();
		
		void SetData(int newYPos, int newXPos, double newXSpeed, double newYSpeed);
		void DoDraw();
		void GetData(int& outputXPos, int& outputYPos, double& outputXSpeed, double& outputYSpeed);
	private:
        int xPos;
        int yPos;
        double xSpeed;
        double ySpeed;
        
        bool drawFacing;
};

#endif // GUY_H
