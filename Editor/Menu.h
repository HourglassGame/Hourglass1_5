#ifndef HOURGLASS_MENU_H
#define HOURGLASS_MENU_H
#include "allegro.h"
#include "ImageLoader.h"
/*
 * No description
 */
 
class Menu
{
	public:
		// class constructor
		Menu();
		// class destructor
		~Menu();
		void Draw();
		int Step();
	private:
        int menuPositionX;
        int menuPositionY;
        int noOfMenuItems;
        static const int MENU_WIDTH = 125;
        static const int MENU_HEIGHT = 50;
        BITMAP* menu_guy;
        BITMAP* menu_box;
};


#endif // MENU_H
