/*
 *  Editor_Loader.cpp
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 17/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Editor_Loader.h"

/*
 ------------===constructor options===---------------
 
 ImageLoader()                 - selects game resource directory
 ImageLoader(ImagePathType)    - switches between known resource directories
 ImageLoader(FileString)       - selects custom relative resource directory
 
 -------------===variable names===-------------------
 
 /resources/images/                   - relativePath
 box.bmp                              - fileName
 C:\...                               - exePath
 C:\.../resources/images/             - absolutePath
 C:\.../resources/images/box.bmp      - filePath
 */
extern int MAX_PATH;

Loader::Loader(const ImagePathType ipt) :
pathToResourceDirectory(""),
#ifdef ALLEGRO_MACOSX
GAME_RELATIVE_IMAGE_PATH("./"),
EDITOR_RELATIVE_IMAGE_PATH("./")
#endif //ALLEGRO_MACOSX
#ifdef ALLEGRO_MINGW32
GAME_RELATIVE_IMAGE_PATH("../dev cpp/resources/images/"),
EDITOR_RELATIVE_IMAGE_PATH("./resources/images/")
#endif //ALLEGRO_MINGW32
{
    switch(ipt)
    {
        case GAME:
            pathToResourceDirectory = GAME_RELATIVE_IMAGE_PATH;
            break;
        case EDITOR:
            pathToResourceDirectory = EDITOR_RELATIVE_IMAGE_PATH;
            break;
        default:
            allegro_message("Shouldn't ever get here -\nImageLoader constructor switch got to default.");
            break;
	}
}

Loader::Loader(const std::string relativePath) :
pathToResourceDirectory(""),
#ifdef ALLEGRO_MACOSX
GAME_RELATIVE_IMAGE_PATH("./"),
EDITOR_RELATIVE_IMAGE_PATH("./")
#endif //ALLEGRO_MACOSX
#ifdef ALLEGRO_MINGW32
GAME_RELATIVE_IMAGE_PATH("../dev cpp/resources/images/"),
EDITOR_RELATIVE_IMAGE_PATH("./resources/images/")
#endif //ALLEGRO_MINGW32
{
	pathToResourceDirectory = relativePath;
}

// class destructor
Loader::~Loader()
{
}

//void Loader::SetPath(const std::string relativePath)
//{
	
//}

