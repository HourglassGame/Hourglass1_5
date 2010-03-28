#include "Editor_LevelSaver.h"

// class constructor
LevelSaver::LevelSaver(){}
// class destructor
void LevelSaver::SaveLevel(Level* level, const std::string fileName)
{
	std::string filePath;
    filePath = fileName; //absolutePath + fileName;
	DoSaveLevel(level, fileName.c_str());
}

LevelSaver::~LevelSaver(){}

void LevelSaver::DoSaveLevel(Level* level, const std::string fileName)
{
	std::ofstream outputFile;
    outputFile.open(fileName.c_str());
    outputFile << "[WALL]\n";
    for (int y=0; y < LEVEL_HEIGHT; ++y)
    {
        for(int x=0;x < LEVEL_WIDTH; ++x)
        {
			if (level->GetWall(x,y))
			{
				outputFile.put('1');
			}
			else
			{
				outputFile.put('0'); 
			}
        }
        outputFile.put('\n');
    }
    outputFile << "<IMAGES>\n";
    outputFile << "</IMAGES>\n";
	Object* object;
	outputFile << "<GUYS>\n";
	while ((object = level->GetNextObject()) != NULL)
	{
		if (dynamic_cast<Guy*>(object) != NULL)
		{
			outputFile << (object->GetOutputString()).c_str() << "\n";
		}
	}
	outputFile << "</GUYS>\n";
	level->Reset();
	outputFile << "<BOXES>\n";
	while ((object = level->GetNextObject()) != NULL)
	{
		if (dynamic_cast<Box*>(object) != NULL)
		{
			outputFile << (object->GetOutputString()).c_str() << "\n";
		}
	}
	outputFile << "</BOXES>";
	level->Reset();
	outputFile.close();
}
