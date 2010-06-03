/*
 *  ValueHolder.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 26/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
/**
 * Transports a value through time. ForwardsPhysicsStep does value(t) = value(t-1), ReversePhysicsStep does value(t) = value(t+1).
 * Performs function which could be hacked up using platforms 
 * (there is no excuse for doing it with platforms given ValueHolder exists).
 */

#include "StaticallyIdentifiable.h"
class ValueHolder : public StaticallyIdentifiable {
public:
	void DoForwardsPhysicsStep(const int initialTime);
	void DoReversePhysicsStep(const int initialTime);
};
