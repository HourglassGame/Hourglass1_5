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


Loader::Loader(const ResourcePathType rpt) :
pathToResourceDirectory(""),
#ifdef ALLEGRO_MACOSX
gameRelativeResourcePath("./"),
editorRelativeResourcePath("./")
#endif //ALLEGRO_MACOSX
#ifdef ALLEGRO_MINGW32
gameRelativeResourcePath("../dev cpp/resources/"),
editorRelativeResourcePath("./resources/")
#endif //ALLEGRO_MINGW32
{
    switch(rpt)
    {
        case GAME:
            pathToResourceDirectory = gameRelativeResourcePath;
            break;
        case EDITOR:
            pathToResourceDirectory = editorRelativeResourcePath;
            break;
        default:
            allegro_message("Shouldn't ever get here -\nImageLoader constructor switch got to default.");
            break;
	}
}

Loader::Loader(const std::string relativePath) :
pathToResourceDirectory(""),
#ifdef ALLEGRO_MACOSX
gameRelativeResourcePath("./"),
editorRelativeResourcePath("./")
#endif //ALLEGRO_MACOSX
#ifdef ALLEGRO_MINGW32
gameRelativeResourcePath("../dev cpp/resources/images/"),
editorRelativeResourcePath("./resources/images/")
#endif //ALLEGRO_MINGW32
{
	pathToResourceDirectory = relativePath;
}

// class destructor
Loader::~Loader()
{
}
