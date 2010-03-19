/*
 *  Editor_DoubleField.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 18/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/*
 *  TextField.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef HOURGLASS_DOUBLE_FIELD_H
#define HOURGLASS_DOUBLE_FIELD_H
#include <Allegro/Allegro.h>
#include "Editor_TextFieldTypeEnum.h"
#include "Editor_SwitchingInput.h"
#include "Editor_HourglassInputsEnum.h"
#include <map>
#include <string>
#include "Editor_InputField.h"

class DoubleField  : public InputField {
public:
	DoubleField(const int tLX,const int tLY,const int bRX,const int bRY);
	~DoubleField();
	
	void Init(const double& initialValue);
	double Update();
	
private:
	std::string readVal(int input);
};

#endif //HOURGLASS_DOUBLE_FIELD_H
