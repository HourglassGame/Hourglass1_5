#include <allegro.h>



#include <direct.h> // for getcwd
#include <stdio.h>
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for strcat
#include <ctime> // for timing
#include <fstream> // for file I/O
#include <string> // for strings
#include <map> //used in LoadLevel

#include "Guy.h"
#include "Box.h"
#include "PropManager.h"

#include "Exceptions.h" //For all my exceptions

// Relative time: steps that the player has experianced since the start of the game.
// Absolute time: steps since the start of the game.

bool won = false;

int relativeTime;
int absoluteTime;

// the time which propagation will stop at
int propagating;

const double STEP_TIME = 0.029;

const int MAX_GUYS = 100;
const int MAX_BOXES = 100;

PropManager propManager;

Guy guy[MAX_GUYS];
int guyCount = 0; // number of guys 'created'
Box box[MAX_BOXES];
int boxCount = 0; // number of boxes 'created'

Box MintConditionBox; // never taken out of it's packet. For overriding dead box arrays 

bool DeadBox[MAX_BOXES] = {false}; // true for unused box indexes

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
char CurrentPath[_MAX_PATH] = {'\0'}; // .exe path
char levelPath[_MAX_PATH] = {'\0'}; // .lvl path
char imagePath[_MAX_PATH] = {'\0'}; // .bmp path

// wall segment count within level
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 21;

bool paradoxTriggered; // you just lost the game

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

void StringAdd(const char* string1,const char* string2, char* newString)
{
     // (first string, string to add to end of first string, string to copy result to)
     // fixme: check for newString length
    int i;
    for (i=0;string1[i];i++) newString[i] = string1[i]; // copy string1 onto new string
    for (int j=0;string2[j];i++,j++) newString[i] = string2[j]; // copy string2 onto the end of new string
    newString[i] = 0; // terminate new string or the end will be full of junk  
}

BITMAP* LoadImage(const char* imageName)
{
    // loads a bitmap
    char tempPath[_MAX_PATH];
    StringAdd(imagePath,imageName,tempPath);
    BITMAP* tempBitmap;
    tempBitmap = load_bitmap(tempPath, NULL);
    if (!tempBitmap)
    {
       throw ImageNotLoadedException();
    }
    return tempBitmap;
}

void TestLevel(double squareSize)
{
    for (int x = 0; x < LEVEL_WIDTH; ++x)
    {
        for (int y = 0; y < LEVEL_HEIGHT; ++y)
        {
            //char testString[20];
            //sprintf(testString,"%d",wall[x][y]);
            //textout_ex( buffer, font, testString, x*block_size, y*block_size, makecol( 255, 0, 0), makecol( 0, 0, 0) );
            if (wall[x][y])
            {
               rectfill( buffer, int((x+(0.5-squareSize/2))*BLOCK_SIZE), int((y+(0.5-squareSize/2))*BLOCK_SIZE), int((x+(0.5+squareSize/2))*BLOCK_SIZE), int((y+(0.5+squareSize/2))*BLOCK_SIZE), makecol ( 70, 70, 70));
            } 
        }
    }
}

