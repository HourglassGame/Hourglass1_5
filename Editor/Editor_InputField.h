#ifndef HOURGLASS_INPUT_FIELD_H
#define HOURGLASS_INPUT_FIELD_H
#include "Hourglass_Allegro.h"
#include "Editor_TextFieldTypeEnum.h"
#include "Editor_SwitchingInput.h"
#include "Editor_HourglassInputsEnum.h"
#include <map>
#include <string>
class InputField {
public:
	InputField(const int tLX,const int tLY,const int bRX,const int bRY);
	virtual ~InputField();
protected:
	static const int characterWidth = 8;
	static const int cursorWidth = 8;
	std::string currentValue;
	unsigned int cursorPosition;
	//virtual std::string readVal(int input);
	void TranslateInputs();
	int x0; //Top Left x
	int y0; //Top Left y
	int height; //Bottom Right x
	int width; //Bottom Right y
};
#endif //HOURGLASS_INPUT_FIELD_H
