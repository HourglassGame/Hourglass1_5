/*
 *  Editor_LevelSaver.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

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
			outputFile << "<GUY>\n";
			int xPos;
			int yPos;
			double xSpeed;
			double ySpeed;
			object->GetData(xPos, yPos, xSpeed, ySpeed);
			outputFile << "X_POS=" << xPos << '\n';
			outputFile << "Y_POS=" << yPos << '\n';
			outputFile << "X_SPEED=" << xSpeed << '\n';
			outputFile << "Y_SPEED=" << ySpeed << '\n';
			outputFile << "</GUY>\n";
		}
	}
	outputFile << "</GUYS>\n";
	level->Reset();
	outputFile << "<BOXES>\n";
	while ((object = level->GetNextObject()) != NULL)
	{
		if (dynamic_cast<Box*>(object) != NULL)
		{
			outputFile << "<BOX>\n";
			int xPos;
			int yPos;
			double xSpeed;
			double ySpeed;
			object->GetData(xPos, yPos, xSpeed, ySpeed);
			outputFile << "X_POS=" << xPos << '\n';
			outputFile << "Y_POS=" << yPos << '\n';
			outputFile << "X_SPEED=" << xSpeed << '\n';
			outputFile << "Y_SPEED=" << ySpeed << '\n';
			outputFile << "</BOX>\n";
		}
	}
	outputFile << "</BOXES>";
	level->Reset();
	outputFile.close();
}
