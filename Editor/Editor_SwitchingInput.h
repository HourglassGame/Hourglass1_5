#ifndef HOURGLASS_SWITCHINGINPUT_H
#define HOURGLASS_SWITCHINGINPUT_H
#include <Allegro/Allegro.h>
#include <list>
#include "Editor_KeyCombination.h"
#include "Editor_HourglassInputsEnum.h"

/**
 * ----------------------------===Input===-----------------------------------
 * Holds a mapping between an ALLEGRO_KEY value and a unique keypress.
 * Calls to Add do shit
 * void Add(const int keyCode, const int shiftCode, const RepeatType repeatType);
 * void Add(const int keyCode, const RepeatType repeatType);
 *     Adds a new KeyCombination to the SwitchingInput, keycode is the aallegro keycode of the keypress, 
 *     shiftcode is the allegro shift flag (defaults to any flag, 
 *     passing argument of 0 means no flags may be true for the keypress to be true.
 *     (this is not quite true - some flags may be on (*LOCK_FLAG and KB_ACCENT*_FLAG and KB_INALTSEQ_FLAG)))
 * TODO - almost eveything in this class needs a better name - including the class itself );
 * TODO add exclusive vs includsive modifiers tag (for caps lock etc)
 * TODO maybe, add something so modifier must be pressed first (not sure how though)
 */
class SwitchingInput
{
public:
	// class constructor
	SwitchingInput();
	// class destructor
	~SwitchingInput();
	void Add(const int keyCode, const RepeatType repeatType, const int shiftCode);
	void Add(const int keyCode, const RepeatType repeatType);
	void ToggleOn();
	void ToggleOff();
	void Toggle();
	void Reset();
	void Update();
	bool GetCurrentValue();
private:
	std::list<KeyCombination*> assignedCombinations;
	bool currentValue; //Does this Input return true this step
	bool isToggledOn;
};

#endif // HOURGLASS_SWITCHINGINPUT_H
