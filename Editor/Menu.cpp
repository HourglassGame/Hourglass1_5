#include "menu.h" // class's header file
extern BITMAP* buffer;
extern BITMAP* tempBuffer;

extern bool pastKeyMouse1;
const int MENU_HEIGHT = 50;
// class constructor
Menu::Menu()
{
	menuPositionX = mouse_x;
    menuPositionY = mouse_y;
    ImageLoader imageLoader = ImageLoader(EDITOR);
    menu_box = imageLoader.LoadImage("add_box.bmp");
    menu_guy = imageLoader.LoadImage("add_guy.bmp");
    Draw();
}

// class destructor
Menu::~Menu()
{
	// insert your code here
}

void Menu::Draw()
{
    noOfMenuItems = 0;
    draw_sprite(buffer, menu_guy, menuPositionX, menuPositionY + noOfMenuItems*MENU_HEIGHT);
    noOfMenuItems++;
    draw_sprite(buffer, menu_box, menuPositionX, menuPositionY + noOfMenuItems*MENU_HEIGHT);
    noOfMenuItems++;             
}

int Menu::Step()
{
    if (mouse_b & 1)
    {
            pastKeyMouse1 = true; //TODO, remove requirement for this global, it is fail
            draw_sprite(buffer, tempBuffer, 0, 0);
        if (mouse_x > menuPositionX && 
            mouse_x < (menuPositionX + MENU_WIDTH) && 
            mouse_y > menuPositionY && 
            mouse_y < (menuPositionY + MENU_HEIGHT))
        {
            return(0);  // enter adding Guy mode
        }
        else if (mouse_x > menuPositionX &&
                mouse_x < (menuPositionX + MENU_WIDTH) &&
                mouse_y > (menuPositionY + MENU_HEIGHT) &&
                mouse_y < (menuPositionY + MENU_HEIGHT*2))
        {
            return(1);  // enter adding Box mode
        }
        else
        {
            return (-1); // continue, doing nothing
        }
    }
    else
    {
        return(-1); // continue, doing nothing
    }
}
