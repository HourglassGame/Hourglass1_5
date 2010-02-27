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
#include "guy.h"
#include "box.h"
using namespace std;
//extern char allegro_id[];

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
char path[_MAX_PATH]; // temporary, used in load and save dialogues
char currentPath[_MAX_PATH] = {'\0'}; // .exe path
char levelPath[_MAX_PATH] = {'\0'}; // .lvl path
char imagePath[_MAX_PATH] = {'\0'}; // .bmp path

//Functions
void init();
void deinit();

void StringAdd(const char* string1,const char* string2, char* newString);

BITMAP* LoadImage(const char* imageName);

void LoadLevel (char* filePath);
void SaveLevel(char* outputPath);
void TestLevel(double squareSize);
void doLoadLevel();

void Draw();

//Globals TODO DESPERATELY - redo program in OO way to avoid these spaz number of globals );

BITMAP* buffer;
BITMAP* tempBuffer;

BITMAP* guy_left_stop;
BITMAP* guy_right_stop;
BITMAP* box_sprite;

BITMAP* menu_box;
BITMAP* menu_guy;

bool drawingWall = false;
bool pastKeyW = false;
bool pastKeySpace = false;
bool doingAddObjectMenu = false;
bool doingAddGuy = false;
bool doingAddBox = false;
bool pastKeyMouse1 = false;

const int BUFFER_WIDTH = 1024;
const int BUFFER_HEIGHT = 768;

const int MENU_WIDTH = 125;
const int MENU_HEIGHT = 50;

int menuPositionX;
int menuPositionY;

int noOfMenuItems = 0;

bool snapToGrid = true;

int gridSize = 16;

vector<Guy> guys;
vector<Box> boxes;

int guyCount = 0;

const int GUY_WIDTH = 24;
const int GUY_HEIGHT = 32;

const int BOX_WIDTH = 32;
const int BOX_HEIGHT = 32;

