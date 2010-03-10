//TODO improve error handling in general, atm it mostly just does exit();, which kind of fails.
#include <allegro.h>

//#include "cgui.h" //for Req(...) - (unused)

//#include <stdio.h>
//#include <stdlib.h>// for MAX_PATH
//#include <iostream> // for strcat (unneeded?)
//#include <ctime> // for timing
#include <fstream> // for file I/O
//#include <string> // for strings
#include <map> //used in LoadLevel
#include <cmath> // for floor()
#include <vector> //for guys, boxes, platforms etc...
#include "ImagePathEnum.h"
#include "Exceptions.h" //For all my exceptions

#include "Object.h"
#include "Menu.h"
#include "Guy.h"
#include "Box.h"
#include "ImagePathEnum.h"
#include "ImageLoader.h"
#include "FileString.h"
#include "SwitchingInput.h"
#include "HourglassInputsEnum.h"
using namespace std; //TODO - remove at some stage.
extern char allegro_id[];
//extern char allegro_error[ALLEGRO_ERROR_SIZE];
// timing
double elapsed_time;
clock_t start_timer,finish_timer;

const double STEP_TIME = 0.0145; // at ~60 fps wall drawing is smother (but mouse flickers more)

//Paths
char levelPath[MAX_PATH]; // .lvl path

//Walls
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 19;

const int BLOCK_SIZE = 32;

bool wall[LEVEL_WIDTH][LEVEL_HEIGHT] = {0};

//Functions
void CloseButtonHandler();

void Init();
void EngineInit();
void StateInit();
void DeInit();
void InputInit();
void TranslateInputs();

void WallInit();

void LoadLevel (const char* filePath);
void SaveLevel(const char* outputPath);
void TestLevel(const double squareSize);
void DoLoadLevel();
void DoSaveLevel();

void UpdateDrawingWall();
void DoDrawWall();
void DoAddObjectMenu();
void DoNewSelection();
void AddObject(const int objectX, const int objectY);
void DrawMenu();
void DeleteSelectedObject();
void DoMoveSelected();
void ClearBuffer();
void Draw();

void DoAddingObject();

void DoStep();

void DrawToScreen();

//Globals TODO DESPERATELY - redo program in OO way to avoid these spaz number of globals ); Getting there (:

BITMAP* buffer;
BITMAP* tempBuffer;

BITMAP* guy_left_stop;
BITMAP* guy_right_stop;
BITMAP* box_sprite;


bool closeButtonPressed = false;
bool drawingWall = false;
bool doingAddGuy = false;
bool doingAddBox = false;
bool pastKeyMouse1 = false;
bool objectSelected = false;
int selectedObject = -1;
bool pastMouse2CancellingAddObject = false;

const int BUFFER_WIDTH = 1024;
const int BUFFER_HEIGHT = 768;

const int MILLISECONDS_PER_SEC = 1000;

bool snapToGrid = true;

int gridSize = 16;

vector<Object*> objects;
map<string,Menu> menus;
map<HourglassInput,SwitchingInput> inputs;

int main()
{
    try
    {
        try
        {
           Init();
        }
        catch (ImageNotLoadedException) //pretty fail, already caught by global catch which also just exits.
        {
           return(4);
        }
    	
        double step_interval = STEP_TIME*CLOCKS_PER_SEC; // minimun time between steps
        //allegro_message(allegro_id);
        
        while (!(inputs[EXIT_EDITOR].GetCurrentValue() || closeButtonPressed)) //Game Loop
        {
            finish_timer = clock();
            elapsed_time = (double(finish_timer)-double(start_timer));
            if (elapsed_time >= step_interval)
            {
                start_timer = clock();
                poll_mouse();
                poll_keyboard();// inputs are now definately taken -here-
                TranslateInputs(); //inputs converted from keypresses to generic (i.e. not associated with any key within the rest of the program) key downs, which last only for 1 step
                //readkey();
                DoStep();
            }
            else
            {
                //rest(1); //Also works, if you think there is a problem with the below,
                //such as the below being too slow (althought it should be faster);
                //or bad form, with finish and elapsed timer being defined many times per loop etc...
                finish_timer = clock();
                elapsed_time = (double(finish_timer)-double(start_timer));
                rest(int(step_interval-elapsed_time)/MILLISECONDS_PER_SEC);
            }
        }
        DeInit();
    }
    catch(HourglassException)
    {
       allegro_message("An incompletely handled hourglass exception occurred in\n""main(), Exiting");
       return(1);
    }
    catch(std::exception& e)
    {
       allegro_message("A standard library exception occurred in\n""main(), Exiting");
       allegro_message(e.what());
       return(2);
    }
    catch (...)
    {
       allegro_message("An unknown exception occurred in\n""main(), Exiting");
       return(3);
    }
   	return(0);
}
END_OF_MAIN()

