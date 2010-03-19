/*
 *  Exceptions.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 15/03/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */

#ifndef HOURGLASS_EXCPETIONS_H
#define HOURGLASS_EXCPETIONS_H
#include <allegro/allegro.h>
#include <string>
//#include "FileString.hpp"
class HourglassException
{
public:
	// class constructor
	HourglassException();
	// class destructor
	virtual ~HourglassException();
	virtual std::string what();
};

class FileNotOpenedException : public HourglassException
{
public:
	// class constructor
	FileNotOpenedException();
	// class destructor
	~FileNotOpenedException();
	virtual std::string what();
};

class WallNotFoundException : public HourglassException
{
public:
	// class constructor
	WallNotFoundException();
	// class destructor
	~WallNotFoundException();
	virtual std::string what();
};

class ImageNotLoadedException : public HourglassException
{
public:
	// class constructor
	ImageNotLoadedException();
	ImageNotLoadedException(const std::string whatImage);
	//imageName(whatImage){}
	// class destructor
	~ImageNotLoadedException();
	virtual std::string what();
	virtual std::string whichImage();
private:
	std::string imageName;
};

class LevelNotLoadedException : public HourglassException
{
public:
	// class constructor
	LevelNotLoadedException();
	LevelNotLoadedException(const std::string whatImage);
	//imageName(whatImage){}
	// class destructor
	~LevelNotLoadedException();
	virtual std::string what();
	virtual std::string whichImage();
private:
	std::string levelName;
};

class UndefinedObjectException : public HourglassException
{
public:
	// class constructor
	UndefinedObjectException();
	// class destructor
	~UndefinedObjectException();
	virtual std::string what();
};

class GFXModeNotSetException : public HourglassException
{
public:
	// class constructor
	GFXModeNotSetException();
	// class destructor
	~GFXModeNotSetException();
	virtual std::string what();
};

class InvalidPathException: public HourglassException
{
public:
	// class constructor
	InvalidPathException();
	// class destructor
	virtual ~InvalidPathException();
	virtual std::string what();
};
class MaxPathLengthExceededException : public InvalidPathException
{
public:
	// class constructor
	MaxPathLengthExceededException();
	// class destructor
	~MaxPathLengthExceededException();
	virtual std::string what();
};


#endif
