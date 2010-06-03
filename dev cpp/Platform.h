#ifndef PLATFORM_H
#define PLATFORM_H
/*
 *  Platform.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 25/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
class Platform;
#include "StaticallyIdentifiable.h"
#include "TriggerEngine.h"
#include "TimeDirection.h"
#include <vector>
class Platform : StaticallyIdentifiable {
public:
	Platform(const unsigned int xPos, const unsigned int yPos, const unsigned int xSize, const unsigned int ySize, const unsigned int maxSpeed, const unsigned int acceleration, const TimeDirection direction);
	~Platform();

	void DoForwardsPhysicsStep(const int initialTime);
	void DoReversePhysicsStep(const int initialTime);
	
	void SetMaxSpeedSource(const int relativeTime, const int instructionIndex);
	void SetAccelerationSource(const int relativeTime, const int instructionIndex);
	void SetAimPositionXSource(const int relativeTime, const int instructionIndex);
	void SetAimPositionYSource(const int relativeTime, const int instructionIndex);
	void SetXPositionSource(const int relativeTime, const int instructionIndex);
	void SetYPositionSource(const int relativeTime, const int instructionIndex);

	int TS_GetMaxSpeed(const int time) const;
	int TS_GetAcceleration(const int time) const;
	int TS_GetAimPositionX(const int time) const;
	int TS_GetAimPositionY(const int time) const;
	int TS_GetXPosition(const int time) const;
	int TS_GetYPosition(const int time) const;
	int TS_GetXSize(const int time) const;
	int TS_GetYSize(const int time) const;
	int TS_GetXSpeed(const int time) const;
	int TS_GetYSpeed(const int time) const;
	
	void UpdateInstructions(int absoluteTime);

private:
	//Time dependent data
	unsigned int xPos_[];
	unsigned int yPos_[];
	unsigned int maxSpeed_[];
	unsigned int acceleration_[];
	unsigned int xSpeed_[];
	unsigned int ySpeed_[];
	unsigned int aimPositionX_[];
	unsigned int aimPositionY_[];
	
	//Static data
	const TimeDirection direction_;
	const unsigned int defaltXPos;
	const unsigned int defaltYPos;
	const unsigned int defaltMaxSpeed;
	const unsigned int defaultAcceleration;
	const unsigned int xSize_;
	const unsigned int ySize_;
};

#endif //PLATFORM_H
