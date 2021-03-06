#ifndef HOURGLASS_IMAGELOADER_H
#define HOURGLASS_IMAGELOADER_H
#include "Hourglass_Allegro.h"
#include <fstream>
#include <map>
#include "Editor_ResourcePathEnum.h"
#include "Editor_Exceptions.h"

/* ------------===constructor options===---------------
 *   
 * ImageLoader()                 - selects game resource directory
 * ImageLoader(ImagePathType)    - switches between known resource directories
 * ImageLoader(FileString)       - selects custom relative resource directory
 *
 * -----------===Public Member Functions===------------
 * 
 * BITMAP* LoadImage(const FileString fileName) - Loads a BITMAP from file fileName in selected directory and returns a pointer to that BITMAP
 *
 * -------------===variable names===-------------------
 * 
 * /resources/images/                   - relativePath
 * box.bmp                              - fileName
 * C:\...                               - exePath
 * C:\.../resources/images/             - absolutePath
 * C:\.../resources/images/box.bmp      - filePath
 */
#include "Editor_Loader.h"
#include <string>
class ImageLoader
{
public:
	// class constructor
	ImageLoader(ResourcePathType rpt = GAME);
	ImageLoader(const std::string);
	BITMAP* LoadImage(const std::string);
	// class destructor
	~ImageLoader();
	
private:
	static std::map<std::string,BITMAP*> loadedImages;
	void DestroyImage(std::pair<std::string,BITMAP*> toDestroy);
	Loader loader;
};
#endif // IMAGELOADER_H
