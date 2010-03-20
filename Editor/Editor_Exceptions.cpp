/*
 *  Exceptions.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 15/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Editor_Exceptions.h"

// class constructor
HourglassException::HourglassException(){}
// class destructor
HourglassException::~HourglassException(){}
std::string HourglassException::HourglassException::what()
{
	return (std::string("HourglassException"));
}

// class constructor
FileNotOpenedException::FileNotOpenedException(){}
// class destructor
FileNotOpenedException::~FileNotOpenedException(){}
std::string FileNotOpenedException::what()
{
	return (std::string("FileNotOpenedException"));
}

// class constructor
WallNotFoundException::WallNotFoundException(){}
// class destructor
WallNotFoundException::~WallNotFoundException(){}
std::string WallNotFoundException::what()
{
	return (std::string("WallNotFoundException"));
}

ImageNotLoadedException::ImageNotLoadedException() :
imageName("") {}
ImageNotLoadedException::ImageNotLoadedException(const std::string whatImage) :
imageName(whatImage) {}
//imageName(whatImage){}
// class destructor
ImageNotLoadedException::~ImageNotLoadedException(){}
std::string ImageNotLoadedException::what()
{
	return (std::string("ImageNotLoadedException"));
}
std::string ImageNotLoadedException::whichImage()
{
	return (imageName);
}

LevelNotLoadedException::LevelNotLoadedException() :
levelName("") {}
LevelNotLoadedException::LevelNotLoadedException(const std::string whatLevel) :
levelName(whatLevel)
{
}
// class destructor
LevelNotLoadedException::~LevelNotLoadedException(){}
std::string LevelNotLoadedException::what()
{
	return (std::string("LevelNotLoadedException"));
}
std::string LevelNotLoadedException::whichImage()
{
	return (levelName);
}

// class constructor
UndefinedObjectException::UndefinedObjectException(){}
// class destructor
UndefinedObjectException::~UndefinedObjectException(){}
std::string UndefinedObjectException::what()
{
	return (std::string("UndefinedObjectException"));
}

// class constructor
GFXModeNotSetException::GFXModeNotSetException(){}
// class destructor
GFXModeNotSetException::~GFXModeNotSetException(){}
std::string GFXModeNotSetException::what()
{
	return (std::string("GFXModeNotSetException"));
}

// class constructor
InvalidPathException::InvalidPathException(){}
// class destructor
InvalidPathException::~InvalidPathException(){}
std::string InvalidPathException::what()
{
	return (std::string("InvalidPathException"));
}

// class constructor
MaxPathLengthExceededException::MaxPathLengthExceededException(){}
// class destructor
MaxPathLengthExceededException::~MaxPathLengthExceededException(){}
std::string MaxPathLengthExceededException::what()
{
	return (std::string("MaxPathLengthExceededException"));
}
