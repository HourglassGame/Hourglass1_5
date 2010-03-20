#include "Hourglass_Allegro.h"
#include <semaphore.h> //To get this to work you will need to download 
                       //http://mirrors.kernel.org/sources.redhat.com/pthreads-win32/pthreads-w32-2-8-0-release.exe
                       //And copy the headers into somewhere in your include path.
                       //It's not that hard, but ask me if you need more details.
#include <vector>
#include <map>
#include <string>
#include "Editor_Exceptions.h"
#include "Editor_ResourcePathEnum.h"
#include "Editor_HourglassInputsEnum.h"
#include "Editor_SwitchingInput.h"
#include "Editor_Object.h"
#include "Editor_Box.h"
#include "Editor_Guy.h"
#include "Editor_Menu.h"
#include "Editor_Level.h"
#include "Editor_LevelLoader.h"
#include "Editor_LevelSaver.h"
#include "Editor_IntField.h"
using namespace std;

//Functions
int main();
void CloseButtonHandler();
void Ticker();

void Init();
void EngineInit();
void StateInit();
void DeInit();
void InputInit();
void TranslateInputs();

void LoadLevel (const char* filePath);
void SaveLevel(const char* outputPath);
void DoLoadLevel();
void DoLoadLevelDialog();
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

//Globals

Level* currentLevel;
BITMAP* buffer;
BITMAP* tempBuffer;

BITMAP* guy_left_stop;
BITMAP* guy_right_stop;
BITMAP* box_sprite;
#ifdef ALLEGRO_MACOSX
const int MAX_PATH = 2600;
#endif //ALLEGRO_MACOSX
bool closeButtonPressed = false;
bool drawingWall = false;
bool doingAddGuy = false;
bool doingAddBox = false;

const int BUFFER_WIDTH = 1024;
const int BUFFER_HEIGHT = 768;

const int MILLISECONDS_PER_SEC = 1000;

bool snapToGrid = true;

int gridSize = 16;
IntField gridSizeField = IntField(980,650,10,1);

extern char allegro_id[];
Level existingLevel;

const int updates_per_second = 60;

//Paths
string levelPath; // .lvl path

//Walls
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 19;

const int BLOCK_SIZE = 32;

map<string,Menu> menus;
map<HourglassInput,SwitchingInput> inputs;


#ifdef ALLEGRO_MINGW32
sem_t sem_rest;
#endif //ALLEGRO_MINGW32
#ifdef ALLEGRO_MACOSX
sem_t* sem_rest_ptr;
#endif //ALLEGRO_MACOSX
volatile int ticks = 0;
void Ticker()
{
#ifdef ALLEGRO_MACOSX
	sem_post(sem_rest_ptr);//unlock the semaphore, and allow the game loop to continue
#endif //ALLEGRO_MACOSX
#ifdef ALLEGRO_MINGW32
	sem_post(&sem_rest);
#endif //ALLEGRO_MINGW32
	ticks++;
}
END_OF_FUNCTION(ticker)

int main()
{
	try {
		Init();
        // Game Loop
		while (!(inputs[EXIT_EDITOR].GetCurrentValue() || closeButtonPressed))
		{
#ifdef ALLEGRO_MACOSX
			sem_wait(sem_rest_ptr);//wait until a full tick has passed using the semaphore
#endif //ALLEGRO_MACOSX
#ifdef ALLEGRO_MINGW32
			sem_wait(&sem_rest);//wait until a full tick has passed using the semaphore
#endif //ALLEGRO_MINGW32
			poll_mouse();
			poll_keyboard();// inputs are now definately taken -here-
			TranslateInputs(); //inputs converted from keypresses to generic (i.e. not associated with any key within the rest of the program) key downs, which last only for 1 step
			DoStep();
		}
		DeInit();
	}
	catch (HourglassException& e) {
		allegro_message("An incompletely handled hourglass exception occurred in main(); exiting.\n"
						"The exception was %s.",e.what().data());
		exit(-4);
	}
	catch (exception& e) {
		allegro_message("A standard library exception occurred in main(); exiting.\n"
						"The exception was %s.",e.what());
		return(-3);
	}
	catch (...) {
		allegro_message("An unknown exception occurred in main(); exiting.");
		return(-2);
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

#ifdef ALLEGRO_MACOSX
	remove_int(ticker);
	sem_close(sem_rest_ptr);
	sem_unlink("sem_rest");
#endif //ALLEGRO_MACOSX
#ifdef ALLEGRO_MINGW32
	sem_destroy(&sem_rest);
#endif //ALLEGRO_MINGW32
}

void CloseButtonHandler()
{
	closeButtonPressed = true;
}
//    END_OF_FUNCTION(close_button_handler) // in manual, unneeded? - used in DOS or something

void EngineInit()
{
#ifdef ALLEGRO_MINGW32
	sem_init(&sem_rest, 0, 1);
#endif //ALLEGRO_MINGW32
#ifdef ALLEGRO_MACOSX
	sem_rest_ptr = sem_open("sem_rest", O_CREAT, (S_IRUSR | S_IWUSR), 1);
#endif //ALLEGRO_MACOSX
	if (allegro_init() != 0)
	{
		printf("\"allegro_init()\" failed, Allegro could not initialised");
		exit(-1); //-1 allegro init failed
    }
	int depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_window_title("Hourglass - Editor");
	set_color_depth(depth);
    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 12024, 768, 0, 0) !=0)
    {
		if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0) != 0)
		{
			allegro_message("Hourglass could not set any graphics mode.\n"
							"This can happen if:\nYour screen is smaller than 1024/768.\n"
							"Your screen does not have a 4:3 aspect ratio.\n"
							"Other problems?\n\nallegro_error is:\n%s",allegro_error);
			throw GFXModeNotSetException();
		}
    }
    if (install_timer() != 0)
	{
		allegro_message("\"install_timer()\" failed, allegro_error is:\n%s",allegro_error);
	}
	if (install_keyboard() < 0)
    {
        allegro_message("\"install_keyboard()\" failed, allegro_error is:\n%s",allegro_error);
    }
    if (install_mouse() == -1)
    {
        allegro_message("\"install_mouse()\" failed, allegro_error is:\n%s",allegro_error);
    }

	LOCK_VARIABLE(ticks)
	LOCK_FUNCTION(Ticker)
	install_int_ex(Ticker, BPS_TO_TIMER(updates_per_second));

	//   LOCK_FUNCTION(CloseButtonHandler); // in manual, unneeded? - used in DOS or something
	set_close_button_callback(CloseButtonHandler);
    
    buffer = create_bitmap( BUFFER_WIDTH, BUFFER_HEIGHT); // create buffer, all drawing done to buffer
    tempBuffer = create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    
    try
    {
        ImageLoader imageLoader(GAME);
        guy_left_stop = imageLoader.LoadImage("rhino_left_stop.bmp");
        guy_right_stop = imageLoader.LoadImage("rhino_right_stop.bmp");
        box_sprite = imageLoader.LoadImage("box.bmp");
    }
    catch (ImageNotLoadedException& e)
    {
        allegro_message("Unable to load %s",e.whichImage().data());
        throw; // Failed to load images
    }
}

