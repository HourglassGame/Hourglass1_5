#include <allegro.h>

#include <direct.h> // for getcwd
#include <stdio.h>
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for strcat
#include <ctime> // for timing
#include <fstream> // for file I/O

// Relative time: steps that the player has experianced since the start of the game.
// Absolute time: steps since the start of the game.
int relativeTime;
int absoluteTime;

#include "Guy.h"
#include "MetaGuy.h"

Guy player;
MetaGuy metaguy;

using namespace std;

// timing
double elpased_time;
clock_t start_timer,finish_timer;

BITMAP* foreground;
BITMAP* guy_left;
BITMAP* guy_right;
BITMAP* guy_left_stop;
BITMAP* guy_right_stop;
BITMAP* buffer;

char CurrentPath[_MAX_PATH];
char levelPath[_MAX_PATH];
char imagePath[_MAX_PATH];

// wall segment count within level
const int level_width = 32;
const int level_height = 21;

// size of a wall segment
const int block_size = 32;

// wall array
bool wall[level_width][level_height];

void Draw()
{
    // draws the buffer to the screen
    acquire_screen();
    draw_sprite( screen, buffer, 0, 0);
    release_screen();
}

void StringAdd(char* string1, char* string2, char* newString)
{
     // (first string, string to add to end of first string, string to copy result to)
     // fixme: check for newString length
    int i;
    for (i=0;string1[i];i++) newString[i] = string1[i]; // copy string1 onto new string
    for (int j=0;string2[j];i++,j++) newString[i] = string2[j]; // copy string2 onto the end of new string
    newString[i] = 0; // terminate new string or the end will be full of junk
    
}

void LoadLevel(char* filePath)
{      
    
    // fixme: (level_height+1) should not be required
    ifstream inputFile;
    inputFile.open(filePath);
    char wallString[level_width*(level_height+1)];
    inputFile.getline(wallString, level_width*(level_height+1), '\n');
    //textout_ex( buffer, font, wallString, 150, 450, makecol( 255, 0, 0), makecol( 0, 0, 0) );

    // read wall array
    int i = 0;
    for (int x = 0; x < level_width; ++x,++i)
    {
        for (int y = 0; y < level_height; ++y,++i)
        {
            char temp = wallString[i];
            wall[x][y] = atoi(&temp); 
        }
    }
}

BITMAP* LoadImage(char* imageName)
{
    char tempPath[_MAX_PATH];
    StringAdd(imagePath,imageName,tempPath);
    BITMAP* tempBitmap;
    tempBitmap = load_bitmap(tempPath, NULL);
    if (!tempBitmap)
    {
        char tempString[_MAX_PATH+30];
        StringAdd("Error Loading image, path: ",tempPath,tempString);
        allegro_message(tempString, allegro_error);
    }
    return tempBitmap;
}

int main()
{

    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode( GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0); 
    // GFX_AUTODETECT as first param for fullscreen
    // GFX_AUTODETECT_WINDOWED as first param for windowed
    
    buffer = create_bitmap( 1024, 768); // create buffer, all drawing done to buffer
    
    // Get working directory and setup paths
    getcwd(CurrentPath, _MAX_PATH);
    char *levelPathName = "/resources/levels/";
    char *imagePathName = "/resources/images/";
    
    StringAdd(CurrentPath,levelPathName,levelPath);
    StringAdd(CurrentPath,imagePathName,imagePath);
    
    //load images
    foreground = LoadImage("testlevel.bmp");
    guy_left = LoadImage("rhino_left.bmp");
    guy_right = LoadImage("rhino_right.bmp");
    guy_left_stop = LoadImage("rhino_left_stop.bmp");
    guy_right_stop = LoadImage("rhino_right_stop.bmp");
    
    // how to do text: textout_ex( screen, font, "@", 50, 50, makecol( 255, 0, 0), makecol( 0, 0, 0) );
    
    draw_sprite( buffer, foreground, 0, 0); // move to somewhere else

    // load level 
    char tempPath[_MAX_PATH];
    StringAdd(levelPath,"testlevel.lvl",tempPath);
    LoadLevel(tempPath);//"C:/Dev-Cpp/Projects/time game/resources/levels/testlevel.lvl");
    
    // test loaded level
    /*
    for (int x = 0; x < level_width; ++x)
    {
        for (int y = 0; y < level_height; ++y)
        {
            //char testString[20];
            //sprintf(testString,"%d",wall[x][y]);
            //textout_ex( buffer, font, testString, x*block_size, y*block_size, makecol( 255, 0, 0), makecol( 0, 0, 0) );
            
            if (wall[x][y])
            {
               rectfill( buffer, (x+0.4)*block_size, (y+0.4)*block_size, (x+0.6)*block_size, (y+0.6)*block_size, makecol ( 70, 70, 70));
            } 
        }
    }
    */
    
    player.SetStart(double(200),double(200),0,0,0,0);
    
    // Game Loop 
    double step_interval = 0.029*CLOCKS_PER_SEC; // minimun time between steps
    start_timer = clock(); // timers for stable steps/second
    
    while( !key[KEY_ESC])
    { 
        finish_timer = clock();
        elpased_time = (double(finish_timer)-double(start_timer));
        
        if (elpased_time >= step_interval) 
        {
            start_timer = clock(); 
            
            // float to string:
            //arg3(string) = arg1(double) to string with arg2(int) figures.
            //gcvt(elpased_time, 10, testString);

            //blank the area of the buffer, faster than copying over entire foreground and background every frame, remember to implement fully
            rectfill( buffer, 100, 100, 250, 350, makecol ( 0, 0, 0));
            char testString[20];
            sprintf(testString,"%d",relativeTime);
            textout_ex( buffer, font, testString, 150, 200, makecol( 255, 0, 0), makecol( 0, 0, 0) ); // display elapsed frames to ensure the steps are happening at the correct speed
            
            metaguy.GetInput(relativeTime);
            
            player.ForwardTimeStep(absoluteTime);
            
            player.DrawSprite(absoluteTime);
            
            relativeTime++;
            absoluteTime++;
            
            Draw(); // draw buffer to screen
            
        }
    }  

    // remember to cleanup all bitmaps
    destroy_bitmap( foreground);
    destroy_bitmap( buffer);
    destroy_bitmap( guy_left);
    destroy_bitmap( guy_right);
    destroy_bitmap( guy_left_stop);
    destroy_bitmap( guy_right_stop);
    
    //readkey();
    
    return 0;
    
}   
END_OF_MAIN();


