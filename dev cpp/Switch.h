/*
 *  Switch.h
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 1/06/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
#include "StaticallyIdentifiable.h"
class Switch : public StaticallyIdentifiable {
public:
	void DoForwardsPhysicsStep(const int initialTime);
	void DoReversePhysicsStep(const int initialTime);
	bool GetState(const int time);
};