int main() {
	try
	{
        try
        {
        init();
        }
        catch (ImageNotLoadedException)
        {
        return(2);
        }
    	double step_interval = STEP_TIME*CLOCKS_PER_SEC; // minimun time between steps
    //	allegro_message(allegro_id);
        
        while (!key[KEY_ESC]) //Game Loop
        {
            finish_timer = clock();
            elapsed_time = (double(finish_timer)-double(start_timer));
            if (elapsed_time >= step_interval) 
            {
                start_timer = clock();
                if(key[KEY_L] && !doingAddObjectMenu && !doingAddGuy && !doingAddBox)
                {
                   try
                   {
                      doLoadLevel();
                   }
                   catch(HourglassException)
                   {
                      return(1);
                   }
                }
                
                if (key[KEY_S])
                {
                   for (int i=0;i<_MAX_PATH;i++)
                   {
                      path[i] = levelPath[i];
                   }
                   if(file_select_ex("Please enter the location where you would like this level to be saved", path, NULL, 500, 0, 0)) //Returns zero if closed with "cancel"
                   {
                      SaveLevel(path);
                   }
                }
                
                if(!pastKeyW && key[KEY_W]) // Switch in and out of wall drawing mode
                {
                   pastKeyW = true;
                   if (drawingWall)
                   {
                      drawingWall = false;
                   }
                   else
                   {
                      drawingWall = true;
                   }
                }
                
                if(pastKeyW && !key[KEY_W])
                {
                   pastKeyW = false;
                }
                
                if (drawingWall)
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
                       TestLevel(1);
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
                       TestLevel(1);
                   }
                }
                if(!pastKeySpace && key[KEY_SPACE] && !doingAddGuy && !doingAddBox) // Do add object menu
                {
                   if (!doingAddObjectMenu)
                   {
                       doingAddObjectMenu = true;
                       pastKeySpace = true;
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
                   else
                   {
                      draw_sprite(buffer, tempBuffer, 0, 0);
                      doingAddObjectMenu = false;
                      pastKeySpace = true;
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
              //  doingAddGuy = true;
                if (doingAddGuy)
                {
                   int guyX = 0;
                   int guyY = 0;
                   draw_sprite(tempBuffer, buffer,0,0);
                   if (snapToGrid)
                   {
                      guyX = int(gridSize*floor(double(mouse_x/gridSize)));
                      guyY = int(gridSize*floor(double(mouse_y/gridSize)));
                   }
                   else
                   {
                      guyX = int(floor(mouse_x));
                      guyY = int(floor(mouse_y));
                   }
                   if(guyX < LEVEL_WIDTH*BLOCK_SIZE-GUY_WIDTH && guyY < LEVEL_HEIGHT*BLOCK_SIZE-GUY_HEIGHT)
                   {
                       draw_sprite(buffer,guy_left_stop, guyX, guyY);
                       if ((mouse_b & 1) && !pastKeyMouse1)
                       {
                          pastKeyMouse1 = true;
                          Guy newGuy;
                          guys.push_back(newGuy);
                          guys[guys.size()-1].SetData(guyX,guyY,0,0);
                          draw_sprite(buffer,tempBuffer,0,0);
                          doingAddGuy = false;
                       }
                   }
                   if (mouse_b & 2)
                   {
                      doingAddGuy = false;
                      draw_sprite(buffer,tempBuffer,0,0);
                   }
                }
                
                if (doingAddBox)
                {
                   int boxX = 0;
                   int boxY = 0;
                   draw_sprite(tempBuffer, buffer,0,0);                     
                   
                   if (snapToGrid)
                   {
                      boxX = int(gridSize*floor(double(mouse_x/gridSize)));
                      boxY = int(gridSize*floor(double(mouse_y/gridSize)));
                   }
                   else
                   {
                      boxX = int(floor(mouse_x));
                      boxY = int(floor(mouse_y));
                   }
                   if(boxX < LEVEL_WIDTH*BLOCK_SIZE-BOX_WIDTH && boxY < LEVEL_HEIGHT*BLOCK_SIZE-BOX_HEIGHT)
                   {
                       draw_sprite(buffer,box_sprite, boxX, boxY);
                       if ((mouse_b & 1) && !pastKeyMouse1)
                       {
                          pastKeyMouse1 = true;
                          Box newBox;
                          boxes.push_back(newBox);
                          boxes[boxes.size()-1].SetData(boxX,boxY,0,0);
                          draw_sprite(buffer,tempBuffer,0,0);
                       }
                   }
                   if (mouse_b & 2)
                   {
                      doingAddBox = false;
                      draw_sprite(buffer,tempBuffer,0,0);
                   }
                }
                if(pastKeyMouse1 && !(mouse_b & 1))
                {
                   pastKeyMouse1 = false;            
                }
                for(int i=0;i < boxes.size();i++)
                {
                   boxes[i].DoDraw();
                }
                for(int i=0;i < guys.size();i++)
                {
                   guys[i].DoDraw();
                }
                if(doingAddObjectMenu)
                {
                   noOfMenuItems = 0;
                   draw_sprite(buffer, menu_guy, menuPositionX, menuPositionY + noOfMenuItems*MENU_HEIGHT);
                   noOfMenuItems++;
                   draw_sprite(buffer, menu_box, menuPositionX, menuPositionY + noOfMenuItems*MENU_HEIGHT);
                   noOfMenuItems++;                        
                }
                Draw();
                if(doingAddGuy)
                {
                   draw_sprite(buffer, tempBuffer,0,0);
                }
                if(doingAddBox)
                {
                   draw_sprite(buffer, tempBuffer,0,0);
                }
            }
        }
        deinit();
    }
    catch(std::exception& e)
    {
       allegro_message("A standard library exception occurred in\n""main(), Exiting",allegro_error);
       allegro_message(e.what(),allegro_error);
       return(1);
    }
    catch (...)
    {
       allegro_message("An unknown exception occurred in\n""main(), Exiting",allegro_error);
       return(1);
    }
   	return 0;
}
END_OF_MAIN()

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1024, 768, 0, 0) !=0)
    {
       set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 748, 0, 0);
    }
    
	install_keyboard();
	install_mouse();
	
    getcwd(currentPath, _MAX_PATH);
    char *levelPathName = "/../dev cpp/resources/levels/";
    char *imagePathName = "/../dev cpp/resources/images/";
    
    StringAdd(currentPath,levelPathName,levelPath);
    StringAdd(currentPath,imagePathName,imagePath);
    
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
          allegro_message("Unable to load some image, exiting",allegro_error); // should add "Which Image" functionallity to ImageNotLoadedException
          throw; // Failed to load images
    }
    
    select_mouse_cursor(MOUSE_CURSOR_ARROW);
    
    show_mouse(screen);
    
    TestLevel(1);
    
    drawingWall = false;
    pastKeyW = false;
    pastKeySpace = false; 
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
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

