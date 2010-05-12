/*
 *  Editor_Level.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Editor_Level.h"
#include "Editor_InputMap.h"
extern BITMAP* buffer;
//std::map<HourglassInput,SwitchingInput> inputs = InputMap::GetMap();
Level::Level() :
inputs(InputMap::GetMap()),
snapToGrid(true),
gridSize(16),
objectSelected(false),
selectedObject(-1),
canSelect(true),
nextObject(0),
objects(std::vector<Object*>())
{
    for(int x = 0; x < LEVEL_WIDTH; ++x)
    {
		for(int y = 0; y < LEVEL_HEIGHT; ++y)
		{
			if(x == 0 || y == 0 || x == LEVEL_WIDTH-1 || y == LEVEL_HEIGHT-1)
			{
				wall[x][y] = true;     
			}
			else {
				wall[x][y] = false;
			}
			
		}
    }
}
Level::~Level()
{
	while (objects.size() != 0)
	{
		delete objects.back();
		objects.pop_back();
	}
}

Level& Level::operator=(const Level& l) //TODO This was done in a hurry, check if I missed any data...
{
    snapToGrid = l.snapToGrid;
    gridSize = l.gridSize;
    objectSelected = l.objectSelected;
    selectedObject = l.selectedObject;
    canSelect = l.canSelect;
    nextObject = l.nextObject;
    objects = l.objects;
    for (unsigned int i = 0; i < objects.size();++i)
    {
        if (dynamic_cast<Box*>(objects.at(i)) != NULL)
        {
           objects.at(i) = new Box(*dynamic_cast<Box*>(objects.at(i)));
        }
        else if(dynamic_cast<Guy*>(objects.at(i)) != NULL) 
        {
          objects.at(i) = new Guy(*dynamic_cast<Guy*>(objects.at(i)));       
        }
    }
    for(int x = 0; x < LEVEL_WIDTH; ++x)
    {
		for(int y = 0; y < LEVEL_HEIGHT; ++y)
		{
			wall[x][y] = l.wall[x][y];
		}
    }
	return(*this);
}

void Level::SetWall(bool value,unsigned int x, unsigned int y) {
	wall[x][y] = value;
}

bool Level::GetWall(unsigned int x, unsigned int y) {
	return wall[x][y];
}

void Level::UpdateSelection()
{
	if (canSelect) {
		if (mouse_b & 1) {
			for(unsigned int i=0;i < objects.size();++i)
			{
				if(objects.at(i)->DoSelectionCheck())
				{
					for(unsigned int j=0;j < objects.size(); ++j)
					{
						if(j != i)
						{
							objects.at(j)->SetSelected(false);
						}
					}
					objectSelected = true;
					selectedObject = i;
					break;
				}
			}
		}
	}
}

void Level::UpdateSelected()
{
	if (objectSelected)
	{
		if (mouse_b & 2)
		{
			DoMoveSelected();
		}
		if(inputs[DELETE_OBJECT]())
		{
			DeleteSelectedObject();
		}
	} 
}

void Level::AddGuy(unsigned int xPos, unsigned int yPos, double xSpeed, double ySpeed, unsigned int startTime, AbsoluteTimeDirectionEnum atd)
{
	for (unsigned int i = 0; i < objects.size(); ++i) {
		if (dynamic_cast<Guy*> (objects.at(i)) != NULL) {
			delete objects.at(i);
			std::vector<Object*>::iterator it = objects.begin();
			it += i;
			objects.erase(it);
		}
	}
	objects.push_back(new Guy(xPos,yPos,xSpeed,ySpeed,startTime,atd));
}

void Level::AddBox(unsigned int xPos, unsigned int yPos, double xSpeed, double ySpeed)
{
	objects.push_back(new Box(xPos,yPos,xSpeed,ySpeed));
}

void Level::Draw()
{
	TestLevel(1);
	for(unsigned int i=0; i < objects.size(); ++i)
    {
        objects.at(i)->DoDraw();
    }	
}
void Level::DrawWall()
{
	TestLevel(1);
}
void Level::DrawObjects()
{
	for(unsigned int i=0; i < objects.size(); ++i)
    {
        objects.at(i)->DoDraw();
    }	
}
void Level::TestLevel(const double squareSize)
{
    rectfill( buffer, 0 , 0 , LEVEL_WIDTH * BLOCK_SIZE, LEVEL_HEIGHT * BLOCK_SIZE, makecol (255,255,255));
    for (int x = 0; x < LEVEL_WIDTH; ++x)
    {
        for (int y = 0; y < LEVEL_HEIGHT; ++y)
        {
            if (wall[x][y])
            {
                rectfill( buffer, static_cast<int>((x+(0.5-squareSize/2))*BLOCK_SIZE), 
						 static_cast<int>((y+(0.5-squareSize/2))*BLOCK_SIZE),
						 static_cast<int>((x+(0.5+squareSize/2))*BLOCK_SIZE), 
						 static_cast<int>((y+(0.5+squareSize/2))*BLOCK_SIZE), 
						 makecol ( 70, 70, 70));
            }
        }
    }
}

void Level::DoMoveSelected()
{
    int objectX = 0;
    int objectY = 0;
    if (snapToGrid)
    {
        objectX = static_cast<int>(gridSize*floor(static_cast<double>(mouse_x/gridSize)));
        objectY = static_cast<int>(gridSize*floor(static_cast<double>(mouse_y/gridSize)));
    }             
    else
    {
        objectX = static_cast<int>(floor(mouse_x));
        objectY = static_cast<int>(floor(mouse_y));
    }
    
    if(objectX <= LEVEL_WIDTH*BLOCK_SIZE-objects.at(selectedObject)->GetXSize() && 
	   objectY <= LEVEL_HEIGHT*BLOCK_SIZE-objects.at(selectedObject)->GetYSize())
    {
        objects.at(selectedObject)->SetPos(objectX,objectY);
    }
}

void Level::DeleteSelectedObject()
{
    // Maybe add checking whether an object is selected before deleting,
    //(although that can't happen with the current code)
	std::vector<Object*>::iterator it = objects.begin();
    it += selectedObject;
    delete objects.at(selectedObject);
    objects.erase(it);
    selectedObject = -1;
    objectSelected = false;
}

void Level::SetCanSelect(bool ICanSelect)
{
	canSelect = ICanSelect;
	//allegro_message("y0 %d",ICanSelect);
	if (ICanSelect == false) {
		if (objectSelected) {
			objects.at(selectedObject)->SetSelected(false);
			selectedObject = -1;
			objectSelected = false;
		}
	}
}

void Level::UpdateGUI()
{
	if (objectSelected)
	{
		objects.at(selectedObject)->DoGui();
	} 
}

Object* Level::GetNextObject()
{
	if (nextObject < objects.size())
	{
		++nextObject;
		return (objects.at(nextObject - 1));
	}
	else {
		return NULL;
	}

}
void Level::Reset()
{
	nextObject = 0;
}
void Level::SetGridSize(const unsigned int size)
{
    gridSize = size;
}
