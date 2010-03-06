#include <allegro.h>

//#include "cgui.h" //for Req(...) - (unused)

//#include <stdio.h>
//#include <stdlib.h>// for MAX_PATH
//#include <iostream> // for strcat (unneeded?)
//#include <ctime> // for timing
#include <fstream> // for file I/O
//#include <string> // for strings
#include <map> //used in LoadLevel
#include <math.h> // for floor()
#include <vector> //for guys, boxes, platforms etc...

#include "Exceptions.h" //For all my exceptions

#include "object.h"
//#include "menu.h"
//#include "guy.h"
using namespace std;
//extern char allegro_id[];
extern char allegro_error[ALLEGRO_ERROR_SIZE];
// timing
double elapsed_time;
clock_t start_timer,finish_timer;

const double STEP_TIME = 0.0145; // at ~60 fps wall drawing is smother (but mouse flickers more)


//Walls
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 19;

const int BLOCK_SIZE = 32;

bool wall[LEVEL_WIDTH][LEVEL_HEIGHT];

// file paths
char currentPath[_MAX_PATH] = {'\0'}; // .exe path
char levelPath[_MAX_PATH] = {'\0'}; // .lvl path
char imagePath[_MAX_PATH] = {'\0'}; // .bmp path

//Functions
void CloseButtonHandler();

void Init();
void EngineInit();
void StateInit();
void DeInit();

void WallInit();


BITMAP* LoadImage(const char* imageName);

void LoadLevel (char* filePath);
void SaveLevel(char* outputPath);
void TestLevel(double squareSize);
void DoLoadLevel();
void DoSaveLevel();

void DoToggleDrawingWall();
void DoDrawWall();
void DoAddObjectMenu();
void DoNewSelection();
void AddObject(int objectX, int objectY);
void DrawMenu();
void DeleteSelectedObject();
void DoMoveSelected();
void Draw();

void DoAddingObject();

void DoStep();

void DrawToScreen();

//Globals TODO DESPERATELY - redo program in OO way to avoid these spaz number of globals );

BITMAP* buffer;
BITMAP* tempBuffer;

BITMAP* guy_left_stop;
BITMAP* guy_right_stop;
BITMAP* box_sprite;

BITMAP* menu_box;
BITMAP* menu_guy;

bool closeButtonPressed = false;
bool drawingWall = false;
bool pastKeyW = false;
bool pastKeySpace = false;
bool doingAddObjectMenu = false;
bool doingAddGuy = false;
bool doingAddBox = false;
bool pastKeyMouse1 = false;
bool objectSelected = false;
int selectedObject = -1;
bool pastMouse2CancellingAddObject = false;

const int BUFFER_WIDTH = 1024;
const int BUFFER_HEIGHT = 768;

const int MENU_WIDTH = 125;
const int MENU_HEIGHT = 50;

const int MILLISECONDS_PER_SEC = 1000;

int menuPositionX;
int menuPositionY;

int noOfMenuItems = 0;

bool snapToGrid = true;

int gridSize = 16;

vector<Object> objects;
//vector<Menu> menus;

