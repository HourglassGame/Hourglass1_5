#ifndef HOURGLASS_LOADER_H
#define HOURGLASS_LOADER_H
#include "Hourglass_Allegro.h"
#include "Editor_ResourcePathEnum.h"
#include <string>

class Loader {
public:
	// class constructor
	Loader(ResourcePathType rpt = GAME);
	Loader(const std::string relativePath);
	// class destructor
	virtual ~Loader();
	
//public:
	std::string pathToResourceDirectory;
	std::string gameRelativeResourcePath;
	std::string editorRelativeResourcePath;
};
#endif //HOURGLASS_LOADER_H
