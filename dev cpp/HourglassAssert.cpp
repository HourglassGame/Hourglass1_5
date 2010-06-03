/*
 *  AllegroAssert.cpp
 *  HourglassIIGame
 *
 *  Created by Evan Wallace on 1/06/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#include "HourglassAssert.h"
#include "Hourglass_Allegro.h"
#include <string>
#include <sstream>
void boost::assertion_failed(char const * expr, char const * function, char const * file, long line) {
	allegro_init();
	std::stringstream message;
	message << "<---Assertion Failed--->" << std::endl;
	message << "Assertion: " << "(" << expr << ")" << std::endl;
	message << "Function: " << function << std::endl;
	message << "File: " << file << std::endl;
	message << "Line: " << line;
	allegro_message("%s",message.str().c_str());
	exit(-10);
}
