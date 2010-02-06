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

// the time which propagation will stop at
int propagationAim;

#include "Guy.h"
#include "Box.h"

const double STEP_TIME = 0.029;

const int MAX_GUYS = 100;
const int MAX_BOXES = 100;

Guy guy[MAX_GUYS];
int guyCount; // number of guys 'created'
Box box[MAX_BOXES];
int boxCount; // number of boxes 'created'

//MetaGuy metaguy; // Stores input in relative time

using namespace std;

// timing
double elapsed_time;
clock_t start_timer,finish_timer;

// images
BITMAP* guy_left;
BITMAP* guy_right;
BITMAP* guy_left_stop;
BITMAP* guy_right_stop;
BITMAP* box_sprite;

BITMAP* foreground;
BITMAP* background;
BITMAP* buffer;

// file paths
char CurrentPath[_MAX_PATH]; // .exe path
char levelPath[_MAX_PATH]; // .lvl path
char imagePath[_MAX_PATH]; // .bmp path

// wall segment count within level
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 21;

// size of a wall segment
const int BLOCK_SIZE = 32;

// wall array
bool wall[LEVEL_WIDTH][LEVEL_HEIGHT];

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
    
    // fixme: (level_height+1) should not be required. Redo level format to fix when creating editor
    ifstream inputFile;
    inputFile.open(filePath);
    char wallString[LEVEL_WIDTH*(LEVEL_HEIGHT+1)];
    inputFile.getline(wallString, LEVEL_WIDTH*(LEVEL_HEIGHT+1), '\n');
    //textout_ex( buffer, font, wallString, 150, 450, makecol( 255, 0, 0), makecol( 0, 0, 0) );

    // read wall array
    int i = 0;
    for (int x = 0; x < LEVEL_WIDTH; ++x,++i)
    {
        for (int y = 0; y < LEVEL_HEIGHT; ++y,++i)
        {
            char temp = wallString[i];
            wall[x][y] = atoi(&temp); 
        }
    }
}

