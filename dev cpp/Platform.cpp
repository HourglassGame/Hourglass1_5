/*
 *  Platform.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 25/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "Platform.h"

Platform::Platform(const unsigned int xPos, const unsigned int yPos, const unsigned int xSize, const unsigned int ySize, const unsigned int maxSpeed, const unsigned int acceleration, const TimeDirection direction) :
direction_(direction),
defaltXPos(xPos),
defaltYPos(yPos),
defaltMaxSpeed(maxSpeed),
defaultAcceleration(acceleration),
xSize_(xSize),
ySize_(ySize)
{
}

Platform::~Platform(){}
