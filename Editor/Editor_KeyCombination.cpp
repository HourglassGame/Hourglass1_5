#include "Editor_KeyCombination.h" // class's header file

// class constructor
/*
KeyCombination::KeyCombination(const int newKey,const int newShift,const RepeatType newType) :
    hg_key(newKey), modifiers(newShift), type(newType), pastPressed(false), pressed(false)
{

}
*/
KeyCombination::KeyCombination(const int newKey,const RepeatType newType,const int newShift) :
    hg_key(newKey), type(newType), modifiers(newShift), pastPressed(false), pressed(false)
{}

KeyCombination::KeyCombination(const int newKey,const RepeatType newType):
hg_key(newKey), modifiers(-1), type(newType), pastPressed(false), pressed(false)
{
             
}

// class destructor
KeyCombination::~KeyCombination()
{ }


int KeyCombination::GetKey()
{
    return(hg_key);                   
}
int KeyCombination::GetShifts()
{
    return(modifiers);                   
}
RepeatType KeyCombination::GetType()
{
    return(type);                   
}

void KeyCombination::UpdatePressed()
{
    pastPressed = pressed;
    //only key_shifts KB_SHIFT_FLAG | KB_CTRL_FLAG | KB_ALT_FLAG | KB_LWIN_FLAG | KB_RWIN_FLAG | KB_COMMAND_FLAG checked against.
	if (hg_key < KEY_MAX)
	{
		if(key[hg_key] && 
		   (((key_shifts & (KB_SHIFT_FLAG | KB_CTRL_FLAG | KB_ALT_FLAG | KB_LWIN_FLAG | KB_RWIN_FLAG | KB_COMMAND_FLAG)) == modifiers) ||
			modifiers == -1)) 
		{
			pressed = true;
		}
		else
		{
			pressed = false;
		}
	}
	else if (hg_key > KEY_MAX && hg_key <= KEY_MAX + 10)
	{
		int mousePressed = hg_key - KEY_MAX;
		if (mouse_b & mousePressed)
		{
			pressed = true;
		}
		else
		{
			pressed = false;
		}
	}
	else if (hg_key > KEY_MAX + 10)
	{
		//read from internal flags.
	}
}

bool KeyCombination::GetPressed()
{
    return(pressed);
}

bool KeyCombination::GetPastPressed()
{
    return(pastPressed);
}