BITMAP* LoadImage(char* imageName)
{
    // loads a bitmap
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

    allegro_init(); // for all allegro functions
    install_keyboard(); // for keyboard use
    install_mouse();  // for mouse use
    show_os_cursor(MOUSE_CURSOR_ARROW); // display mouse, it is disabled by default with allegro
    set_color_depth(32);
    set_gfx_mode( GFX_AUTODETECT_FULLSCREEN, 1024, 768, 0, 0); 
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
    background = LoadImage("background.bmp");
    foreground = LoadImage("testlevel.bmp");
    guy_left = LoadImage("rhino_left.bmp");
    guy_right = LoadImage("rhino_right.bmp");
    guy_left_stop = LoadImage("rhino_left_stop.bmp");
    guy_right_stop = LoadImage("rhino_right_stop.bmp");
    box_sprite = LoadImage("box.bmp");
    
    // how to do text: textout_ex( screen, font, "@", 50, 50, makecol( 255, 0, 0), makecol( 0, 0, 0) );
    
    // Draw foreground and background
    draw_sprite( buffer, background, BLOCK_SIZE, BLOCK_SIZE); 
    draw_sprite( buffer, foreground, 0, 0); 

    // load level 
    char tempPath[_MAX_PATH];
    StringAdd(levelPath,"testlevel.lvl",tempPath);
    LoadLevel(tempPath);//"C:/Dev-Cpp/Projects/time game/resources/levels/testlevel.lvl");
    
    // test loaded level
    
    for (int x = 0; x < LEVEL_WIDTH; ++x)
    {
        for (int y = 0; y < LEVEL_HEIGHT; ++y)
        {
            //char testString[20];
            //sprintf(testString,"%d",wall[x][y]);
            //textout_ex( buffer, font, testString, x*block_size, y*block_size, makecol( 255, 0, 0), makecol( 0, 0, 0) );
            
            if (wall[x][y])
            {
               rectfill( buffer, int((x+0.4)*BLOCK_SIZE), int((y+0.4)*BLOCK_SIZE), int((x+0.6)*BLOCK_SIZE), int((y+0.6)*BLOCK_SIZE), makecol ( 70, 70, 70));
            } 
        }
    }
    
    
    box[boxCount].SetStart(double(300),double(200),0,0,0);
    box[boxCount].SetId(boxCount);
    boxCount = 1;
    
    box[boxCount].SetStart(double(400),double(200),0,0,0);
    box[boxCount].SetId(boxCount);
    boxCount++;
    
    box[boxCount].SetStart(double(380),double(120),0,0,0);
    box[boxCount].SetId(boxCount);
    boxCount++;
    
    box[boxCount].SetStart(double(800),double(160),-10,-2,0);
    box[boxCount].SetId(boxCount);
    boxCount++;
    
    box[boxCount].SetStart(double(680),double(60),0,5,0);
    box[boxCount].SetId(boxCount);
    boxCount++;
    
    box[boxCount].SetStart(double(420),double(270),0,0,0);
    box[boxCount].SetId(boxCount);
    boxCount++;
    
    box[boxCount].SetStart(double(380),double(160),0,0,0);
    box[boxCount].SetId(boxCount);
    boxCount++;
    
    box[boxCount].SetStart(double(300),double(160),0,-10,0);
    box[boxCount].SetId(boxCount);
    boxCount++;
    
    box[boxCount].SetStart(double(250),double(160),5,-5,0);
    box[boxCount].SetId(boxCount);
    boxCount++;
    
    box[boxCount].SetStart(double(360),double(80),0,0,0);
    box[boxCount].SetId(boxCount);
    boxCount++;
    
    guy[guyCount].SetStart(double(200),double(200),0,0,0,0);
    guyCount = 1;
    guy[0].SetOrder(guyCount);
   
    
    // Game Loop 
    double step_interval = STEP_TIME*CLOCKS_PER_SEC; // minimun time between steps
    start_timer = clock(); // timers for stable steps/second
    
    while( !key[KEY_ESC])
    { 
        finish_timer = clock();
        elapsed_time = (double(finish_timer)-double(start_timer));
        
        if (elapsed_time >= step_interval or propagationAim) 
        {
            start_timer = clock(); 
            
            // float to string:
            //arg3(string) = arg1(double) to string with arg2(int) figures.
            //gcvt(elpased_time, 10, testString);

            //blank the area of the buffer, faster than copying over entire foreground and background every frame, remember to implement fully
            rectfill( buffer, 100, 150, 250, 250, makecol ( 0, 0, 0));
            char testString[20];
            sprintf(testString,"%d",absoluteTime);
            textout_ex( buffer, font, testString, 150, 200, makecol( 255, 0, 0), makecol( 0, 0, 0) ); // display elapsed frames to ensure the steps are happening at the correct speed
            
            sprintf(testString,"%d",(mouse_x*3));
            textout_ex( buffer, font, testString, 150, 240, makecol( 255, 255, 0), makecol( 0, 0, 0) );
            
            // stop propagation if aim time is reached
            if (absoluteTime >= propagationAim)
            {
                propagationAim = 0;
            }
            
            // get input if not propagating
            if (!propagationAim)
            {
                Guy::StoreInput(relativeTime);
            }
            
            // order boxes from highest to lowest for easy collision checking, find a faster method that does not include many nested loops.
            
            int activeBoxes = 0;
            int activeBoxOrder[boxCount];
           
            sprintf(testString,"%d",activeBoxes);
            //allegro_message(testString, allegro_error);
            
            for (int i = 0; i < boxCount; ++i)
            {
                if (!box[i].GetCarried(absoluteTime) and absoluteTime > box[i].GetStartAbsTime() and (!box[i].GetEndAbsTime() or absoluteTime <= box[i].GetEndAbsTime() ))
                {
                    double boxY = box[i].GetY(absoluteTime-1);
                    activeBoxOrder[activeBoxes] = i;
                    for (int j = 0; j < activeBoxes; ++j)
                    {
                        if (box[activeBoxOrder[j]].GetY(absoluteTime-1) < boxY)
                        {
                            for (int k = activeBoxes; k > j; k = k-1)
                            {
                                activeBoxOrder[k] = activeBoxOrder[k-1];
                            }
                            activeBoxOrder[j] = i;
                            break;
                        }
                    }
                    activeBoxes++;
                }
                
                if (!propagationAim)
                {
                    box[i].unDrawSprite();
                }
            }
            
            // step through boxes in height order
            for (int i = 0; i < activeBoxes; ++i)
            {
                box[activeBoxOrder[i]].ForwardTimeStep(absoluteTime);
            }
            
            for (int i = 0; i < guyCount; ++i)
            {
                guy[i].ForwardTimeStep(absoluteTime);
                guy[i].UpdateBoxCarrying(absoluteTime);
                if (!propagationAim)
                {
                    guy[i].unDrawSprite();
                }
            }
            
            if (!propagationAim)
            {
                 for (int i = 0; i < boxCount; ++i)
                {
                    box[i].DrawSprite(absoluteTime);
                }
                for (int i = 0; i < guyCount; ++i)
                {
                    guy[i].DrawSprite(absoluteTime);
                }
                relativeTime++;
                Draw();
            }
            absoluteTime++;
            
            // draw buffer to screen
            
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


