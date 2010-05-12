/*
 *  Editor_ObjectSkeleton.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 12/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "Editor_ObjectSkeleton.h"

// class constructor
ObjectSkeleton::ObjectSkeleton(const int newXPos,const int newYPos, const int newXSize, const int newYSize):
xPos(newXPos),
yPos(newYPos),
xSize(newXSize),
ySize(newYSize),
drawFacing(false),
selected(false),
xPositionField(IntField(100,628,10,1)),
yPositionField(IntField(100,648,10,1))
{ }
// class destructor
ObjectSkeleton::~ObjectSkeleton()
{ }

void ObjectSkeleton::DoGui()
{
	InitGui();
	UpdateGui();
}

void ObjectSkeleton::InitGui()
{
	xPositionField.Init(xPos);
	yPositionField.Init(yPos);
}
void ObjectSkeleton::UpdateGui()
{
	xPos = xPositionField.Update();
	yPos = yPositionField.Update();
}

void ObjectSkeleton::SetPos(int newXPos, int newYPos)
{
	xPos = newXPos;
	yPos = newYPos;
}

std::string ObjectSkeleton::GetOutputStringParts()
{
	std::stringstream ss;
	ss << "\n" << "X_POS=" << yPos << "\n" <<  "Y_POS=" << yPos;
	return(ss.str());
}

void ObjectSkeleton::SetSelected(bool newSelected)
{
	selected = newSelected;    
}

bool ObjectSkeleton::DoSelectionCheck()
{
	if(mouse_x > xPos && mouse_x < xPos + xSize && mouse_y > yPos && mouse_y < yPos + ySize)
	{
		selected = true;
		return(true);
	}
	return(false);
}

