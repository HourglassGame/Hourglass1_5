/*
 *  Editor_Loader.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef HOURGLASS_LOADER_H
#define HOURGLASS_LOADER_H
#include <Allegro/Allegro.h>
#include "Editor_ImagePathEnum.h"
#include <string>
//#include "FileString.hpp"
class Loader {
public:
	// class constructor
	Loader(ImagePathType ipt = GAME);
	Loader(const std::string relativePath);
	// class destructor
	virtual ~Loader();
	
protected:
	std::string pathToResourceDirectory;
	//void SetPath(const std::string relativePath);
	std::string GAME_RELATIVE_IMAGE_PATH;
	std::string EDITOR_RELATIVE_IMAGE_PATH;
};

#endif //HOURGLASS_LOADER_H
