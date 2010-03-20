/*
 *  Editor_LevelLoader.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef HOURGLASS_LEVEL_LOADER_H
#define HOURGLASS_LEVEL_LOADER_H
#include "Editor_Loader.h"
#include "Editor_Level.h"
//#include "FileString.hpp"
#include "Editor_ResourcePathEnum.h"
#include "Editor_Exceptions.h"
#include <fstream>
#include <map>
#include <string>
class LevelLoader : public Loader {
public:
	// class constructor
	LevelLoader(ResourcePathType ipt = GAME);
	LevelLoader(const std::string);
	
	Level* LoadLevel();
	Level* LoadLevel(const std::string);
	// class destructor
	~LevelLoader();	
private:
	static std::map<std::string,Level*> loadedLevels;
	Level* LoadLevelFromFile(const std::string filePath);
	static const unsigned int LEVEL_WIDTH = 32;
	static const unsigned int LEVEL_HEIGHT = 19;
	//static void Clear();
};

#endif // HOURGLASS_LEVEL_LOADER_H