void StateInit()
{
#ifdef ALLEGRO_MINGW32
    levelPath = "../dev cpp/resources/levels/";
#endif //ALLEGRO_MINGW32
#ifdef ALLEGRO_MACOSX
    levelPath = "./";
#endif //ALLEGRO_MACOSX
    select_mouse_cursor(MOUSE_CURSOR_ARROW);
    show_mouse(screen);
	LevelLoader loadLevel;
    currentLevel = loadLevel.LoadLevel();
    menus.clear();
    
    drawingWall = false;
    doingAddGuy = false;
    doingAddBox = false;

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
	inputs[ADD_OBJECT].Add((KEY_MAX +1),PRESS);
	inputs[CANCEL_ADD_OBJECT].Add((KEY_MAX + 2),PRESS);
	inputs[REMOVE_ADD_OBJECT_MENU].Add((KEY_MAX + 2),PRESS); //KEY_MAX + x, x is mouse_b to & with.
	inputs[REMOVE_ADD_OBJECT_MENU].Add(KEY_SPACE,PRESS);
	inputs[ADD_WALL].Add((KEY_MAX + 1),HOLD);
	inputs[DELETE_WALL].Add((KEY_MAX + 2), HOLD);
	inputs[LEAVE_INPUT_FIELD].Add(KEY_ENTER,PRESS);
	inputs[LEAVE_INPUT_FIELD].Add(KEY_ENTER_PAD,PRESS);
	inputs[RESET_STATE].Add(KEY_R,PRESS);
}

void TranslateInputs() 
{
    for(std::map<HourglassInput,SwitchingInput>::iterator it = inputs.begin(); it != inputs.end(); it++)
    {
        it->second.Update();
    }
}

void LoadLevel (const char* filePath)
{
	StateInit();
	LevelLoader levelLoader;
	levelLoader.LoadLevel(filePath);
    currentLevel = levelLoader.LoadLevel(filePath);
}

void SaveLevel(const char* outputPath)
{
	LevelSaver levelSaver;
	levelSaver.SaveLevel(currentLevel, outputPath);
}

void DoLoadLevel()
{
    existingLevel = *currentLevel; //Save the state of the level as it is, before it gets messed up by an unsuccessful load.
    DoLoadLevelDialog();
}
void DoLoadLevelDialog()
{
    char path[MAX_PATH];
    strncpy(path,levelPath.c_str(),MAX_PATH);
    if(file_select_ex("Select the file you would like to Load", path, "lvl", 500, 0, 0))
    {
		try
		{
			LoadLevel(path);
		}
		catch (FileNotOpenedException)
		{
			allegro_message("File:\n%s\ncould not be loaded.",path);
			DoLoadLevelDialog();
		}
		catch (WallNotFoundException)
		{
			allegro_message("\"[WALL]\" could not be found in the level file:\n%s\nthe file may be corrupt or incorrect",path);
			DoLoadLevelDialog();
		}
	//	currentLevel->Draw();
    }
    else
    {
        delete currentLevel;
        currentLevel = &existingLevel;
    }
}
void DoSaveLevel()
{
    char path[MAX_PATH];
    strncpy(path,levelPath.c_str(),MAX_PATH);
	if(file_select_ex("Please enter the location where you would like this level to be saved", path, NULL, 500, 0, 0)) //Returns zero if closed with "cancel"
	{
		SaveLevel(path);
	}
}

