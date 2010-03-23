#include "Hourglass_Allegro.h"

//#include <direct.h> // for getcwd - unneeded
//#include <stdio.h>
//#include <stdlib.h>// for MAX_PATH
//#include <iostream> // for strcat
//#include <ctime> // for timing
#include <fstream> // for file I/O
//#include <string> // for strings
#include <vector>
#include <map> //used in LoadLevel

#include "Guy.h"
#include "Box.h"
#include "PropManager.h"

#include "Exceptions.h" //For all my exceptions

// Relative time: steps that the player has experianced since the start of the game.
// Absolute time: steps since the start of the game.

int relativeTime;
int absoluteTime;
int absoluteTimeDirection; // -1 = backward, 0 = pause, 1 = forward

// the time which propagation will stop at
int propagating;
bool resetParadox;

// read variables from prop manager
bool changeTime;
int newTime;
int newTimeDirection;

const int MAX_GUYS = 200;
const int MAX_BOXES = 200;

const int MAX_TIME = 3000; // should be 5400 for 3 minutes, 3000 is nice for now

bool viewPropagation = false;
bool waitForDraw = false; // stalls for a frame to give drawing time between jumps

PropManager propManager;

Guy guy[MAX_GUYS];
int guyCount = 0; // number of guys 'created'
Box box[MAX_BOXES];
int boxCount = 0; // number of boxes 'created'

Box MintConditionBox; // never taken out of it's packet. For overriding dead box arrays 

bool DeadBox[MAX_BOXES] = {false}; // true for unused box indices

//MetaGuy metaguy; // Stores input in relative time

using namespace std;

// timing
double elapsed_time;
clock_t start_timer,finish_timer;

const double STEP_TIME = 0.029;

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
#ifdef ALLEGRO_MACOSX
const int MAX_PATH = 2600;
#endif
#ifdef ALLEGRO_MACOSX
char CurrentPath[MAX_PATH] = "./"; // .exe path
char levelPath[MAX_PATH] = "./"; // .lvl path
char imagePath[MAX_PATH] = "./"; // .bmp path
#endif
#ifdef ALLEGRO_MINGW32
char CurrentPath[MAX_PATH] = "./"; // .exe path
char levelPath[MAX_PATH] = "./resources/levels/"; // .lvl path
char imagePath[MAX_PATH] = "./resources/images/"; // .bmp path
#endif


// wall segment count within level
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 19;

bool paradoxTriggered; // you just lost the game

// size of a wall segment
const int BLOCK_SIZE = 32;

// wall array
bool wall[LEVEL_WIDTH][LEVEL_HEIGHT];

void Draw();
void StringAdd(const char* string1,const char* string2, char* newString);
BITMAP* LoadImage(const char* imageName);
void TestLevel(double squareSize);
void LoadLevel (char* filePath);
void MakeLevelFile(char* outputPath);

int main()
{
    
    allegro_init(); // for all allegro functions
    install_keyboard(); // for keyboard use
    install_mouse();  // for mouse use
    //show_os_cursor(MOUSE_CURSOR_ARROW); // display mouse, it is disabled by default with allegro
    
    select_mouse_cursor(MOUSE_CURSOR_ARROW);  //Changed, show_os_cursor dosen't work fullscreen. Better solution is probably to draw our own bitmap mouse at end of every step.
    show_mouse(screen);
    
    set_color_depth(32);
    
	//if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1024,768, 0, 0) !=0)
    //{
       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0);
    //}
    
    // GFX_AUTODETECT_FULLSCREEN as first param for fullscreen
    // GFX_AUTODETECT_WINDOWED as first param for windowed
    
    buffer = create_bitmap( 1024, 768); // create buffer, all drawing done to buffer
    
    // Get working directory and setup paths
	
    //getcwd(CurrentPath, _MAX_PATH);
#ifdef ALLEGRO_MINGW32
    char *levelPathName = "./resources/levels/";
    char *imagePathName = "./resources/images/";
    StringAdd(CurrentPath,levelPathName,levelPath);
    StringAdd(CurrentPath,imagePathName,imagePath);
#endif
#ifdef ALLEGRO_MACOSX
 //   char *levelPath = "./";
