#include "Editor_DoubleField.h"
#include "Editor_InputMap.h"
extern BITMAP* buffer;
//std::map<HourglassInput,SwitchingInput> inputs = InputMap::GetMap();
DoubleField::DoubleField(const int tLX,const int tLY,const int Width,const int Height) :
InputField(tLX,tLY,Width,Height)
{}

DoubleField::~DoubleField()
{}
void DoubleField::Init(const double& initialValue)
{
	char initialString[100];
	sprintf(initialString, "%f", initialValue);
	currentValue = std::string(initialString);
	textout_ex(buffer, font, currentValue.c_str(), x0, y0, makecol(255, 0, 0), makecol(255, 255, 255));
	cursorPosition = currentValue.size();
	rectfill(buffer, x0+cursorPosition*characterWidth, y0,x0+cursorPosition*characterWidth + (cursorWidth - 1), y0 + height*(text_height(font)-1), makecol(255, 255, 0));	
}
double DoubleField::Update()
{
	clear_keybuf();
	if ((mouse_b & 1) && (mouse_x > x0 && mouse_x < x0+text_length(font, currentValue.c_str()) && mouse_y > y0 && mouse_y < y0+height*text_height(font))) {
		while (!(((mouse_b & 1) && (mouse_x < x0 || mouse_x > x0+text_length(font, currentValue.c_str())|| mouse_y < y0 || mouse_y > y0+height*text_height(font)))
				 || inputs[LEAVE_INPUT_FIELD]())) {
			poll_mouse();
			poll_keyboard();
			TranslateInputs();
			while (keypressed()) {
				int pointPos = currentValue.find(".");
				int val = readkey();
				if ((val >> 8) == KEY_LEFT) {
					if (cursorPosition > 0) {
						--cursorPosition;
					}
				}
				else if ((val >> 8) == KEY_RIGHT) {
					if (cursorPosition < currentValue.size()) {
						++cursorPosition;
					}
				}
				else if ((val >> 8) == KEY_BACKSPACE) {
					if (cursorPosition > 0) {
						currentValue.erase(cursorPosition-1,1);
						--cursorPosition;
					}
				}
				else if ((val >> 8) == KEY_DEL) {
					if (cursorPosition < currentValue.size()) {
						currentValue.erase(cursorPosition,1);
					}
				}
				else {
					std::string newChar(readVal(val));
					if (newChar != "" && newChar != "-") {
						if (newChar == ".") {
							if (pointPos != -1) {
								currentValue.erase(pointPos,1);
								--cursorPosition;
							}
						}
						
						currentValue.insert(cursorPosition, newChar);
						++cursorPosition;
					}
					else if (newChar == "-") {
						if ((double)atof(currentValue.c_str()) < 0) {
							currentValue.erase(0,1);
							--cursorPosition;
						}
						else {
							currentValue.insert(0,newChar);
							++cursorPosition;
						}

					}
				}
				rectfill(buffer, x0, y0, x0+characterWidth*width, y0+height*text_height(font), makecol(0, 0, 0));
				textout_ex(buffer, font, currentValue.c_str(), x0, y0, makecol(255, 0, 0), makecol(255, 255, 255));
				rectfill(buffer, x0+cursorPosition*characterWidth, y0,x0+cursorPosition*characterWidth + (cursorWidth - 1), y0 + height*(text_height(font)-1), makecol(255, 255, 0));
				scare_mouse();
                acquire_screen();
				draw_sprite(screen, buffer, 0, 0);
				release_screen();
				unscare_mouse();
			}
			rest(1);
		}
	}
	return (double)atof(currentValue.c_str());
}
std::string DoubleField::readVal(int input)
{
	std::string output("");
	if ((input & 0xff) == '0') {
		output.assign("0");
	}
	else if ((input & 0xff) == '1') {
		output.assign("1");
	}
	else if ((input & 0xff) == '2') {
		output.assign("2");
	}
	else if ((input & 0xff) == '3') {
		output.assign("3");
	}
	else if ((input & 0xff) == '4') {
		output.assign("4");
	}
	else if ((input & 0xff) == '5') {
		output.assign("5");
	}
	else if ((input & 0xff) == '6') {
		output.assign("6");
	}
	else if ((input & 0xff) == '7') {
		output.assign("7");
	}
	else if ((input & 0xff) == '8') {
		output.assign("8");
	}
	else if ((input & 0xff) == '9') {
		output.assign("9");
	}
	else if ((input & 0xff) == '.') {
		output.assign(".");
	}
	else if ((input & 0xff) == '-') {
		output.assign("-");
	}
	return output;
}