void LoadLevel (char* filePath)
{
     while (guys.size() != 0)
     {
        guys.pop_back();
     }
     guyCount = 0;
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
                     int xPos = atoi(boxData["X_POS"].data());
                     int yPos = atoi(boxData["Y_POS"].data());
                     double xSpeed = atof(boxData["X_SPEED"].data());
                     double ySpeed = atof(boxData["Y_SPEED"].data());
                     //int absTime = atoi(boxData["ABS_TIME"].data());
                     Box newBox;
                     boxes.push_back(newBox);
                     boxes[boxes.size()-1].SetData(xPos,yPos,xSpeed,ySpeed);
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
                     Guy newGuy;
                     guys.push_back(newGuy);
                     guys[guys.size()-1].SetData(xPos,yPos,xSpeed,ySpeed);
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
    outputFile << "<IMAGES>\n";
    outputFile << "</IMAGES>\n";
    outputFile << "<GUYS>\n";
    for (int i = 0; i < guys.size(); i++)
    {
        outputFile << "<GUY>\n";
        int xPos;
        int yPos;
        double xSpeed;
        double ySpeed;
        guys[i].GetData(xPos, yPos, xSpeed, ySpeed);
        //char message[100];
       // sprintf(message, "%d" , xPos);
        //allegro_message(message);
        outputFile << "X_POS=" << xPos << '\n';
        outputFile << "Y_POS=" << yPos << '\n';
       // outputFile << "X_SPEED = " << xSpeed << '\n';
      //  outputFile << "Y_SPEED = " << ySpeed << '\n';
        outputFile << "</GUY>\n";
    }
    outputFile << "</GUYS>\n";
    
    outputFile << "<BOXES>\n";
    for (int i = 0; i < boxes.size(); i++)
    {
        outputFile << "<BOX>\n";
        int xPos;
        int yPos;
        double xSpeed;
        double ySpeed;
        boxes[i].GetData(xPos, yPos, xSpeed, ySpeed);
        //char message[100];
       // sprintf(message, "%d" , xPos);
        //allegro_message(message);
        outputFile << "X_POS=" << xPos << '\n';
        outputFile << "Y_POS=" << yPos << '\n';
       // outputFile << "X_SPEED = " << xSpeed << '\n';
      //  outputFile << "Y_SPEED = " << ySpeed << '\n';
        outputFile << "</BOX>\n";
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

void doLoadLevel()
{
    for (int i=0;i<_MAX_PATH;i++)
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
          char message[MAX_PATH+27];
          sprintf(message,"File:\n%s\ncould not be loaded",path);
          allegro_message(message,allegro_error);
          throw; // Could not load level
       }
       catch (WallNotFoundException)
       {
          allegro_message("\"[WALL]\" could not be found in the level file,\nthe file may be corrupt or incorrect",allegro_error);
          throw; // Could not load level
       }
       TestLevel(1);
    }
}

void Draw()
{
    // draws the buffer to the screen
    show_mouse(NULL);
    acquire_screen();
    draw_sprite( screen, buffer, 0, 0);
    release_screen();
    show_mouse(screen);
}
