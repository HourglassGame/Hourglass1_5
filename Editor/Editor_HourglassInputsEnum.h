#ifndef HOURGLASS_INPUTS_ENUM_H
#define HOURGLASS_INPUTS_ENUM_H
enum HourglassInput
{
	ADD_ADD_OBJECT_MENU,
	REMOVE_ADD_OBJECT_MENU,
	DO_LOAD_LEVEL_DIALOG,
	DO_SAVE_LEVEL_DIALOG,
	DRAW_WALL,
	ADD_WALL,
	DELETE_WALL,
	ADD_OBJECT,
	DELETE_OBJECT,
	EXIT_EDITOR,
	LEAVE_INPUT_FIELD
};
enum RepeatType
{
    PRESS,
    RELEASE,
    TOGGLE_ON_PRESS,
    TOGGLE_ON_RELEASE,
    HOLD
};
#endif // HOURGLASS_INPUTS_ENUM_H