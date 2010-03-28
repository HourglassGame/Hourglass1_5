#ifndef HOURGLASS_INT_FIELD_H
#define HOURGLASS_INT_FIELD_H
#include "Hourglass_Allegro.h"
#include "Editor_TextFieldTypeEnum.h"
#include "Editor_SwitchingInput.h"
#include "Editor_HourglassInputsEnum.h"
#include <map>
#include <string>
#include "Editor_InputField.h"
class IntField  : public InputField {
public:
	IntField(const int tLX,const int tLY,const int bRX,const int bRY);
	~IntField();
	
	void Init(const int& initialValue);
	int Update();
	
private:
	std::string readVal(int input);
};
#endif //HOURGLASS_INT_FIELD_H
