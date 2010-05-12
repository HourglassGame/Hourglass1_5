#ifndef EDITOR_OBJECT_SKELETON_H
#define EDITOR_OBJECT_SKELETON_H
/*
 *  Editor_ObjectSkeleton.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 12/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
#include <string>
#include <sstream>
#include "Editor_IntField.h"
class ObjectSkeleton {
public:
	ObjectSkeleton(const int newXPos = 0,const int newYPos = 0, const int newXSize = 0, const int newYSize = 0);
	~ObjectSkeleton();
	
	void SetPos(const int newXPos = 0,const int newYPos = 0);
	void SetSelected(const bool newSelected = 0);
	bool DoSelectionCheck();
	std::string GetOutputStringParts();
	void InitGui();
	void UpdateGui();
	int GetXPos();
	int GetYPos();
	bool IsFacingRight();
	bool IsSelected();
private:
	int xPos;
	int yPos;
	int xSize;
	int ySize;
	bool drawFacing;
	bool selected;
	
	IntField xPositionField;
	IntField yPositionField;
};
#endif //EDITOR_OBJECT_SKELETON_H