void UpdateDrawingWall()
{
    drawingWall = inputs[DRAW_WALL].GetCurrentValue();
	if (drawingWall)
    {
		currentLevel->SetCanSelect(false);
	}
	else
    {
		currentLevel->SetCanSelect(true);
	}
}

void DoDrawWall()
{
    if ((inputs[ADD_WALL].GetCurrentValue()) && !(mouse_b & 2))
    {
        //add wall
        int x = int(floor(mouse_x/BLOCK_SIZE));
        int y = int(floor(mouse_y/BLOCK_SIZE));
        if (x < LEVEL_WIDTH && y < LEVEL_HEIGHT)
        {
            currentLevel->SetWall(true,x,y);
        }
    }
    else if (inputs[DELETE_WALL].GetCurrentValue() && !(mouse_b & 1))
    {
        //delete wall
        int x = int(floor(mouse_x/BLOCK_SIZE));
        int y = int(floor(mouse_y/BLOCK_SIZE));
        if (x < LEVEL_WIDTH && y < LEVEL_HEIGHT)
        {
            currentLevel->SetWall(false, x, y);
        }
    }
}

void DoAddObjectMenu()
{
    inputs[DRAW_WALL].ToggleOff();
	textout_ex( buffer, font, "W: Wall Drawing Mode - Off", 800, 16, makecol( 255, 0, 0), makecol( 0, 0, 0) );
    draw_sprite(tempBuffer, buffer, 0, 0);
    menus["addObjectMenu"];
	currentLevel->SetCanSelect(false);
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
        if (inputs[ADD_OBJECT].GetCurrentValue())
        {
			if (doingAddBox)
            {
				currentLevel->AddBox(objectX, objectY, 0, 0);
			}
			if (doingAddGuy)
            {
				currentLevel->AddGuy(objectX, objectY, 0, 0);
			}
            doingAddGuy = false;
        }
    }
    if (inputs[CANCEL_ADD_OBJECT].GetCurrentValue())
    {
        doingAddGuy = false;
        doingAddBox = false;
        draw_sprite(buffer,tempBuffer,0,0);
    }
}

void ClearBuffer()
{
    rectfill(buffer,0,0,BUFFER_WIDTH,BUFFER_HEIGHT,makecol(0,0,0));        
}

void Draw()
{
	currentLevel->DrawObjects();
	if(menus.find("addObjectMenu") != menus.end())
    {
        menus["addObjectMenu"].Draw();
    }
	gridSizeField.Init(gridSize);
	currentLevel->UpdateGUI();
	gridSize = gridSizeField.Update();
	currentLevel->SetGridSize(gridSize);
	DrawToScreen();
    ClearBuffer();
    currentLevel->DrawWall();
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
    if(inputs[RESET_STATE].GetCurrentValue())
    {
         StateInit();      
    }
	if(inputs[DO_LOAD_LEVEL_DIALOG].GetCurrentValue() && !(menus.find("addObjectMenu") != menus.end()) && !doingAddGuy && !doingAddBox)
	{
		DoLoadLevel();
	}
	
	if(inputs[DO_SAVE_LEVEL_DIALOG].GetCurrentValue())
    {
		DoSaveLevel();
    }

  	UpdateDrawingWall();
	
	if (drawingWall)
    {
        DoDrawWall();
        textout_ex( buffer, font, "W: Wall Drawing Mode - On", 800, 16, makecol( 255, 0, 0), makecol( 0, 0, 0) );
    }
    else
    {
        textout_ex( buffer, font, "W: Wall Drawing Mode - Off", 800, 16, makecol( 255, 0, 0), makecol( 0, 0, 0) );
    }
	
	if (doingAddGuy || doingAddBox)
    {
		currentLevel->SetCanSelect(false);
        DoAddingObject();
    }
	
	if(inputs[ADD_ADD_OBJECT_MENU].GetCurrentValue() && !(doingAddGuy || doingAddBox))
    {
		if (menus.find("addObjectMenu") == menus.end())
		{
			DoAddObjectMenu();
		}
    }

    if (menus.find("addObjectMenu") != menus.end())
    {
        switch(menus["addObjectMenu"].Step())
        {
			case -1:
				break;
			case 0:
				menus.erase(menus.find("addObjectMenu"));
				break;
			case 1:
				doingAddGuy = true;
				menus.erase(menus.find("addObjectMenu"));
				break;
			case 2:
				doingAddBox = true;
				menus.erase(menus.find("addObjectMenu"));
				break;
			default:
				allegro_message("menus[\"addObjectMenu\"].Step() returned a value other than -1, 0 or 1\n"
                                "this should never happen o:");
				break;
		}
    }
	currentLevel->UpdateSelection();
	currentLevel->UpdateSelected();
    Draw();
}
