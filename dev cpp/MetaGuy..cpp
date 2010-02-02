#include "MetaGuy.h"
/*
extern int relativeTime;
extern int absoluteTime;
*/
MetaGuy::MetaGuy()
{
}


void MetaGuy::GetInput(int time){
    inputLeft[time] = key[KEY_A];
    inputRight[time] = key[KEY_D];
    inputUp[time] = key[KEY_W];
    inputDown[time] = key[KEY_S];
}
