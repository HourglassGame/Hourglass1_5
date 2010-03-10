#ifndef HOURGLASS_IMAGELOADER_H
#define HOURGLASS_IMAGELOADER_H
#include <allegro.h>
#include <fstream>
#include "FileString.h"
#include "ImagePathEnum.h"
#include "Exceptions.h"

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
class ImageLoader
{
	public:
		// class constructor
		ImageLoader(ImagePathType ipt = GAME);
		ImageLoader(const FileString relativePath);
		BITMAP* LoadImage(const FileString fileName);
		// class destructor
		~ImageLoader();
	private:
        FileString absolutePath;
        void SetPath(const FileString relativePath);
        FileString GAME_RELATIVE_IMAGE_PATH;
        FileString EDITOR_RELATIVE_IMAGE_PATH;
};

#endif // IMAGELOADER_H