void LoadLevel (char* filePath)
{
     ifstream inputFile;
     inputFile.open(filePath);
     if (inputFile.is_open())
     {
        bool wallFound = false; //was "[WALL]" found in the file?
        const int MAX_LINE_LENGTH = 300; //MAX_LINE_LENGTH, Blasphomy! This is Madness! THIS IS SPARTAA!!!!!
        char line[MAX_LINE_LENGTH];
        while (!inputFile.eof())
        {          
            inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
            string gotLine(line);
            if (gotLine.compare(0,6,"[WALL]")==0) //Did I get "[WALL]"? Compare is actually designed for ordering strings alphabetically
            {
               wallFound = true;
               char wallString[LEVEL_WIDTH];
               for (int i=0; i < LEVEL_HEIGHT; i++)
               {
                  //getline extracts - for getline(char* s,int n,char delim) - at most n-1 characters
                  inputFile.getline(wallString,(LEVEL_WIDTH+1),'\n');
                  for (int j=0; j < LEVEL_WIDTH; j++)
                  {
                     char temp = wallString[j];
                     wall[j][i] = atoi(&temp);
                  }
               }
            }
            else if (gotLine.compare(0,7,"<BOXES>")==0) // Did I get "<BOXES>"
            {
               inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
               gotLine = line;
               while (gotLine.compare(0,8,"</BOXES>")!= 0)
               {
                  if (gotLine.compare(0,5,"<BOX>")==0)
                  {
                     map<string,string> boxData;
                     inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                     gotLine = line;
                     while (gotLine.compare(0,6,"</BOX>")!=0)
                     {
                        string::size_type it = gotLine.find("=");
                        boxData[gotLine.substr(0,it)] = gotLine.substr(it+1,gotLine.length()-(it+1));
                        inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                        gotLine = line;
                     }
                     double xPos = atof(boxData["X_POS"].data());
                     double yPos = atof(boxData["Y_POS"].data());
                     double xSpeed = atof(boxData["X_SPEED"].data());
                     double ySpeed = atof(boxData["Y_SPEED"].data());
                     //int absTime = atoi(boxData["ABS_TIME"].data());
                     box[boxCount].SetStart(xPos,yPos,xSpeed,ySpeed,0);
                     box[boxCount].SetId(boxCount);
                     boxCount++;
                  }
                  inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                  gotLine = line;
               }
            }
            else if (gotLine.compare(0,6,"<GUYS>")==0)
            {
               inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
               gotLine = line;
               while (gotLine.compare(0,8,"</GUYS>")!= 0)
               {
                  if (gotLine.compare(0,5,"<GUY>")==0)
                  {
                     map<string,string> guyData;
                     inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                     gotLine = line;
                     while (gotLine.compare(0,6,"</GUY>")!=0)
                     {
                        string::size_type it = gotLine.find("=");
                        guyData[gotLine.substr(0,it)] = gotLine.substr(it+1,gotLine.length()-(it+1));
                        inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                        gotLine = line;
                     }
                     double xPos = atof(guyData["X_POS"].data());
                     double yPos = atof(guyData["Y_POS"].data());
                     //double xSpeed = atof(boxData["X_SPEED"].data());
                     //double ySpeed = atof(boxData["Y_SPEED"].data());
                     //bool carryingBox = atoi(boxData["CARRYING_BOX"].data()); //need some kind of ascii to bool - this is ugly
                     //int absTime = atoi(boxData["ABS_TIME"].data());
                     //int relTime = atoi(boxData["REL_TIME"].data());
                     guy[guyCount].SetStart(xPos,yPos,0,0,false,0,0);
                     guy[guyCount].SetOrder(guyCount);
                     guyCount++;
                  }
                  inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                  gotLine = line;
               }
            }
            else if (gotLine.compare(0,8,"<IMAGES>")==0)
            {
               map<string,string> imageData;
               inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
               gotLine = line;
               while (gotLine.compare(0,9,"</IMAGES>")!= 0)
               {
                  string::size_type it = gotLine.find("=");
                  imageData[gotLine.substr(0,it)] = gotLine.substr(it+1,gotLine.length()-(it+1));
                  inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                  gotLine = line;
               }
               try
               {
                  background = LoadImage(imageData["BACKGROUND"].data());
               }
               catch(ImageNotLoadedException)
               {
                  background = LoadImage("background.bmp");
               }
               draw_sprite( buffer, background, BLOCK_SIZE, BLOCK_SIZE);
               try
               {
                  foreground = LoadImage(imageData["FOREGROUND"].data());    
               }
               catch (ImageNotLoadedException)
               {
                  TestLevel(1);
                  foreground = LoadImage("foreground.bmp");
               }
               draw_sprite( buffer, foreground, 0, 0);
            }
        }
        if ((wallFound) == false)
        {
           throw WallNotFoundException();
        }
        inputFile.close();
     }
     else
     {
        throw FileNotOpenedException(); //could not open file
     }
}

void MakeLevelFile(char* outputPath)
{
    ofstream outputFile;
    outputFile.open(outputPath);
    for (int i=0; i < LEVEL_HEIGHT; i++)
    {
        for(int j=0;j < LEVEL_WIDTH; j++)
        {
             if (wall[j][i])
             {
                 outputFile.put('1');
             }
             else
             {
                 outputFile.put('0'); 
             }
        }
        outputFile.put('\n');
    }
    outputFile.close();
}

