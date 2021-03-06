#ifndef HOURGLASS_KEYCOMBINATION_H
#define HOURGLASS_KEYCOMBINATION_H
#include "Editor_HourglassInputsEnum.h"
#include "Hourglass_Allegro.h"
/**
 * Holds a KEY_* value a key_shifts value and a RepeatType.
 */
class KeyCombination
{
	public:
		// class constructor
		KeyCombination(const int newKey,const RepeatType newType,const int newShift);
		KeyCombination(const int newKey, const RepeatType newType);
		// class destructor
		~KeyCombination();
		
		RepeatType GetType();
		void UpdatePressed();
		bool GetPressed();
		bool GetPastPressed();
	private:
        int hg_key;
        int modifiers;
        RepeatType type;
        bool pressed;
        bool pastPressed;
	    int GetKey();
		int GetShifts();
};
#endif // HOURGLASS_KEYCOMBINATION_H
