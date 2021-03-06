#ifndef HOURGLASS_LEVEL_SAVER_H
#define HOURGLASS_LEVEL_SAVER_H
#include "Hourglass_Allegro.h"
#include "Editor_ResourcePathEnum.h"
#include "Editor_AbsoluteTimeDirectionEnum.h"
#include <string>
#include <fstream>
#include "Editor_Level.h"
class LevelSaver {
public:
	// class constructor
	LevelSaver();
	// class destructor
	void SaveLevel(Level*, const std::string);
	virtual ~LevelSaver();
private:
	void DoSaveLevel(Level*, const std::string);
	static const int LEVEL_WIDTH = 32;
	static const int LEVEL_HEIGHT = 19;
};
#endif //HOURGLASS_LOADER_H