void Init()
{
    EngineInit();
    StateInit(); 
}

void DeInit()
{
	clear_keybuf();
}

void CloseButtonHandler()
{
   closeButtonPressed = true;
}
//    END_OF_FUNCTION(close_button_handler) // in manual, unneeded? - used in DOS or something
      
void EngineInit()
{
    int depth = 32;
	if (allegro_init() != 0)
	{
       printf("\"allegro_init()\" failed, Allegro could not initialised");
       exit(-1); //-1 allegro init failed
    }
    
	//depth = desktop_color_depth(); // consider doing this at some stage, it may be useful on certain systems.
	//if (depth == 0) depth = 32;
	set_window_title("Hourglass - Editor");
	set_color_depth(depth);
    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1024, 7668, 0, 0) !=0)
    {
       if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0) != 0)
       {
          allegro_message("Hourglass could not set any graphics mode.\nThis can happen if:\nYour screen is smaller than 1024/768.\nYour screen does not have a 4:3 aspect ratio.\nOther problems?\n\nallegro_error is:\n%s",allegro_error);
	      throw GFXModeNotSetException();
       }
    }
    
	if (install_keyboard() < 0)
    {
        allegro_message("\"install_keyboard()\" failed, allegro_error is:\n%s",allegro_error);
    }
    if (install_mouse() == -1)
    {
        allegro_message("\"install_mouse()\" failed, allegro_error is:\n%s",allegro_error);
    }
    
 //   LOCK_FUNCTION(CloseButtonHandler); // in manual, unneeded? - used in DOS or something
	set_close_button_callback(CloseButtonHandler);
	
	// file paths
    char exePath[MAX_PATH]; // .exe path

    getcwd(exePath, MAX_PATH);
    char *levelPathName = "/../dev cpp/resources/levels/";
    
    sprintf(levelPath,"%s%s",exePath,levelPathName);
    
    buffer = create_bitmap( BUFFER_WIDTH, BUFFER_HEIGHT); // create buffer, all drawing done to buffer
    tempBuffer = create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    
    try
    {
        ImageLoader imageLoader(GAME);
        guy_left_stop = imageLoader.LoadImage("rhino_left_stop.bmp");
        guy_right_stop = imageLoader.LoadImage("rhino_right_stop.bmp");
        box_sprite = imageLoader.LoadImage("box.bmp");
    }
    catch (ImageNotLoadedException)
    {
        allegro_message("Unable to load some image, exiting"); // should add "Which Image" functionallity to ImageNotLoadedException
        throw; // Failed to load images
    }
}

void StateInit()
{
    select_mouse_cursor(MOUSE_CURSOR_ARROW);
    show_mouse(screen);
    
    WallInit();
    
    while (objects.size() != 0)
    {
       delete objects.back();
       objects.pop_back();
    }
    menus.clear();
    
    drawingWall = false;
    doingAddGuy = false;
    doingAddBox = false;
    pastKeyMouse1 = false;
    objectSelected = false;
    selectedObject = -1;
    
    pastMouse2CancellingAddObject = false;
    InputInit();
}

void InputInit() //when we port to Linux, KEY_* must be replaced with _allegro_KEY_*
{
    for (map<HourglassInput,SwitchingInput>::iterator it = inputs.begin(); inputs.size() !=0;)
    {
        it->second.Reset(); //calls KeyCombination destructors, stops memory leak.
        inputs.erase(it);
        it = inputs.begin();
    }
    inputs[ADD_ADD_OBJECT_MENU].Add(KEY_SPACE,PRESS);
    inputs[DO_LOAD_LEVEL_DIALOG].Add(KEY_L,PRESS);
    inputs[DO_SAVE_LEVEL_DIALOG].Add(KEY_S,PRESS);
    inputs[DRAW_WALL].Add(KEY_W,TOGGLE_ON_PRESS);
    inputs[DELETE_OBJECT].Add(KEY_BACKSPACE,PRESS);
    inputs[DELETE_OBJECT].Add(KEY_DEL,PRESS);
    inputs[EXIT_EDITOR].Add(KEY_ESC,HOLD);
}

