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
#include "ImageLoader.h" // class's header file

// class constructor
ImageLoader::ImageLoader(const ImagePathType ipt)
{
    for(int i = 0; i < MAX_PATH; ++i)
    {
        absolutePath[1] = '\0';
    }
    GAME_RELATIVE_IMAGE_PATH = "/../dev cpp/resources/images/";
    EDITOR_RELATIVE_IMAGE_PATH = "/resources/images/";
    switch(ipt)
    {
        case GAME:
            SetPath(GAME_RELATIVE_IMAGE_PATH);
            break;
        case EDITOR:
            SetPath(EDITOR_RELATIVE_IMAGE_PATH);
            break;
        default:
            allegro_message("Shouldn't ever get here -\nImageLoader constructor switch got to default");
            break;
    }
}

ImageLoader::ImageLoader(const FileString relativePath)
{
    char exePath[MAX_PATH] = {'\0'};
    getcwd(exePath, _MAX_PATH);
	absolutePath = exePath + relativePath;
}

// class destructor
ImageLoader::~ImageLoader()
{
	// insert your code here
}

void ImageLoader::SetPath(const FileString relativePath)
{
    char exePath[MAX_PATH] = {'\0'};
    getcwd(exePath, MAX_PATH);
	absolutePath = exePath + relativePath;
}

BITMAP* ImageLoader::LoadImage(const FileString fileName)
{
    FileString filePath;
    filePath = absolutePath + fileName;
    BITMAP* newBitmap = load_bitmap(filePath.c_str(), NULL);
    if (!newBitmap)
    {
       throw ImageNotLoadedException();
    }
    return newBitmap;
}