//    char *imagePath = "./";
#endif

    //allegro_message("y0");
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
	//allegro_message("y1");
    char tempPath[MAX_PATH];
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

    // Determine Level
    
    // determine forwards boxes
    relativeTime = 1;
    absoluteTime = 1;
    absoluteTimeDirection = 1;
    
    while (absoluteTime <= MAX_TIME)
    {
        // boxes
        int activeBoxes = 0;
		int activeBoxOrder[MAX_BOXES];
 
        for (int i = 0; i < boxCount; ++i)
        {
            if (box[i].GetTimeDirection() == absoluteTimeDirection)
            {
                box[i].UpdateExist(absoluteTime);
                box[i].SetCollideable(false);
                    
                if (box[i].GetActive(absoluteTime) )
                {
                    double boxY = box[i].GetY(absoluteTime-absoluteTimeDirection);
                    activeBoxOrder[activeBoxes] = i;
                    for (int j = 0; j < activeBoxes; ++j)
                    {
                        if (box[activeBoxOrder[j]].GetY(absoluteTime-absoluteTimeDirection) < boxY)
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
            }
        }
        
        for (int i = 0; i < activeBoxes; ++i)
        {
            box[activeBoxOrder[i]].PhysicsStep(absoluteTime);
        }
        
        absoluteTime = absoluteTime + absoluteTimeDirection;
    }
    
    // determine backwards boxes
    // these boxes can disrupt the forwards boxes
    absoluteTime = MAX_TIME+1;
    absoluteTimeDirection = -1;
    
    while (absoluteTime > 0)
    {
        // boxes
        int activeBoxes = 0;
		int activeBoxOrder[MAX_BOXES];
 
        for (int i = 0; i < boxCount; ++i)
        {
            if (box[i].GetTimeDirection() == absoluteTimeDirection)
            {
                box[i].UpdateExist(absoluteTime);
                box[i].SetCollideable(false);
                    
                if (box[i].GetActive(absoluteTime) )
                {
                    double boxY = box[i].GetY(absoluteTime-absoluteTimeDirection);
                    activeBoxOrder[activeBoxes] = i;
                    for (int j = 0; j < activeBoxes; ++j)
                    {
                        if (box[activeBoxOrder[j]].GetY(absoluteTime-absoluteTimeDirection) < boxY)
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
            }
        }
        
        for (int i = 0; i < activeBoxes; ++i)
        {
            box[activeBoxOrder[i]].PhysicsStep(absoluteTime);
        }
        
        // reverse boxes
        activeBoxes = 0;
		activeBoxOrder[MAX_BOXES];
 
        for (int i = 0; i < boxCount; ++i)
        {
            if (box[i].GetTimeDirection() != absoluteTimeDirection)
            {
                box[i].UpdateExist(absoluteTime);
                box[i].SetCollideable(false);
                    
                if (box[i].GetActive(absoluteTime))
                {
                    double boxY = box[i].GetY(absoluteTime+absoluteTimeDirection);
                    activeBoxOrder[activeBoxes] = i;
                    for (int j = 0; j < activeBoxes; ++j)
                    {
                        if (box[activeBoxOrder[j]].GetY(absoluteTime+absoluteTimeDirection) < boxY)
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
            }
        }
        
        for (int i = 0; i < activeBoxes; ++i)
        {
            box[activeBoxOrder[i]].ReversePhysicsStep(absoluteTime-absoluteTimeDirection);
        }
        
        // if a propgation has been added that requires a time change time will be changed here
        if (changeTime)
        {
            for (int i = 0; i < boxCount; ++i)
            {
                box[i].TimeChangeHousekeeping(absoluteTime,absoluteTimeDirection,newTime,newTimeDirection);
            }
            changeTime = false;
            absoluteTime = newTime;
            absoluteTimeDirection = newTimeDirection;   
            continue;
        }
            
        // progress asb time
        
        absoluteTime = absoluteTime + absoluteTimeDirection;


        if (absoluteTime < 1)
        {
            break;
        }
        
    }
    
    // Set player start
    absoluteTime = guy[0].GetStartTime()+guy[0].GetTimeDirection();
    relativeTime = 1;
    absoluteTimeDirection = guy[0].GetTimeDirection();
 
    int rel_test = 1; // this tests for sync between real relativeTime and what it is suppose to be.
    
    // Game Loop 
    double step_interval = STEP_TIME*CLOCKS_PER_SEC; // minimun time between steps
    start_timer = clock(); // timers for stable steps/second
   	//allegro_message(allegro_id); // "Allegro 4.4.1, MinGW" on my system (although I believe I have allegro 4.4.1.1 ...)
    while( !key[KEY_ESC])
    { 
        finish_timer = clock();
        elapsed_time = (double(finish_timer)-double(start_timer));
        
        if (elapsed_time >= step_interval or (!viewPropagation and !guy[guyCount-1].GetActive(absoluteTime-absoluteTimeDirection) and !waitForDraw)) 
        {
            start_timer = clock();
            if (waitForDraw)
            {
                waitForDraw = false;
                continue;
            }
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
            
            sprintf(testString,"%d",propManager.GetQueuedProps());
            textout_ex( buffer, font, testString, 150, 160, makecol( 255, 255, 0), makecol( 0, 0, 0) );
         
            // extern char allegro_id[];
            // returns true if propagation has just ended

            if (changeTime)
            {
                for (int i = 0; i < boxCount; ++i)
                {
                    box[i].TimeChangeHousekeeping(absoluteTime,absoluteTimeDirection,newTime,newTimeDirection);
                }
                for (int i = 0; i < guyCount; ++i)
                {
                    guy[i].TimeChangeHousekeeping(absoluteTime,absoluteTimeDirection,newTime,newTimeDirection);
                }
                changeTime = false;
                absoluteTime = newTime;
                absoluteTimeDirection = newTimeDirection;   
            }
            
            // get input if not propagating
            if (!propagating)
            {
                // test for input desync, test can be removed if everything seems to be working
                if (rel_test != relativeTime)
                {
                    allegro_message("input desync");
                }
                rel_test++;
                // end of desync test
                Guy::StoreInput(relativeTime);
            }
            
            // order boxes from highest to lowest for easy collision checking, find a faster method that does not include many nested loops.
            
            int activeBoxes = 0;
			int activeBoxOrder[MAX_BOXES];
           
            for (int i = 0; i < boxCount; ++i)
            {
                if (box[i].GetTimeDirection() == absoluteTimeDirection)
                {
                    box[i].UpdateExist(absoluteTime);
                    box[i].SetCollideable(false);
                        
                    if (box[i].GetActive(absoluteTime) )
                    {
                        double boxY = box[i].GetY(absoluteTime-absoluteTimeDirection);
                        activeBoxOrder[activeBoxes] = i;
                        for (int j = 0; j < activeBoxes; ++j)
                        {
                            if (box[activeBoxOrder[j]].GetY(absoluteTime-absoluteTimeDirection) < boxY)
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
                }
            }
            
            // step through boxes in height order

		    for (int i = 0; i < activeBoxes; ++i)
            {
                box[activeBoxOrder[i]].PhysicsStep(absoluteTime);
            }
            
            // reverse boxes
            activeBoxes = 0;
    		activeBoxOrder[MAX_BOXES];
     
            for (int i = 0; i < boxCount; ++i)
            {
                if (box[i].GetTimeDirection() != absoluteTimeDirection)
                {
                    box[i].UpdateExist(absoluteTime);
                    box[i].SetCollideable(false);
                        
                    if (box[i].GetActive(absoluteTime))
                    {
                        double boxY = box[i].GetY(absoluteTime+absoluteTimeDirection);
                        activeBoxOrder[activeBoxes] = i;
                        for (int j = 0; j < activeBoxes; ++j)
                        {
                            if (box[activeBoxOrder[j]].GetY(absoluteTime+absoluteTimeDirection) < boxY)
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
                }
            }
            
            for (int i = 0; i < activeBoxes; ++i)
            {
                box[activeBoxOrder[i]].ReversePhysicsStep(absoluteTime-absoluteTimeDirection);
            }
            
            // step through guys
            int currentGuyCount = guyCount; // for when new ones are created
            for (int i = 0; i < currentGuyCount; ++i)
            {
                if (guy[i].GetActive(absoluteTime))
                {
                    if (guy[i].GetTimeDirection() == absoluteTimeDirection)
                    {
                        guy[i].PhysicsStep(absoluteTime);
                    }
                    else
                    {
                         guy[i].ReversePhysicsStep(absoluteTime-absoluteTimeDirection);
                    }
                    guy[i].UpdateBoxCarrying(absoluteTime);
                    guy[i].UpdateTimeTravel(absoluteTime);
                }
            }
            
            // paradox tests have to be reset after the time step as a propagation may occur at this step
            if (resetParadox)
            {
                for (int i = 0; i < guyCount; ++i)
                {
                    guy[i].ResetParadoxChecking();
                }
            }
            
            // Most of drawing
            if (guy[guyCount-1].GetActive(absoluteTime) or currentGuyCount != guyCount or viewPropagation)
            {
                for (int i = 0; i < currentGuyCount; ++i)
                {
                    guy[i].unDrawSprite();
                }
                for (int i = 0; i < boxCount; ++i)
                {
                    box[i].unDrawSprite();
                }
                
                for (int i = 0; i < boxCount; ++i)
                {
                    box[i].DrawSprite(absoluteTime);
                }
                
                for (int i = 0; i < currentGuyCount; ++i)
                {
                    guy[i].DrawSprite(absoluteTime);
                }
               
                Draw();
            }
            
            if (!propagating)
            {
                relativeTime++;
            }
            
            // if a propgation has been added that requires a time change, time will be changed here
            if (changeTime)
            {
                for (int i = 0; i < boxCount; ++i)
                {
                    box[i].TimeChangeHousekeeping(absoluteTime,absoluteTimeDirection,newTime,newTimeDirection);
                }
                for (int i = 0; i < guyCount; ++i)
                {
                    guy[i].TimeChangeHousekeeping(absoluteTime,absoluteTimeDirection,newTime,newTimeDirection);
                }
                
                if (propManager.GetQueuedProps())
                {
                    propagating = true;   
                }
                changeTime = false;
                absoluteTime = newTime;
                absoluteTimeDirection = newTimeDirection;
                
                resetParadox = propManager.UpdatePropagation();
                continue;
            }
            else
            {
                resetParadox = propManager.UpdatePropagation();
            }
            
            // progress abs time
            absoluteTime = absoluteTime + absoluteTimeDirection;

            if (absoluteTime > MAX_TIME)
            {
                absoluteTime = MAX_TIME;
            }
             if (absoluteTime < 1)
            {
                absoluteTime = 1;
            }
        }
        //else
        //{
        //    rest(1); //dosent seem to work properly on macos...
        //}
    }  

    // remember to cleanup all bitmaps
    // pointless step (I think) as they get destroyed anyway when the programme closes
    // if you have any evidence to the contrary please tell me
    destroy_bitmap( background);
    destroy_bitmap( foreground);
    destroy_bitmap( buffer);
    destroy_bitmap( guy_left);
    destroy_bitmap( guy_right);
    destroy_bitmap( guy_left_stop);
    destroy_bitmap( guy_right_stop);

    return 0;
    
}   
END_OF_MAIN()



void Draw()
{
    // draws the buffer to the screen
    show_mouse(NULL);
    acquire_screen();
    draw_sprite( screen, buffer, 0, 0);
    release_screen();
    show_mouse(screen);
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
    char tempPath[MAX_PATH];
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
               rectfill( foreground, int((x+(0.5-squareSize/2))*BLOCK_SIZE), int((y+(0.5-squareSize/2))*BLOCK_SIZE), int((x+(0.5+squareSize/2))*BLOCK_SIZE), int((y+(0.5+squareSize/2))*BLOCK_SIZE), makecol ( 70, 70, 70));
            } 
        }
    }
}

void LoadLevel (char* filePath)
{
     //TODO fix "&& !inputFile.eof()" so that it also throws an exception
	std::ifstream inputFile;
	inputFile.open(filePath);
	
	if (inputFile.is_open())
	{
		//Level* newLevel = new Level();
		bool wallFound = false; //was "[WALL]" found in the file?
		const int MAX_LINE_LENGTH = 300; //MAX_LINE_LENGTH, Blasphomy! This is Madness! THIS IS SPARTAA!!!!!
		char line[MAX_LINE_LENGTH];
		std::string* gotLine = new std::string();
		while (!inputFile.eof())
		{   
			inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
			gotLine->assign(line);
			if (gotLine->compare(0,6,"[WALL]")==0) //Did I get "[WALL]"? Compare is actually designed for ordering strings alphabetically
			{
				wallFound = true;
				for (int i=0; i < LEVEL_HEIGHT; ++i)
				{
					//		//getline extracts - for getline(char* s,int n,char delim) - at most n-1 characters
					inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
					gotLine->assign(line);
					for (int j=0; j < LEVEL_WIDTH; ++j)
					{
						char temp[1] = {line[j]};
						wall[j][i] = atoi(temp);
					}
					
				}
			}
			else if (gotLine->compare(0,6,"<GUYS>")==0)
			{
				inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
				gotLine->assign(line);
				while (gotLine->compare(0,7,"</GUYS>")!= 0 && !inputFile.eof())
				{
					if (gotLine->compare(0,5,"<GUY>")==0)
					{
						std::map<std::string,std::string> guyData;
						inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
						gotLine->assign(line);
						while (gotLine->compare(0,6,"</GUY>")!=0 && !inputFile.eof())
						{
							std::string::size_type it = gotLine->find("=");
							guyData[gotLine->substr(0,it)] = gotLine->substr(it+1,gotLine->length()-(it+1));
							inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
							gotLine->assign(line);
						}
						int xPos = atoi(guyData["X_POS"].data());
						int yPos = atoi(guyData["Y_POS"].data());
						double xSpeed = atof(guyData["X_SPEED"].data());
						double ySpeed = atof(guyData["Y_SPEED"].data());
						int start_time = atoi(guyData["START_TIME"].data());
						int start_direction = atoi(guyData["START_DIRECTION"].data());
						//bool carryingBox = atoi(guyData["CARRYING_BOX"].data()); //need some kind of ascii to bool - this is ugly
						//int absTime = atoi(guyData["ABS_TIME"].data());
						//int relTime = atoi(guyData["REL_TIME"].data());
						guy[guyCount].SetStart(xPos,yPos,xSpeed,ySpeed,false,0,0,start_time,start_direction,0);
						guy[guyCount].SetOrder(guyCount);
						guyCount++;
					}
					inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
					gotLine->assign(line);
				}
			}
			else if (gotLine->compare(0,7,"<BOXES>")==0) // Did I get "<BOXES>"
			{
				inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
				gotLine->assign(line);
				while (gotLine->compare(0,8,"</BOXES>") != 0 && !inputFile.eof()) //TODO add *better* error handling if eof reached without end symbol.
				{
					if (gotLine->compare(0,5,"<BOX>")==0)
					{
						std::map<std::string,std::string> boxData;
						inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
						gotLine->assign(line);
						while (gotLine->compare(0,6,"</BOX>")!=0 && !inputFile.eof())
						{
							std::string::size_type it = gotLine->find("=");
							boxData[gotLine->substr(0,it)] = gotLine->substr(it+1,gotLine->length()-(it+1));
							inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
							gotLine->assign(line);
						}
						int xPos = atoi(boxData["X_POS"].data());
						int yPos = atoi(boxData["Y_POS"].data());
						double xSpeed = atof(boxData["X_SPEED"].data());
						double ySpeed = atof(boxData["Y_SPEED"].data());
						//int absTime = atoi(boxData["ABS_TIME"].data());
						box[boxCount].SetStart(xPos,yPos,xSpeed,ySpeed,0,1);
						box[boxCount].SetId(boxCount);
						boxCount++;
					}
					inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
					gotLine->assign(line);
				}
			}
			else if (gotLine->compare(0,8,"<IMAGES>")==0)
            {
				map<string,string> imageData;
				inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
				gotLine->assign(line);
				while (gotLine->compare(0,9,"</IMAGES>")!= 0 && !inputFile.eof())
				{
					string::size_type it = gotLine->find("=");
					imageData[gotLine->substr(0,it)] = gotLine->substr(it+1,gotLine->length()-(it+1));
					inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
					gotLine->assign(line);
				}
				try
				{
					background = LoadImage(imageData["BACKGROUND"].data());
					//backgroundLoaded = true;
				}
				catch(ImageNotLoadedException)
				{
					//backgroundLoaded = false;
					background = LoadImage("background.bmp");
				}
				draw_sprite( buffer, background, BLOCK_SIZE, BLOCK_SIZE);
				try
				{
					//leik wtf gaise, but it's needed (on mac at least)
#ifdef ALLEGRO_MACOSX
					foreground = LoadImage(imageData["FOREGROUND"].substr(0,strlen(imageData["FOREGROUND"].data())-1).c_str());
#endif
#ifdef ALLEGRO_MINGW32
                    foreground = LoadImage(imageData["FOREGROUND"].c_str());
#endif
				}
				catch (ImageNotLoadedException)
				{
					foreground = create_bitmap(LEVEL_WIDTH*BLOCK_SIZE,LEVEL_HEIGHT*BLOCK_SIZE);
					rectfill(foreground,0,0,LEVEL_WIDTH*BLOCK_SIZE,LEVEL_HEIGHT*BLOCK_SIZE,makecol(255,0,255));
					TestLevel(1);
				}
				draw_sprite( buffer, foreground, 0, 0);
            }
			
		}
		if ((wallFound) == false)
		{
			throw WallNotFoundException();
		}
		delete gotLine;
		inputFile.close();
		//return newLevel;
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