int main() 
{
    try
    {
        try
        {
           Init();
        }
        catch (ImageNotLoadedException)
        {
           return(2);
        }
    	
        double step_interval = STEP_TIME*CLOCKS_PER_SEC; // minimun time between steps
        //allegro_message(allegro_id);
        
        while (!(key[KEY_ESC] || closeButtonPressed)) //Game Loop
        {
            finish_timer = clock();
            elapsed_time = (double(finish_timer)-double(start_timer));
            if (elapsed_time >= step_interval)
            {
                start_timer = clock();
                poll_mouse();
                poll_keyboard();// inputs are now definately taken -here-
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
   	return 0;
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
    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1024, 768, 0, 0) !=0)
    {
       if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 748, 0, 0) != 0)
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
	
    getcwd(currentPath, _MAX_PATH);
    char *levelPathName = "/../dev cpp/resources/levels/";
    char *imagePathName = "/../dev cpp/resources/images/";
    
    sprintf(imagePath,"%s%s",currentPath,imagePathName);
    sprintf(levelPath,"%s%s",currentPath,levelPathName);
    //StringAdd(currentPath,levelPathName,levelPath);
  //  StringAdd(currentPath,imagePathName,imagePath);
    
    buffer = create_bitmap( BUFFER_WIDTH, BUFFER_HEIGHT); // create buffer, all drawing done to buffer
    tempBuffer = create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    
    try
    {
        guy_left_stop = LoadImage("rhino_left_stop.bmp");
        guy_right_stop = LoadImage("rhino_right_stop.bmp");
        box_sprite = LoadImage("box.bmp");
        menu_box = LoadImage("add_box.bmp");
        menu_guy = LoadImage("add_guy.bmp");
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
    
    objects.clear();
    
    drawingWall = false;
    pastKeyW = false;
    pastKeySpace = false;
    doingAddObjectMenu = false;
    doingAddGuy = false;
    doingAddBox = false;
    pastKeyMouse1 = false;
    objectSelected = false;
    selectedObject = -1;
    pastMouse2CancellingAddObject = false;
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

BITMAP* LoadImage(const char* imageName)
{
    // loads a bitmap
    char tempPath[_MAX_PATH] = {'\0'};
    sprintf(tempPath,"%s%s",imagePath,imageName);
    //StringAdd(imagePath,imageName,tempPath);
    BITMAP* tempBitmap;
    tempBitmap = load_bitmap(tempPath, NULL);
    if (!tempBitmap)
    {
       throw ImageNotLoadedException();
    }
    return tempBitmap;
}

void LoadLevel (char* filePath)
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
                     int xPos = atoi(boxData["X_POS"].data());
                     int yPos = atoi(boxData["Y_POS"].data());
                     double xSpeed = atof(boxData["X_SPEED"].data());
                     double ySpeed = atof(boxData["Y_SPEED"].data());
                     //int absTime = atoi(boxData["ABS_TIME"].data());
                     Object newBox;
                     objects.push_back(newBox);
                     objects[objects.size()-1].SetData(xPos,yPos,xSpeed,ySpeed,2);
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
                     int xPos = atoi(guyData["X_POS"].data());
                     int yPos = atoi(guyData["Y_POS"].data());
                     double xSpeed = atof(guyData["X_SPEED"].data());
                     double ySpeed = atof(guyData["Y_SPEED"].data());
                     //bool carryingBox = atoi(guyData["CARRYING_BOX"].data()); //need some kind of ascii to bool - this is ugly
                     //int absTime = atoi(guyData["ABS_TIME"].data());
                     //int relTime = atoi(guyData["REL_TIME"].data());
                     Object newGuy;
                     objects.push_back(newGuy);
                     objects[objects.size()-1].SetData(xPos,yPos,xSpeed,ySpeed,1);
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

void SaveLevel(char* outputPath)
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
        if (objects[i].GetType() == 1)
        {
            outputFile << "<GUY>\n";
            int xPos;
            int yPos;
            double xSpeed;
            double ySpeed;
            objects[i].GetData(xPos, yPos, xSpeed, ySpeed);
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
        if (objects[i].GetType() == 2)
        {
            outputFile << "<BOX>\n";
            int xPos;
            int yPos;
            double xSpeed;
            double ySpeed;
            objects[i].GetData(xPos, yPos, xSpeed, ySpeed);
            //char message[100];
           // sprintf(message, "%d" , xPos);
            //allegro_message(message);
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

void TestLevel(double squareSize)
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

void DoToggleDrawingWall()
{
   if (drawingWall)
   {
      drawingWall = false;
   }
   else
   {
      if (objectSelected)
      {
         objects[selectedObject].SetSelected(false);                   
      }
      objectSelected = false;
      drawingWall = true;
   }
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
    doingAddObjectMenu = true;
    drawingWall = false;
    draw_sprite(tempBuffer, buffer, 0, 0);
    menuPositionX = mouse_x;
    menuPositionY = mouse_y;
    noOfMenuItems = 0;
    draw_sprite(buffer, menu_guy, menuPositionX, menuPositionY + noOfMenuItems*MENU_HEIGHT);
    noOfMenuItems++;
    draw_sprite(buffer, menu_box, menuPositionX, menuPositionY + noOfMenuItems*MENU_HEIGHT);
    noOfMenuItems++;    
}

void DoNewSelection()
{
    for(int i=0;i < objects.size();++i)
    {
        if(objects[i].DoSelectionCheck())
        {
            for(int j=0;j < objects.size(); ++j)
            {
                if(j != i)
                {
                    objects[j].SetSelected(false);
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
                              
    if((objects[selectedObject].GetType() == 1 && 
       (objectX <= LEVEL_WIDTH*BLOCK_SIZE-GUY_WIDTH && 
        objectY <= LEVEL_HEIGHT*BLOCK_SIZE-GUY_HEIGHT)) || 
       (objects[selectedObject].GetType() == 2 &&
       (objectX <= LEVEL_WIDTH*BLOCK_SIZE-BOX_WIDTH &&
       objectY <= LEVEL_HEIGHT*BLOCK_SIZE-BOX_HEIGHT)))
    {
        objects[selectedObject].SetPos(objectX,objectY);
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
    if((doingAddGuy && objectX <= LEVEL_WIDTH*BLOCK_SIZE-GUY_WIDTH && objectY <= LEVEL_HEIGHT*BLOCK_SIZE-GUY_HEIGHT) || (doingAddBox && (objectX <= LEVEL_WIDTH*BLOCK_SIZE-BOX_WIDTH && objectY <= LEVEL_HEIGHT*BLOCK_SIZE-BOX_HEIGHT)))
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

void AddObject(int objectX,int objectY)
{
    Object newObject;
    objects.push_back(newObject);
    if(doingAddGuy)
    {
        objects[objects.size()-1].SetData(objectX,objectY,0,0,1);
    }
    else if(doingAddBox)
    {
        objects[objects.size()-1].SetData(objectX,objectY,0,0,2);
    }
    draw_sprite(buffer,tempBuffer,0,0);
    
    for(int i=0; i < objects.size(); ++i)
    {
        objects[i].SetSelected(false);        
    }
    objects[objects.size()-1].SetSelected(true);
    objectSelected = true;
    selectedObject = (objects.size()-1);
}

void DrawMenu()
{
    noOfMenuItems = 0;
    draw_sprite(buffer, menu_guy, menuPositionX, menuPositionY + noOfMenuItems*MENU_HEIGHT);
    noOfMenuItems++;
    draw_sprite(buffer, menu_box, menuPositionX, menuPositionY + noOfMenuItems*MENU_HEIGHT);
    noOfMenuItems++;   
}

void DeleteSelectedObject()
{
    // Maybe add checking whether an object is selected before deleting,
    //(although that can't happen eith the current code)
    vector<Object>::iterator it;
    it = objects.begin();
    it += selectedObject;
    objects.erase(it);
    selectedObject = -1;
    objectSelected = false;
}

void Draw()
{
    for(int i=0; i < objects.size(); ++i)
    {
        objects[i].DoDraw();
    }
    
    if(doingAddObjectMenu)
    {
        DrawMenu();
    }
    DrawToScreen();
    TestLevel(1);
    if(doingAddGuy || doingAddBox)
    {
        draw_sprite(buffer, tempBuffer,0,0);
    }    
}

void DrawToScreen()
{
    // draws the buffer to the screen
    show_mouse(NULL);
    acquire_screen();
    draw_sprite( screen, buffer, 0, 0);
    release_screen();
    show_mouse(screen);
}

void DoStep()
{
    if(key[KEY_L] && !doingAddObjectMenu && !doingAddGuy && !doingAddBox)
    {
          DoLoadLevel();    
    }
    if(key[KEY_S])
    {
          DoSaveLevel();
    }
    if(!pastKeyW && key[KEY_W] && !doingAddBox && !doingAddGuy && !doingAddObjectMenu) // Switch in and out of wall drawing mode
    {
        pastKeyW = true;
        DoToggleDrawingWall();  
    }
    if(pastKeyW && !key[KEY_W])
    {
       pastKeyW = false;
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
    if(!pastKeySpace && key[KEY_SPACE] && !doingAddGuy && !doingAddBox) // Do add object menu
    {
       pastKeySpace = true;
       if (!doingAddObjectMenu)
       {
          //Menu newMenu;
          //menus.push_back(newMenu);
          DoAddObjectMenu();
       }
       else // remove AddObjectMenu
       {
           draw_sprite(buffer, tempBuffer, 0, 0);
           doingAddObjectMenu = false;
       }
    }                      
    if(pastKeySpace && !key[KEY_SPACE])
    {
        pastKeySpace = false;
    }
    if (doingAddObjectMenu)
    {
       if (mouse_b & 1)
       {
            pastKeyMouse1 = true;
            draw_sprite(buffer, tempBuffer, 0, 0);
            doingAddObjectMenu = false;
            if (mouse_x > menuPositionX && 
                mouse_x < (menuPositionX + MENU_WIDTH) && 
                mouse_y > menuPositionY && 
                mouse_y < (menuPositionY + MENU_HEIGHT))
            {
                doingAddGuy = true;
            }
            else if (mouse_x > menuPositionX &&
                mouse_x < (menuPositionX + MENU_WIDTH) &&
                mouse_y > (menuPositionY + MENU_HEIGHT) &&
                mouse_y < (menuPositionY + MENU_HEIGHT*2))
            {
                doingAddBox = true;
            }
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
        if (mouse_b & 2 && !pastMouse2CancellingAddObject && !drawingWall && !doingAddObjectMenu && !doingAddGuy && !doingAddBox)
        {
            DoMoveSelected();
        }                
        if(key[KEY_DEL] || key[KEY_BACKSPACE])
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
