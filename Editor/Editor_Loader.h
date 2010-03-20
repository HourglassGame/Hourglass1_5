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
#include "Hourglass_Allegro.h"
#include "Editor_ResourcePathEnum.h"
#include <string>
//#include "FileString.hpp"
class Loader {
public:
	// class constructor
	Loader(ResourcePathType rpt = GAME);
	Loader(const std::string relativePath);
	// class destructor
	virtual ~Loader();
	
protected:
	std::string pathToResourceDirectory;
	std::string gameRelativeResourcePath;
	std::string editorRelativeResourcePath;
};

#endif //HOURGLASS_LOADER_H
