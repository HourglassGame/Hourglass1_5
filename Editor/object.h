#ifndef HOURGLASS_OBJECT_H
#define HOURGLASS_OBJECT_H
#include <allegro.h>

/*
 *     Parent class for things which can be added to levels by the editor.
 */

class Object
{
	public:
		Object(const int newXPos = 0,const int newYPos = 0,const double newXSpeed = 0,const double newYSpeed = 0);
		
        virtual ~Object();
        
	    void SetData(const int newXPos,const int newYPos,const double newXSpeed = 0,const double newYSpeed = 0);
        void SetPos(const int newXPos,const int newYPos);
        virtual void DoDraw() = 0;
		void GetData(int& outputXPos, int& outputYPos, double& outputXSpeed, double& outputYSpeed);
	    virtual int GetType() = 0; //Still needed for Level Saving. May be possible to remove, may not be.
	    void SetSelected(const bool newSelected = 0);
	    bool DoSelectionCheck();
	    virtual int GetXSize() = 0;
	    virtual int GetYSize() = 0;
    protected:
        int xPos;
        int yPos;
        double xSpeed;
        double ySpeed;
        bool drawFacing;
        bool selected;
};

#endif // OBJECT_H