int main()
{
    allegro_init(); // for all allegro functions
    install_keyboard(); // for keyboard use
    install_mouse();  // for mouse use
    show_os_cursor(MOUSE_CURSOR_ARROW); // display mouse, it is disabled by default with allegro
    set_color_depth(32);
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0) !=0)
	   {
          set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1024,768, 0, 0);
       }
    // GFX_AUTODETECT_FULLSCREEN as first param for fullscreen
    // GFX_AUTODETECT_WINDOWED as first param for windowed
    
    buffer = create_bitmap( 1024, 768); // create buffer, all drawing done to buffer
    
    // Get working directory and setup paths
    getcwd(CurrentPath, _MAX_PATH);
    char *levelPathName = "/resources/levels/";
    char *imagePathName = "/resources/images/";
    
    StringAdd(CurrentPath,levelPathName,levelPath);
    StringAdd(CurrentPath,imagePathName,imagePath);
    
    //load images
    try
    {
        guy_left = LoadImage("rhino_left.bmp");
        guy_right = LoadImage("rhino_right.bmp");
        guy_left_stop = LoadImage("rhino_left_stop.bmp");
        guy_right_stop = LoadImage("rhino_right_stop.bmp");
        box_sprite = LoadImage("box.bmp");
    }
    catch (ImageNotLoadedException)
    {
          allegro_message("Unable to load some image, exiting",allegro_error); // should add "Which Image" functionallity to ImageNotLoadedException
          return(2); // Failed to load images
    }
    // how to do text: textout_ex( screen, font, "@", 50, 50, makecol( 255, 0, 0), makecol( 0, 0, 0) );
    
    // load level 
    char tempPath[_MAX_PATH];
    StringAdd(levelPath,"testlevel.lvl",tempPath);
    try
    { 
        LoadLevel(tempPath);//"C:/Dev-Cpp/Projects/time game/resources/levels/testlevel.lvl");
    }
    catch (FileNotOpenedException)
    {
        char message[MAX_PATH+27];
        sprintf(message,"File:\n%s\ncould not be loaded",tempPath);
        allegro_message(message,allegro_error);
        return (1); // Could not load level
    }
    catch (WallNotFoundException)
    {
       allegro_message("\"[WALL]\" could not be found in the level file,\nthe file may be corrupt or incorrect",allegro_error);
       return (1); // Could not load level     
    }
    // test loaded level
    // TestLevel(0.2);

    absoluteTime = 1;
    relativeTime = 1;
    
    // Game Loop 
    double step_interval = STEP_TIME*CLOCKS_PER_SEC; // minimun time between steps
    start_timer = clock(); // timers for stable steps/second
   	//allegro_message(allegro_id); // "Allegro 4.4.0, MinGW" on my system (although I believe I have allegro 4.4.0.1 ...)
    while( !key[KEY_ESC])
    { 
        finish_timer = clock();
        elapsed_time = (double(finish_timer)-double(start_timer));
        
        if (elapsed_time >= step_interval or propagating) 
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
            
            extern char allegro_id[];
            // stop propagation if aim time is reached
            if (propManager.UpdatePropagation())
            {
                for (int i = 0; i < guyCount; ++i)
                {
                    guy[i].ResetParadoxChecking();
                }
            }
            
            // get input if not propagating
            if (!propagating)
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
                box[i].UpdateExist(absoluteTime);
                
                if (box[i].GetActive(absoluteTime))
                {
                    double boxY = box[i].GetY(absoluteTime-1);
                    activeBoxOrder[activeBoxes] = i;
                    for (int j = 0; j < activeBoxes; ++j)
                    {
                        if (box[activeBoxOrder[j]].GetY(absoluteTime-1) < boxY)
                        {
                            for (int k = activeBoxes; k > j; --k)
                            {
                                activeBoxOrder[k] = activeBoxOrder[k-1];
                            }
                            activeBoxOrder[j] = i;
                            break;
                        }
                    }
                    activeBoxes++;
                }
                
                if (!propagating)
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
                if (guy[i].GetActive(absoluteTime))
                {
                    guy[i].ForwardTimeStep(absoluteTime);
                    guy[i].UpdateBoxCarrying(absoluteTime);
                    guy[i].UpdateTimeTravel(absoluteTime);
                }
                if (!propagating)
                {
                    guy[i].unDrawSprite();
                }
            }
            
            if (!propagating)
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
       //     file_select_ex("y0", imagePath, NULL , 500, 800, 600);
            // draw buffer to screen
          //  allegro_message(imagePath);
            
        }
    }  

    // remember to cleanup all bitmaps
    destroy_bitmap( background);
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