void TranslateInputs() 
{
    for(std::map<HourglassInput,SwitchingInput>::iterator it = inputs.begin(); it != inputs.end(); it++)
    {
        it->second.Update();
    }
}
void WallInit()
{
    for(int i = 0; i<LEVEL_WIDTH; ++i)
    {
       for(int j = 0; j<LEVEL_HEIGHT; ++j)
       {
          if(i == 0 || j == 0 || i == LEVEL_WIDTH-1 || j == LEVEL_HEIGHT-1)
          {
             wall[i][j] = true;     
          }        
       }
    }
    TestLevel(1);
}

void LoadLevel (const char* filePath)
{
     StateInit();
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
               for (int i=0; i < LEVEL_HEIGHT; ++i)
               {
                  //getline extracts - for getline(char* s,int n,char delim) - at most n-1 characters
                  inputFile.getline(wallString,(LEVEL_WIDTH+1),'\n');
                  for (int j=0; j < LEVEL_WIDTH; ++j)
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
               while (gotLine.compare(0,8,"</BOXES>") != 0 && !inputFile.eof()) //TODO add *better* error handling if eof reached without end symbol.
               {
                  if (gotLine.compare(0,5,"<BOX>")==0)
                  {
                     map<string,string> boxData;
                     inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                     gotLine = line;
                     while (gotLine.compare(0,6,"</BOX>")!=0 && !inputFile.eof())
                     {
                        string::size_type it = gotLine.find("=");
                        boxData[gotLine.substr(0,it)] = gotLine.substr(it+1,gotLine.length()-(it+1));
                        inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                        gotLine = line;
                     }
                     int xPos = atoi(boxData["X_POS"].data());
                     int yPos = atoi(boxData["Y_POS"].data());
                     double xSpeed = atof(boxData["X_SPEED"].data());
                     double ySpeed = atof(boxData["Y_SPEED"].data());
                     //int absTime = atoi(boxData["ABS_TIME"].data());
                     objects.push_back(new Box());
                     objects.back()->SetData(xPos,yPos,xSpeed,ySpeed);
                  }
                  inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                  gotLine = line;
               }
            }
            
            
            else if (gotLine.compare(0,6,"<GUYS>")==0)
            {
               inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
               gotLine = line;
               while (gotLine.compare(0,8,"</GUYS>")!= 0 && !inputFile.eof())
               {
                  if (gotLine.compare(0,5,"<GUY>")==0)
                  {
                     map<string,string> guyData;
                     inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                     gotLine = line;
                     while (gotLine.compare(0,6,"</GUY>")!=0 && !inputFile.eof())
                     {
                        string::size_type it = gotLine.find("=");
                        guyData[gotLine.substr(0,it)] = gotLine.substr(it+1,gotLine.length()-(it+1));
                        inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                        gotLine = line;
                     }
                     int xPos = atoi(guyData["X_POS"].data());
                     int yPos = atoi(guyData["Y_POS"].data());
                     double xSpeed = atof(guyData["X_SPEED"].data());
                     double ySpeed = atof(guyData["Y_SPEED"].data());
                     //bool carryingBox = atoi(guyData["CARRYING_BOX"].data()); //need some kind of ascii to bool - this is ugly
                     //int absTime = atoi(guyData["ABS_TIME"].data());
                     //int relTime = atoi(guyData["REL_TIME"].data());
                     objects.push_back(new Guy(xPos,yPos,xSpeed,ySpeed));
                  }
                  inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
                  gotLine = line;
               }
            }
            
            /*
            else if (gotLine.compare(0,8,"<IMAGES>")==0)
            {
               map<string,string> imageData;
               inputFile.getline(line,MAX_LINE_LENGTH+1,'\n');
               gotLine = line;
               while (gotLine.compare(0,9,"</IMAGES>")!= 0 && !inputFile.eof())
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
        */
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

void SaveLevel(const char* outputPath)
{
    ofstream outputFile;
    outputFile.open(outputPath);
    outputFile << "[WALL]\n";
    for (int i=0; i < LEVEL_HEIGHT; ++i)
    {
        for(int j=0;j < LEVEL_WIDTH; ++j)
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
    outputFile << "<IMAGES>\n";
    outputFile << "</IMAGES>\n";
    outputFile << "<GUYS>\n";
    for (int i = 0; i < objects.size(); ++i)
    {
        if (objects[i]->GetType() == 1)
        {
            outputFile << "<GUY>\n";
            int xPos;
            int yPos;
            double xSpeed;
            double ySpeed;
            objects[i]->GetData(xPos, yPos, xSpeed, ySpeed);
            //char message[100];
           // sprintf(message, "%d" , xPos);
            //allegro_message(message);
            outputFile << "X_POS=" << xPos << '\n';
            outputFile << "Y_POS=" << yPos << '\n';
           // outputFile << "X_SPEED = " << xSpeed << '\n';
          //  outputFile << "Y_SPEED = " << ySpeed << '\n';
            outputFile << "</GUY>\n";
        }
    }
    outputFile << "</GUYS>\n";
    
    outputFile << "<BOXES>\n";
    for (int i = 0; i < objects.size(); ++i)
    {
        if (objects[i]->GetType() == 2)
        {
            outputFile << "<BOX>\n";
            int xPos;
            int yPos;
            double xSpeed;
            double ySpeed;
            objects[i]->GetData(xPos, yPos, xSpeed, ySpeed);
            outputFile << "X_POS=" << xPos << '\n';
            outputFile << "Y_POS=" << yPos << '\n';
           // outputFile << "X_SPEED = " << xSpeed << '\n';
          //  outputFile << "Y_SPEED = " << ySpeed << '\n';
            outputFile << "</BOX>\n";
        }
    }
    outputFile << "</BOXES>";
    outputFile.close();
}

void TestLevel(const double squareSize)
{
    rectfill( buffer, 0 , 0 , LEVEL_WIDTH * BLOCK_SIZE, LEVEL_HEIGHT * BLOCK_SIZE, makecol (255,255,255));
    for (int x = 0; x < LEVEL_WIDTH; ++x)
    {
        for (int y = 0; y < LEVEL_HEIGHT; ++y)
        {
            if (wall[x][y])
            {
                rectfill( buffer, int((x+(0.5-squareSize/2))*BLOCK_SIZE), int((y+(0.5-squareSize/2))*BLOCK_SIZE), int((x+(0.5+squareSize/2))*BLOCK_SIZE), int((y+(0.5+squareSize/2))*BLOCK_SIZE), makecol ( 70, 70, 70));
            }
        }
    }
}

void DoLoadLevel()
{
    char path[_MAX_PATH] = {'\0'};
    for (int i=0;i<_MAX_PATH;++i)
    {
       path[i] = levelPath[i];
    }
    if(file_select_ex("Select the file you would like to Load", path, NULL, 500, 0, 0))
    {
       try
       {
          LoadLevel(path);
       }
       catch (FileNotOpenedException)
       {
          allegro_message("File:\n%s\ncould not be loaded.",path);
          DoLoadLevel();
          //throw; // Could not load level
       }
       catch (WallNotFoundException)
       {
          allegro_message("\"[WALL]\" could not be found in the level file:\n%s\nthe file may be corrupt or incorrect",path);
          DoLoadLevel();
          //throw; // Could not load level
       }
       TestLevel(1);
    }
}

void DoSaveLevel()
{
   char path[_MAX_PATH] = {'\0'};
   for (int i=0;i<_MAX_PATH;++i)
   {
      path[i] = levelPath[i];
   }
   if(file_select_ex("Please enter the location where you would like this level to be saved", path, NULL, 500, 0, 0)) //Returns zero if closed with "cancel"
   {
      SaveLevel(path);
   }
}

void UpdateDrawingWall()
{
    if (!drawingWall && inputs[DRAW_WALL].GetCurrentValue())
    {
        if (objectSelected)
        {
            objects[selectedObject]->SetSelected(false);               
        }
        objectSelected = false;
    }
    drawingWall = inputs[DRAW_WALL].GetCurrentValue();
}

void DoDrawWall()
{
    if ((mouse_b & 1) && !(mouse_b & 2))
    {
        //add wall
        int x = int(floor(mouse_x/BLOCK_SIZE));
        int y = int(floor(mouse_y/BLOCK_SIZE));
        if (x < LEVEL_WIDTH && y < LEVEL_HEIGHT)
        {
            wall[x][y] = true;
        }
    }
    else if ((mouse_b & 2) && !(mouse_b & 1))
    {
        //delete wall
        int x = int(floor(mouse_x/BLOCK_SIZE));
        int y = int(floor(mouse_y/BLOCK_SIZE));
        if (x < LEVEL_WIDTH && y < LEVEL_HEIGHT)
        {
            wall[x][y] = false;
        }
    }
}

void DoAddObjectMenu()
{
    drawingWall = false;
    draw_sprite(tempBuffer, buffer, 0, 0);
    Menu newMenu;
    menus["addObjectMenu"] = newMenu;
}

void DoNewSelection()
{
    for(int i=0;i < objects.size();++i)
    {
        if(objects[i]->DoSelectionCheck())
        {
            for(int j=0;j < objects.size(); ++j)
            {
                if(j != i)
                {
                    objects[j]->SetSelected(false);
                }
            }
            objectSelected = true;
            selectedObject = i;
            break;
        }
    }
}

void DoMoveSelected()
{
    int objectX = 0;
    int objectY = 0;
    if (snapToGrid)
    {
        objectX = int(gridSize*floor(double(mouse_x/gridSize)));
        objectY = int(gridSize*floor(double(mouse_y/gridSize)));
    }             
    else
    {
        objectX = int(floor(mouse_x));
        objectY = int(floor(mouse_y));
    }
    
    if(objectX <= LEVEL_WIDTH*BLOCK_SIZE-objects[selectedObject]->GetXSize() && 
        objectY <= LEVEL_HEIGHT*BLOCK_SIZE-objects[selectedObject]->GetYSize())
    {
        objects[selectedObject]->SetPos(objectX,objectY);
    }
}

void DoAddingObject()
{
    int objectX = 0;
    int objectY = 0;
    draw_sprite(tempBuffer, buffer,0,0);
    if (snapToGrid)
    {
        objectX = int(gridSize*floor(double(mouse_x/gridSize)));
        objectY = int(gridSize*floor(double(mouse_y/gridSize)));
    }
    else
    {
        objectX = int(floor(mouse_x));
        objectY = int(floor(mouse_y));
    }
    
    if((doingAddGuy && objectX <= LEVEL_WIDTH*BLOCK_SIZE-Guy().GetXSize() && objectY <= LEVEL_HEIGHT*BLOCK_SIZE-Guy().GetYSize()) || (doingAddBox && (objectX <= LEVEL_WIDTH*BLOCK_SIZE-Box().GetXSize() && objectY <= LEVEL_HEIGHT*BLOCK_SIZE-Box().GetYSize())))
    {
        if (doingAddGuy)
        {
            draw_sprite(buffer,guy_left_stop, objectX, objectY);
        }
        else if (doingAddBox)
        {
            draw_sprite(buffer,box_sprite, objectX, objectY);
        }
        
        if ((mouse_b & 1) && !pastKeyMouse1)
        {
            pastKeyMouse1 = true;
            AddObject(objectX, objectY);
            doingAddGuy = false;
        }
    }
    
    if (mouse_b & 2)
    {
        doingAddGuy = false;
        doingAddBox = false;
        draw_sprite(buffer,tempBuffer,0,0);
        pastMouse2CancellingAddObject = true;
    }
}

void AddObject(const int objectX,const int objectY)
{
    if(doingAddGuy)
    {
        objects.push_back(new Guy(objectX,objectY));
    }
    else if(doingAddBox)
    {
        objects.push_back(new Box());
        objects.back()->SetData(objectX,objectY,0,0);
    }
    draw_sprite(buffer,tempBuffer,0,0);
    
    for(int i=0; i < objects.size(); ++i)
    {
        objects[i]->SetSelected(false);        
    }
    objects[objects.size()-1]->SetSelected(true);
    objectSelected = true;
    selectedObject = (objects.size()-1);
}

void DeleteSelectedObject()
{
    // Maybe add checking whether an object is selected before deleting,
    //(although that can't happen with the current code)
    vector<Object*>::iterator it = objects.begin();
    it += selectedObject;
    delete objects[selectedObject];
    objects.erase(it);
    selectedObject = -1;
    objectSelected = false;
}
void ClearBuffer()
{
    rectfill(buffer,0,0,BUFFER_WIDTH,BUFFER_HEIGHT,makecol(0,0,0));        
}
void Draw()
{
    for(int i=0; i < objects.size(); ++i)
    {
        objects[i]->DoDraw();
    }
    //allegro_message("%d",menus.size());
    if(menus.find("addObjectMenu") != menus.end())//doingAddObjectMenu)
    {
        //allegro_message("%d",menus.size());
        menus["addObjectMenu"].Draw();
    }
    DrawToScreen();
    ClearBuffer();
    TestLevel(1);
    if(doingAddGuy || doingAddBox)
    {
        draw_sprite(buffer, tempBuffer,0,0);
    }
}

void DrawToScreen()
{
    // draws the buffer to the screen
    scare_mouse();
    acquire_screen();
    draw_sprite( screen, buffer, 0, 0);
    release_screen();
    unscare_mouse();
}

void DoStep()
{
    if(inputs[DO_LOAD_LEVEL_DIALOG].GetCurrentValue() && !(menus.find("addObjectMenu") != menus.end()) && !doingAddGuy && !doingAddBox)
    {
          DoLoadLevel();
    }
    if(inputs[DO_SAVE_LEVEL_DIALOG].GetCurrentValue())
    {
          DoSaveLevel();
    }
    if(!doingAddBox && !doingAddGuy && menus.find("addObjectMenu") == menus.end()) // Switch in and out of wall drawing mode
    {
        UpdateDrawingWall();  
    }     
    if (drawingWall)
    {
        DoDrawWall();
        textout_ex( buffer, font, "W: Wall Drawing Mode - On", 800, 16, makecol( 255, 0, 0), makecol( 0, 0, 0) );
    }
    else
    {
        textout_ex( buffer, font, "W: Wall Drawing Mode - Off", 800, 16, makecol( 255, 0, 0), makecol( 0, 0, 0) );
    }
    // Do add object menu
    if(inputs[ADD_ADD_OBJECT_MENU].GetCurrentValue() && !(doingAddGuy || doingAddBox))
    {
       if (menus.find("addObjectMenu") == menus.end())
       {
          DoAddObjectMenu();
       }
       else
       {
           menus.erase(menus.find("addObjectMenu")); //TODO Remove, put in Menu.Step();
       }
    }
    
    if (menus.find("addObjectMenu") != menus.end())
    {
        switch(menus["addObjectMenu"].Step())
        {
           case -1:
               break;
           case 0:
               doingAddGuy = true;
               menus.erase(menus.find("addObjectMenu"));
               break;
           case 1:
               doingAddBox = true;
               menus.erase(menus.find("addObjectMenu"));
               break;
           default:
               allegro_message("menus[\"addObjectMenu\"].Step() returned a value other than -1, 0 or 1\n"
                                "this should never happen o:");
               break;
       }
    }
    if (doingAddGuy || doingAddBox)
    {
        DoAddingObject();
    }
    if (!(mouse_b & 2) && pastMouse2CancellingAddObject)
    {
        pastMouse2CancellingAddObject = false;
    }
    if(mouse_b & 1 && !pastKeyMouse1 && !drawingWall)
    {
        pastKeyMouse1 = true;
        DoNewSelection();
    }
    if (objectSelected)
    {
        if (mouse_b & 2 && !pastMouse2CancellingAddObject && !drawingWall && !(menus.find("addObjectMenu") != menus.end()) && !doingAddGuy && !doingAddBox)
        {
            DoMoveSelected();
        }
        if(inputs[DELETE_OBJECT].GetCurrentValue())
        {
           DeleteSelectedObject();
        }
    }       
    if(pastKeyMouse1 && !(mouse_b & 1))
    {
        pastKeyMouse1 = false;
    }
    Draw();
}
