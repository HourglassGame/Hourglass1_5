/*
 *  Editor_LevelLoader.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Editor_LevelLoader.h"
extern int MAX_PATH;
std::map<std::string,Level*> LevelLoader::loadedLevels;

// class constructor
LevelLoader::LevelLoader(ImagePathType){}
LevelLoader::LevelLoader(const std::string){}

// class destructor
LevelLoader::~LevelLoader()
{
   // for (std::map<std::string,Level*>::iterator it = loadedLevels.begin(); loadedLevels.size() !=0;)
   // {
   //     delete it->second;
   //     loadedLevels.erase(it);
   //     it = loadedLevels.begin();
   // }
}

Level* LevelLoader::LoadLevel()
{
	//return(new Level());
	if (loadedLevels.find("__BlankLevel__") == loadedLevels.end()) {
		Level* newLevel = new Level();
		loadedLevels["__BlankLevel__"] = newLevel;
	}
	return loadedLevels["__BlankLevel__"];
}

Level* LevelLoader::LoadLevel(std::string fileName)
{
	std::string filePath = fileName;
	if (loadedLevels.find(filePath) == loadedLevels.end())
	{
		Level* newLevel;
		try
		{
			newLevel = LoadLevelFromFile(filePath);
		}
		catch (LevelNotLoadedException)
		{
			//const char whichImage = fileName.data();
			throw LevelNotLoadedException(fileName);//fileName.data());
		}
		catch (WallNotFoundException)
		{
			throw;
		}
		loadedLevels[filePath] = newLevel;
	}
	return loadedLevels[filePath];
}

Level* LevelLoader::LoadLevelFromFile(const std::string filePath)
{
	std::ifstream inputFile;
	inputFile.open(filePath.c_str());
	
	if (inputFile.is_open())
	{
		Level* newLevel = new Level();
		bool wallFound = false; //was "[WALL]" found in the file?
		const int MAX_LINE_LENGTH = 300; //MAX_LINE_LENGTH, Blasphomy! This is Madness! THIS IS SPARTAA!!!!!
		char line[MAX_LINE_LENGTH];
		std::string* gotLine = new std::string();
		while (!inputFile.eof())
		{   
			inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
			gotLine->assign(line);
			
			//		allegro_message("%s",gotLine->data());
			if (gotLine->compare(0,6,"[WALL]")==0) //Did I get "[WALL]"? Compare is actually designed for ordering strings alphabetically
			{
				wallFound = true;
				for (unsigned int i=0; i < LEVEL_HEIGHT; ++i)
				{
					//		//getline extracts - for getline(char* s,int n,char delim) - at most n-1 characters
					inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
					gotLine->assign(line);
					//				allegro_message("%s",gotLine->data());
					for (unsigned int j=0; j < LEVEL_WIDTH; ++j)
					{
						char temp[1] = {line[j]};
						newLevel->SetWall(atoi(temp), j, i);
					}
					
				}
			}
			else if (gotLine->compare(0,6,"<GUYS>")==0)
			{
				//			allegro_message("%s",gotLine->data());
				inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
				gotLine->assign(line);
				while (gotLine->compare(0,7,"</GUYS>")!= 0 && !inputFile.eof())
				{
					if (gotLine->compare(0,5,"<GUY>")==0)
					{
						std::map<std::string,std::string> guyData;
						inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
						gotLine->assign(line);
						//					allegro_message("%s",gotLine->data());
						while (gotLine->compare(0,6,"</GUY>")!=0 && !inputFile.eof())
						{
							std::string::size_type it = gotLine->find("=");
							guyData[gotLine->substr(0,it)] = gotLine->substr(it+1,gotLine->length()-(it+1));
							inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
							gotLine->assign(line);
							//						allegro_message("%s",gotLine->data());
						}
						int xPos = atoi(guyData["X_POS"].data());
						int yPos = atoi(guyData["Y_POS"].data());
						double xSpeed = atof(guyData["X_SPEED"].data());
						double ySpeed = atof(guyData["Y_SPEED"].data());
						//bool carryingBox = atoi(guyData["CARRYING_BOX"].data()); //need some kind of ascii to bool - this is ugly
						//int absTime = atoi(guyData["ABS_TIME"].data());
						//int relTime = atoi(guyData["REL_TIME"].data());
						newLevel->AddGuy(xPos,yPos,xSpeed,ySpeed);
					}
					inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
					gotLine->assign(line);
					//allegro_message("%s",gotLine->data());
				}
			}
			else if (gotLine->compare(0,7,"<BOXES>")==0) // Did I get "<BOXES>"
			{
				//	allegro_message("%s",gotLine->data());
				inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
				gotLine->assign(line);
				//	allegro_message("%s",gotLine->data());
				while (gotLine->compare(0,8,"</BOXES>") != 0 && !inputFile.eof()) //TODO add *better* error handling if eof reached without end symbol.
				{
					if (gotLine->compare(0,5,"<BOX>")==0)
					{
						std::map<std::string,std::string> boxData;
						inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
						gotLine->assign(line);
						//			allegro_message("%s",gotLine->data());
						while (gotLine->compare(0,6,"</BOX>")!=0 && !inputFile.eof())
						{
							std::string::size_type it = gotLine->find("=");
							boxData[gotLine->substr(0,it)] = gotLine->substr(it+1,gotLine->length()-(it+1));
							inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
							gotLine->assign(line);
							//				allegro_message("%s",gotLine->data());
						}
						int xPos = atoi(boxData["X_POS"].data());
						int yPos = atoi(boxData["Y_POS"].data());
						double xSpeed = atof(boxData["X_SPEED"].data());
						double ySpeed = atof(boxData["Y_SPEED"].data());
						//int absTime = atoi(boxData["ABS_TIME"].data());
						newLevel->AddBox(xPos, yPos, xSpeed, ySpeed);
					}
					inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
					gotLine->assign(line);
					//		allegro_message("%s",gotLine->data());
				}
			}
			
		}
		if ((wallFound) == false)
		{
			throw WallNotFoundException();
		}
		delete gotLine;
		inputFile.close();
		return newLevel;
	}
	else
	{
		throw FileNotOpenedException(); //could not open file
	}
}
