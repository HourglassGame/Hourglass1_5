#include "Editor_Menu.h" // class's header file
extern std::map<HourglassInput,SwitchingInput> inputs;
extern BITMAP* buffer;
extern BITMAP* tempBuffer;
//extern bool pastKeyMouse1;
const int MENU_HEIGHT = 50;
// class constructor
Menu::Menu() :
menu_box(NULL),
menu_guy(NULL),
menuPositionX(mouse_x),
menuPositionY(mouse_y),
noOfMenuItems(0),
firstStep(true)
{
    try {
        ImageLoader imageLoader = ImageLoader(EDITOR);
        menu_box = imageLoader.LoadImage("add_box.bmp");
        menu_guy = imageLoader.LoadImage("add_guy.bmp");
    }
    catch (ImageNotLoadedException& e)
    {
        allegro_message("Unable to load %s",e.whichImage().data());
        throw; // Failed to load images
    }
    Draw();
}

// class destructor
Menu::~Menu()
{
	// insert your code here
}

Menu::Menu(const Menu& m) :
menuPositionX(m.menuPositionX),
menuPositionY(m.menuPositionY),
noOfMenuItems(m.noOfMenuItems),
menu_guy(m.menu_guy), //These are pointers, so be careful, the new one can change the old one
menu_box(m.menu_box), // however, Menus do not do any modifying of their images as of yet, so it is ok.
firstStep(true)
{}

Menu& Menu::operator=(const Menu& m)
{
	menuPositionX = m.menuPositionX;
	menuPositionY = m.menuPositionY;
	noOfMenuItems = m.noOfMenuItems;
	menu_guy = m.menu_guy;
	menu_box = m.menu_box;
	return(*this);
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
	if (!firstStep && inputs[REMOVE_ADD_OBJECT_MENU].GetCurrentValue()) {// || inputs[ADD_ADD_OBJECT_MENU].GetCurrentValue()) {
		return 0;
	}
	firstStep = false;
    if (mouse_b & 1)
    {
            draw_sprite(buffer, tempBuffer, 0, 0);
        if (mouse_x > menuPositionX && 
            mouse_x < (menuPositionX + MENU_WIDTH) && 
            mouse_y > menuPositionY && 
            mouse_y < (menuPositionY + MENU_HEIGHT))
        {
            return(1);  // enter adding Guy mode
        }
        else if (mouse_x > menuPositionX &&
                mouse_x < (menuPositionX + MENU_WIDTH) &&
                mouse_y > (menuPositionY + MENU_HEIGHT) &&
                mouse_y < (menuPositionY + MENU_HEIGHT*2))
        {
            return(2);  // enter adding Box mode
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
