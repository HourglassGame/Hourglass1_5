#ifndef HOURGLASS_LEVEL_LOADER_H
#define HOURGLASS_LEVEL_LOADER_H
#include "Editor_Loader.h"
#include "Editor_Level.h"
#include "Editor_AbsoluteTimeDirectionEnum.h"
#include "Editor_ResourcePathEnum.h"
#include "Editor_Exceptions.h"
#include <fstream>
#include <map>
#include <string>
class LevelLoader : public Loader {
public:
	LevelLoader(ResourcePathType ipt = GAME);
	LevelLoader(const std::string);
	
	Level* LoadLevel();
	Level* LoadLevel(const std::string);
	~LevelLoader();	
private:
	static std::map<std::string,Level*> loadedLevels;
	Level* LoadLevelFromFile(const std::string filePath);
	static const unsigned int LEVEL_WIDTH = 32;
	static const unsigned int LEVEL_HEIGHT = 19;
	//static void Clear();
};
#endif // HOURGLASS_LEVEL_LOADER_H
