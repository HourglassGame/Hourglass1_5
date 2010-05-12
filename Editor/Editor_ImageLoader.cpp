/*
    ------------===constructor options===---------------
    
    ImageLoader()                 - selects game resource directory
    ImageLoader(ImagePathType)    - switches between known resource directories
    ImageLoader(std::string)       - selects custom relative resource directory
    
    -------------===variable names===-------------------
    
    /resources/images/                   - relativePath
    box.bmp                              - fileName
    C:\...                               - exePath
    C:\.../resources/images/             - absolutePath
    C:\.../resources/images/box.bmp      - filePath
*/
#include "Editor_ImageLoader.h" // class's header file

std::map<std::string,BITMAP*> ImageLoader::loadedImages;
ImageLoader::ImageLoader(const ResourcePathType rpt) :
loader(Loader(rpt))
{
#ifdef ALLEGRO_MINGW32
    loader.pathToResourceDirectory += "images/";
#endif
}

ImageLoader::ImageLoader(const std::string str) :
loader(Loader(str))
{
}

ImageLoader::~ImageLoader()
{
	//delete loader;
}

BITMAP* ImageLoader::LoadImage(const std::string fileName)
{
	std::string filePath;
    filePath = loader.pathToResourceDirectory + fileName;
	if (loadedImages.find(filePath) == loadedImages.end()) {
		BITMAP* newBitmap = load_bitmap(filePath.c_str(), NULL);
		if (!newBitmap)
		{
			//const char whichImage = fileName.data();
			throw ImageNotLoadedException(fileName);//fileName.data());
		}
		loadedImages[filePath] = newBitmap;
	}
	return loadedImages[filePath];
}
