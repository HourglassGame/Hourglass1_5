#include "MetaGuy.h"
/*
extern int relativeTime;
extern int absoluteTime;
*/
bool leftMousePressed;

MetaGuy::MetaGuy()
{
}


void MetaGuy::GetInput(int time){
    inputLeft[time] = key[KEY_A];
    inputRight[time] = key[KEY_D];
    inputUp[time] = key[KEY_W];
    inputDown[time] = key[KEY_S];
    
    if (mouse_b & 1) // left mouse is pressed
    {
        if (!leftMousePressed)
        {
            inputSpecial[time] = 1;
            inputSpecialArg1[time] = mouse_x*3;
        }
        leftMousePressed = true;
    }
    else
    {
        leftMousePressed = false;
    }
}
