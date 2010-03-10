#ifndef HOURGLASS_EXCPETIONS_H
#define HOURGLASS_EXCPETIONS_H
class HourglassException
{
   public:
		// class constructor
		HourglassException(){}
		// class destructor
		virtual ~HourglassException(){}
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

class GFXModeNotSetException : public HourglassException
{
	public:
		// class constructor
		GFXModeNotSetException(){}
		// class destructor
		~GFXModeNotSetException(){}
};

class InvalidPathException: public HourglassException
{
      public:
		// class constructor
		InvalidPathException(){}
		// class destructor
		virtual ~InvalidPathException(){}    
};
class MaxPathLengthExceededException : public InvalidPathException
{
	public:
		// class constructor
		MaxPathLengthExceededException(){}
		// class destructor
		~MaxPathLengthExceededException(){}
};


#endif
