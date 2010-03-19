/*
 *  Editor_Loader.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef HOURGLASS_LEVEL_SAVER_H
#define HOURGLASS_LEVEL_SAVER_H
#include <Allegro/Allegro.h>
#include "Editor_ImagePathEnum.h"
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
