#ifndef HOURGLASS_EXCPETIONS_H
#define HOURGLASS_EXCPETIONS_H
class HourglassException
{
   public:
		// class constructor
		HourglassException(){}
		// class destructor
		~HourglassException(){}
};

class FileNotOpenedException : public HourglassException
{
	public:
		// class constructor
		FileNotOpenedException(){}
		// class destructor
		~FileNotOpenedException(){}
};

class WallNotFoundException : public HourglassException
{
	public:
		// class constructor
		WallNotFoundException(){}
		// class destructor
		~WallNotFoundException(){}
};

class ImageNotLoadedException : public HourglassException
{
	public:
		// class constructor
		ImageNotLoadedException(){}
		// class destructor
		~ImageNotLoadedException(){}
};

class UndefinedObjectException : public HourglassException
{
	public:
		// class constructor
		UndefinedObjectException(){}
		// class destructor
		~UndefinedObjectException(){}
};
#endif
