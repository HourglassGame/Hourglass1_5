#include <allegro.h>

#include "cgui.h"

#include <stdio.h>
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for strcat
#include <ctime> // for timing
#include <fstream> // for file I/O
#include <string> // for strings
#include <map> //used in LoadLevel

#include "Exceptions.h" //For all my exceptions


using namespace std;
//extern char allegro_id[];

const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 21;

const int BLOCK_SIZE = 32;

bool wall[LEVEL_WIDTH][LEVEL_HEIGHT];

// file paths
char path[_MAX_PATH]; // temporary, used in load and save dialogues
char currentPath[_MAX_PATH] = {'\0'}; // .exe path
char levelPath[_MAX_PATH] = {'\0'}; // .lvl path
char imagePath[_MAX_PATH] = {'\0'}; // .bmp path

void init();
void deinit();

void StringAdd(const char* string1,const char* string2, char* newString);

void LoadLevel (char* filePath);
void TestLevel(double squareSize);

void Draw();
BITMAP* buffer;

DIALOG editDialog[] =
{
/* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1) (d2)  (dp)           (dp2) (dp3) */
   { d_clear_proc,      0,    0,    0,    0,    255,  0,    0,    0,       0,   0,    NULL,          NULL, NULL  },
   { d_button_proc,     162,  142,  141,  49,   255,  0,    0,    D_EXIT,  0,   0,    path,        NULL, NULL  },
   
   { NULL,              0,    0,    0,    0,    0,    0,    0,    0,       0,   0,    NULL,          NULL, NULL  }
};

int main() {
	init();
//	allegro_message(allegro_id);
	while (!key[KEY_ESC])
    {
        if(key[KEY_L])
        {
           for (int i=0;i<_MAX_PATH;i++)
           {
              path[i] = levelPath[i];
           }
           file_select_ex("Select the file you would like to Load", path, NULL, 500, 0, 0);
           try
           {
              LoadLevel(path);
           }
           catch (FileNotOpenedException)
           {
              char message[MAX_PATH+27];
              sprintf(message,"File:\n%s\ncould not be loaded",path);
              allegro_message(message,allegro_error);
              return (1); // Could not load level
           }
           catch (WallNotFoundException)
           {
              allegro_message("\"[WALL]\" could not be found in the level file,\nthe file may be corrupt or incorrect",allegro_error);
              return (1); // Could not load level     
           }
           TestLevel(1);
           //do_dialog(editDialog,-1);
        }
    //switch (Req("Exit request", "Please make your decision|Continue|Exit|Pause")) 
   // {
   //     case 0:
  //           break;
   // }
    //do_dialog(editDialog, -1);             
    Draw();
	deinit();
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
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0) !=0)
	   {
          set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1024,768, 0, 0);
       }
	//if (res != 0) {
	//	allegro_message(allegro_error);
	//	exit(-1);
	//}
	
	install_timer();
	install_keyboard();
	install_mouse();
	
    getcwd(currentPath, _MAX_PATH);
    char *levelPathName = "/../dev cpp/resources/levels/";
    char *imagePathName = "/../dev cpp/resources/images/";
    
    StringAdd(currentPath,levelPathName,levelPath);
    StringAdd(currentPath,imagePathName,imagePath);
    
    buffer = create_bitmap( 1024, 768); // create buffer, all drawing done to buffer
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
            /*
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
            */
            /*
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
            */
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

void Draw()
{
    // draws the buffer to the screen
    acquire_screen();
    draw_sprite( screen, buffer, 0, 0);
    release_screen();
}
