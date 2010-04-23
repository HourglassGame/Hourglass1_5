#include "Guy.h"
#include "Box.h"
#include "PropManager.h"
//#include "MetaGuy.h"
#include <math.h>
#include <iostream> 

// bitmaps used to draw
extern BITMAP* guy_left;
extern BITMAP* guy_right;
extern BITMAP* guy_left_stop;
extern BITMAP* guy_right_stop;

extern BITMAP* box_sprite;

extern BITMAP* background;
extern BITMAP* foreground;
extern BITMAP* buffer;

// wall segment count within level
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 19;
const int BLOCK_SIZE = 32;

extern bool wall[LEVEL_WIDTH][LEVEL_HEIGHT];
//extern MetaGuy metaguy;

extern PropManager propManager;
// external objects for collision ect..

extern int guyCount;
extern Guy guy[];

extern int boxCount;
extern Box box[];

extern Box MintConditionBox;

extern bool DeadBox[];

extern int relativeTime;
extern int absoluteTime;
extern int absoluteTimeDirection;

extern bool paradoxTriggered;
extern bool viewPropagation;

extern int maxTime; // max time on level

// physics magic numbers
const double MOVE_SPEED = 4;
const double JUMP_SPEED = 6;
const double GRAVITY = 0.17;

// box variables
const int BOX_CARRY_OFFSET_X = -4;
const int BOX_CARRY_OFFSET_Y = -32;

// width and height for purposes of collision detection
const int GUY_COLLISION_WIDTH = 24;
const int GUY_COLLISION_HEIGHT = 32;

// Input Storing
static bool leftMousePressed;
static bool rightMousePressed;
    
static bool inputLeft[9000];
static bool inputRight[9000];
static bool inputUp[9000];
static bool inputDown[9000];
    
// store attempted events, eg: jump to X, or jump X back in time.
// 0 = no action
// 1 = free chronoport with time arg1
static int inputSpecial[9000];
static int inputSpecialArg1[9000];

Guy::Guy()
{
    
}

// Static functions
void Guy::StoreInput(int time)
{
    inputLeft[time] = key[KEY_A];
    inputRight[time] = key[KEY_D];
    inputUp[time] = key[KEY_W];
    inputDown[time] = key[KEY_S];
    
    if (mouse_b & 1) // left mouse is pressed
    {
        if (!leftMousePressed)
        {
            inputSpecial[time] = 1;
            inputSpecialArg1[time] = mouse_x*3;
        }
        leftMousePressed = true;
    }
    else
    {
        leftMousePressed = false;
    }  
    
    if (mouse_b & 2) // right mouse is pressed
    {
        if (!rightMousePressed)
        {
            inputSpecial[time] = 2;
        }
        rightMousePressed = true;
    }
    else
    {
        rightMousePressed = false;
    }   
}

// Mini return functions
double Guy::GetX(int time)
{
    return x[time];
}
double Guy::GetY(int time)
{
    return y[time];
}

double Guy::GetXspeed(int time)
{
    return xSpeed[time];
}
double Guy::GetYspeed(int time)
{
    return ySpeed[time];
}

int Guy::GetOrder()
{
    return order;
}
void Guy::SetOrder(int newOrder)
{
    order = newOrder;
}

int Guy::GetStartTime()
{
    return startAbsTime;
}

int Guy::GetTimeDirection()
{
    return timeDirection;
}

void Guy::SetId(int newId)
{
    id = newId;
}

// Physics functions
void Guy::PhysicsStep(int time)
{
    // input is in relative time
    int personalTime = time*timeDirection+timeOffset;
    
    bool jump = false; // is it allowed to jump?
    double oldX = x[time-timeDirection];
    double oldY = y[time-timeDirection];
        
    // set xspeed from input
    if (inputLeft[personalTime])
    {
        draw_moving = true;
        draw_facing = false;
        xSpeed[time] = -MOVE_SPEED;
    }
    else if (inputRight[personalTime])
    {
        draw_moving = true;
        draw_facing = true;
        xSpeed[time] = MOVE_SPEED;
    }
    else
    {
        draw_moving = false;
        xSpeed[time] = 0;
    }
        
    // add GRAVITY
    ySpeed[time] = ySpeed[time-timeDirection] + GRAVITY;
        
    // new positions for collision checking
    double newX = oldX + xSpeed[time];
    double newY = oldY + ySpeed[time];
        
    //check wall collision in Y direction
    if (ySpeed[time] > 0) // down
    {
        if (wall[int(oldX/BLOCK_SIZE)][int((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)] or ((oldX - floor(oldX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-GUY_COLLISION_WIDTH) and wall[int((oldX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            ySpeed[time] = 0;
            newY = floor((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)*BLOCK_SIZE - GUY_COLLISION_HEIGHT;
            jump = true;
        }
    }
    else if (ySpeed[time] < 0) // up
    {
        if (wall[int(oldX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((oldX - floor(oldX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-GUY_COLLISION_WIDTH) and wall[int((oldX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
        {
            ySpeed[time] = 0;
            newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
        }
    }
         
    //check Box collision in Y direction
    if (ySpeed[time] >= 0) // down
    {
        for (int i = 0; i < boxCount; ++i)
        {
            if (box[i].GetActive(time) and !box[i].GetRequireCheck())
            {
                double boxX = box[i].GetX(time);
                double boxY = box[i].GetY(time);
                int boxTimeDirection = box[i].GetTimeDirection();
                double oldBoxY = box[i].GetY(time-timeDirection);
                if ((newX <= boxX+Box::BOX_WIDTH) and (newX+GUY_COLLISION_WIDTH >= boxX) and ( newY+GUY_COLLISION_HEIGHT >= boxY) and (oldY+GUY_COLLISION_HEIGHT <= oldBoxY) )     
                {
                    //allegro_message("bla 1");
                    if (boxTimeDirection != timeDirection)
                    {
                        jump = true;
                        ySpeed[time] = box[i].GetYspeed(time-timeDirection)*boxTimeDirection*timeDirection;
                    }
                    else
                    {
                        ySpeed[time] = box[i].GetYspeed(time)*boxTimeDirection*timeDirection;
                        if (box[i].GetSupported(time))
                        {
                            jump = true;
                        }
                    }
                    newY = boxY-GUY_COLLISION_HEIGHT;
                }
            }
        }
    }
    
    //check wall collision in X direction
    if (xSpeed[time] > 0) // right
    {
        if ( wall[int((newX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newY - floor(newY/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-GUY_COLLISION_HEIGHT) and wall[int((newX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            xSpeed[time] = 0;
            newX = floor((newX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)*BLOCK_SIZE - GUY_COLLISION_WIDTH;
        }
    }
    else if (xSpeed[time] < 0) // left
    {
        if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newY - floor(newY/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-GUY_COLLISION_HEIGHT) and wall[int(newX/BLOCK_SIZE)][int((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            xSpeed[time] = 0;
            newX = (floor(newX/BLOCK_SIZE) + 1)*BLOCK_SIZE;
        }
    }
        
    // set new locations
    x[time] = newX;
    y[time] = newY;
    if (InsideWall(time) and !paradoxTriggered)
    {
        allegro_message("Squished");
        paradoxTriggered = true;
    }
    // jump next step
    if (inputUp[personalTime] and jump)
    {
        ySpeed[time] = -JUMP_SPEED;
    }    
         
}

// reduced forwards step, checks consistancy
void Guy::ReversePhysicsStep(int time)
{
    
    // input is in relative time
    int personalTime = time*timeDirection+timeOffset;
    
    bool jump = false; // is it allowed to jump?
    double oldX = x[time-timeDirection];
    double oldY = y[time-timeDirection];
    
    double newYspeed = 0;
    double newXspeed = 0;
        
    // set xspeed from input
    if (inputLeft[personalTime])
    {
        draw_moving = true;
        draw_facing = false;
        newXspeed = -MOVE_SPEED;
    }
    else if (inputRight[personalTime])
    {
        draw_moving = true;
        draw_facing = true;
        newXspeed = MOVE_SPEED;
    }
    else
    {
        draw_moving = false;
        newXspeed = 0;
    }
        
    // add GRAVITY
    newYspeed = ySpeed[time-timeDirection] + GRAVITY;
        
    // new positions for collision checking
    double newX = oldX + newXspeed;
    double newY = oldY + newYspeed;
        
        //check wall collision in Y direction
    if (newYspeed > 0) // down
    {
        if (wall[int(oldX/BLOCK_SIZE)][int((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)] or ((oldX - floor(oldX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-GUY_COLLISION_WIDTH) and wall[int((oldX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            newYspeed = 0;
            newY = floor((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)*BLOCK_SIZE - GUY_COLLISION_HEIGHT;
            jump = true;
        }
    }
    else if (newYspeed < 0) // up
    {
        if (wall[int(oldX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((oldX - floor(oldX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-GUY_COLLISION_WIDTH) and wall[int((oldX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
        {
            newYspeed = 0;
            newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
        }
    }
        
    //check Box collision in Y direction
    if (newYspeed >= 0) // down
    {
        for (int i = 0; i < boxCount; ++i)
        {
            if (box[i].GetActive(time) and !box[i].GetRequireCheck())
            {
                double boxX = box[i].GetX(time);
                double boxY = box[i].GetY(time);
                int boxTimeDirection = box[i].GetTimeDirection();
                double oldBoxY = box[i].GetY(time-timeDirection);
                if ((newX <= boxX+Box::BOX_WIDTH) and (newX+GUY_COLLISION_WIDTH >= boxX) and ( newY+GUY_COLLISION_HEIGHT >= boxY) and (oldY+GUY_COLLISION_HEIGHT <= oldBoxY) )     
                {
                    //allegro_message("bla 1");
                    if (boxTimeDirection != timeDirection)
                    {
                        jump = true;
                        newYspeed = box[i].GetYspeed(time-timeDirection)*boxTimeDirection*timeDirection;
                    }
                    else
                    {
                        newYspeed = box[i].GetYspeed(time)*boxTimeDirection*timeDirection;
                        if (box[i].GetSupported(time))
                        {
                            jump = true;
                        }
                    }
                    newY = boxY-GUY_COLLISION_HEIGHT;
                }
            }
        }
    }
    
    //check wall collision in X direction
    if (newXspeed > 0) // right
    {
        if ( wall[int((newX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newY - floor(newY/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-GUY_COLLISION_HEIGHT) and wall[int((newX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            newXspeed = 0;
            newX = floor((newX+GUY_COLLISION_WIDTH)/BLOCK_SIZE)*BLOCK_SIZE - GUY_COLLISION_WIDTH;
        }
    }
    else if (newXspeed < 0) // left
    {
        if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newY - floor(newY/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-GUY_COLLISION_HEIGHT) and wall[int(newX/BLOCK_SIZE)][int((newY+GUY_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            newXspeed = 0;
            newX = (floor(newX/BLOCK_SIZE) + 1)*BLOCK_SIZE;
        }
    }
        
    // jump next step
    if (inputUp[personalTime] and jump)
    {
        newYspeed = -JUMP_SPEED;
    } 
    
    // check against old data
    if (newX == x[time] and newY == y[time] and newXspeed == xSpeed[time] and newYspeed == ySpeed[time])
    {
        if (requireReverseCheck)
        {
            requireReverseCheck = 0;
            propManager.AddPropagation(time,timeDirection);
        }
    }
    else
    {
        if (requireReverseCheck)
        {
            // 
            if ((absoluteTime == 0 and timeDirection == 1) or (absoluteTime == maxTime-1 and timeDirection == -1) )
            {
                // propagate new position if end of time is reached regardless of reverse check
                requireReverseCheck = false;
                propManager.AddPropagation(time,timeDirection);   
            }
        }
        else
        {
            requireReverseCheck = time;
        }
    }
    
}

void Guy::StartReverseCheck(int time)
{
    // start coords will need checking for reverse time guys to see if it is still determined
}

bool Guy::InsideWall(int time)
{
    // left, right, top and bottom coords in wall array
    int left = int(x[time]/BLOCK_SIZE);
    int right = int((x[time]+GUY_COLLISION_WIDTH)/BLOCK_SIZE);
    if (int(x[time]+GUY_COLLISION_WIDTH)%BLOCK_SIZE == 0)
    {
        right--;   
    }
    int top = int(y[time]/BLOCK_SIZE);
    int bottom = int((y[time]+GUY_COLLISION_HEIGHT)/BLOCK_SIZE);
    if (int(y[time]+GUY_COLLISION_HEIGHT)%BLOCK_SIZE == 0)
    {
        bottom--;   
    }
    
    // check top left wall
    if (wall[left][top])
    {
        return true;
    }
    // check top right wall
    if (wall[right][top])
    {
        return true;
    }
    // check bottom left wall
    if (wall[left][bottom])
    {
        return true;
    }
    // check bottom right wall
    if (wall[right][bottom])
    {
        return true;
    }

    return false;
}

void Guy::UpdateBoxCarrying(int time)
{
     // input is in relative time
    int personalTime = time*timeDirection+timeOffset;
    
    //pickup or drop box
    if (inputDown[personalTime] and !inputDown[personalTime-1]) // down was just pressed
    {
                
        if (carryingBox[time-timeDirection] > 0) // If I am carrying a box
        {
            if (carryBoxId[time-timeDirection] != -1) // If I am carrying a box that has the same direction as I do
            {
                if (box[carryBoxId[time-timeDirection]].DropBox(x[time]+BOX_CARRY_OFFSET_X,y[time]+BOX_CARRY_OFFSET_Y,0,0,time) ) // if the box is able to be dropped
                {
                    carryingBox[time] = -1; // drop box
                    // note the box "exist = true" was set in box.DropBox
                }
                else // if the box is unable to be dropped
                {
                    if (timeDirection != absoluteTimeDirection) // If my time direction is opposite the absolute direction
                    {
                        propManager.AddPropagation(time-absoluteTimeDirection, -absoluteTimeDirection); // propagate into the past
                        CheckForParadox(time, carryingBox[time],-1);
                        AddParadoxCheck(time, carryingBox[time],-1);
                    }
                    carryingBox[time] = 1; // do not drop box
                    carryBoxId[time] = carryBoxId[time-timeDirection];
                }
                
            }
            else // If I am carrying a box with an opposite direction as I am
            {
                
                carryBoxId[time] = -1;
                if (Box::CanDropBox(x[time]+BOX_CARRY_OFFSET_X,y[time]+BOX_CARRY_OFFSET_Y,0,0,time)) // if the box is able to be dropped
                {
                    if (carryingBox[time] != -1) // if I did not drop a box last time this time was observed
                    {
                        //create a box
                        bool createdBox = false;
                        for (int i = 0; i < boxCount; ++i)
                        {
                            if (DeadBox[i])
                            {
                                box[i] = MintConditionBox;
                                dropReverseBoxID[time] = i;
                                box[i].SetTimeDirection(-timeDirection);
                                box[i].SetId(i);
                                DeadBox[i] = false;
                                createdBox = true;
                                break;
                            }
                        }
                        if (!createdBox)
                        {
                            box[boxCount].SetId(boxCount);
                            box[boxCount].SetTimeDirection(-timeDirection);
                            dropReverseBoxID[time] = boxCount;
                            boxCount++;
                        }
                        //
                        box[dropReverseBoxID[time]].DropBox(x[time]+BOX_CARRY_OFFSET_X,y[time]+BOX_CARRY_OFFSET_Y,0,0,time); // set box start positon
                        propManager.AddPropagation(time-absoluteTimeDirection, -absoluteTimeDirection); // propagate into the past
                        CheckForParadox(time, carryingBox[time],-1);
                        AddParadoxCheck(time, carryingBox[time],-1);
                    }
                    else // if I did create a box last time
                    {
                        // check that it is in the same spot
                        int id = dropReverseBoxID[time];
                        double oldX = box[id].GetX(time);
                        double oldY = box[id].GetY(time);
                        double oldXspeed = box[id].GetXspeed(time);
                        double oldYspeed = box[id].GetYspeed(time);
                        box[id].DropBox(x[time]+BOX_CARRY_OFFSET_X,y[time]+BOX_CARRY_OFFSET_Y,0,0,time); // set box start positon
                        if (oldX != box[id].GetX(time) or oldY != box[id].GetY(time) or oldXspeed != box[id].GetXspeed(time) or oldYspeed != box[id].GetYspeed(time))
                        {
                            propManager.AddPropagation(time-absoluteTimeDirection, -absoluteTimeDirection); // propagate into the past
                            CheckForParadox(time, carryingBox[time],-1);
                            AddParadoxCheck(time, carryingBox[time],-1);
                        }
                    }
                    carryingBox[time] = -1; // set not carrying box
                }
                else // if the box was unable to be dropped
                {
                    if (carryingBox[time] == -1) // if the box was dropped last time
                    {
                        //destroy the box
                        DeadBox[dropReverseBoxID[time]] = true;
                        propManager.AddPropagation(time-absoluteTimeDirection, -absoluteTimeDirection); // propagate into the past
                        CheckForParadox(time, carryingBox[time],-1);
                        AddParadoxCheck(time, carryingBox[time],-1);
                    }
                    carryingBox[time] = 1; // set carrying box
                }
            }
        }
        else // if I am not carrying a box
        {
            
            //set old carry data for overwriting
            int oldCarryingBox = carryingBox[time];
            int oldCarryBoxId = carryBoxId[time];
            carryingBox[time] = 0;
            
            if (oldCarryingBox == -1 and carryBoxId[time] == -1) // if I dropped a box last time
            {
                DeadBox[dropReverseBoxID[time]] = true;
                propManager.AddPropagation(time+timeDirection, timeDirection); // propagate into the my future
                CheckForParadox(time, carryingBox[time],-1);
                AddParadoxCheck(time, carryingBox[time],-1);
            }
            
            //check against all boxes for ones able to be picked up.
            for (int i = 0; i < boxCount; ++i)
            {
                
                if (box[i].GetActive(time))
                {
                    double boxX = box[i].GetX(time);
                    double boxY = box[i].GetY(time);
                    if (!box[i].GetRequireCheck() and ( x[time] < boxX+Box::BOX_WIDTH) and (x[time]+GUY_COLLISION_WIDTH > boxX) and ( y[time]+GUY_COLLISION_HEIGHT > boxY) and (y[time] < boxY+Box::BOX_HEIGHT) )     
                    // if there is a box able to be picked up
                    {
                        box[i].SetExist(time,false);
                        
                        // if I picked up a box I have not picked up before
                        if (oldCarryingBox != 2 and (box[i].GetTimeDirection() != absoluteTimeDirection or timeDirection != absoluteTimeDirection) )
                        {
                            propManager.AddPropagation(time-absoluteTimeDirection, -absoluteTimeDirection); // propagate into past
                            CheckForParadox(time, carryingBox[time],-1);
                            AddParadoxCheck(time, carryingBox[time],-1);
                        }

                        // set carrying box ID                      
                        if (box[i].GetTimeDirection() == timeDirection)
                        {
                            carryBoxId[time] = i;   
                        }
                        else
                        {
                            carryBoxId[time] = -1;
                        }
                        
                        // if I picked up a different direction box last time
                        // todo: requires boxes to not be able to be on each other
                        //if (oldCarryingBox == 2 and oldCarryBoxId == -1 xor carryBoxId[time] == -1)
                        //{
                        //    propManager.AddPropagation(time-absoluteTimeDirection, -absoluteTimeDirection); // propagate into past
                        //}
                        
                        // set just picked up
                        carryingBox[time] = 2;
                        break;
                    }
                }
 
            }
            if (oldCarryingBox == 2 and carryingBox[time] != 2 and carryBoxId[time] == -1) // if I picked up a reverse box last time and did not this time
            {
                propManager.AddPropagation(time+timeDirection, timeDirection); // propagate into past
                CheckForParadox(time, carryingBox[time],-1);
                AddParadoxCheck(time, carryingBox[time],-1);
            }
            
        }
    }
    else // down was not just pressed
    {
        if (carryingBox[time-timeDirection] > 0)
        {
            carryingBox[time] = 1;
            carryBoxId[time] = carryBoxId[time-timeDirection];
        }
        else
        {
            carryingBox[time] = 0;
        }
    }
    
}

// updates time travel changes from special input
void Guy::UpdateTimeTravel(int time)
{
    int personalTime = time*timeDirection+timeOffset;
    // time travel
        
    if (inputSpecial[personalTime] == 1) // jump
    {
        if (order+1 == guyCount)
        {
            int portTime = inputSpecialArg1[personalTime];
            guy[guyCount].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],(carryingBox[time] > 0), carryBoxId[time],relativeTime,portTime,timeDirection,subimage[time]);
            guy[guyCount].SetOrder(guyCount);

            propManager.PlayerTravel(portTime,timeDirection);
            
            AddImportantTime(time, carryingBox[time], portTime);
             
            guyCount++;
            
            endAbsTime = time;
            endRelTime = relativeTime;
        }
        else
        {
            endAbsTime = time;
            
            int portTime = inputSpecialArg1[personalTime];
            
            if (TimeDiffersFromImportantTime(time,portTime))
            {
                CheckForParadox(time,carryingBox[time],portTime);
                
                guy[order+1].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],(carryingBox[time] > 0), carryBoxId[time],endRelTime,portTime,timeDirection,subimage[time]);
 
                propManager.AddPropagation(portTime,timeDirection);
                
                AddParadoxCheck(time, carryingBox[time], portTime);

                AddImportantTime(time, carryingBox[time], portTime);
            }
            
            
        }
    }    
    else if (inputSpecial[personalTime] == 2) // reverse
    {
        if (order+1 == guyCount)
        {
            guy[guyCount].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],(carryingBox[time] > 0), carryBoxId[time],relativeTime,time,-timeDirection,subimage[time]);
            guy[guyCount].SetOrder(guyCount);

            propManager.PlayerTravel(time-timeDirection,-timeDirection);
            
            AddImportantTime(time, carryingBox[time], time);
             
            guyCount++;
            
            endAbsTime = time;
            endRelTime = relativeTime;
        }
        else
        {
            endAbsTime = time;
            
            if (TimeDiffersFromImportantTime(time,time))
            {
                CheckForParadox(time,carryingBox[time],time);
                
                guy[order+1].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],(carryingBox[time] > 0), carryBoxId[time],endRelTime,time,-timeDirection,subimage[time]);
 
                propManager.AddPropagation(time-timeDirection,-timeDirection);
                
                AddParadoxCheck(time, carryingBox[time], time);

                AddImportantTime(time, carryingBox[time], time);
            }
            
            
        }
    }
}

// paradox stuff
void Guy::CheckForParadox(int time, int boxCarry, int otherTime)
{
    
    //allegro_message("bla");
    for (int i = 0; i < paradoxChecks; ++i)
    {
        int xt = int(x[time]);int yt = int(y[time]);int xst = int(xSpeed[time]);int yst = int(ySpeed[time]);
        if (time == paradoxCheckTime[i] and int(x[time]) == paradoxCheckX[i] and int(y[time]) == paradoxCheckY[i] and int(xSpeed[time]) == paradoxCheckXspeed[i] and  int(ySpeed[time]) == paradoxCheckYspeed[i] and boxCarry == paradoxCheckCarrying[i] and otherTime == paradoxCheckOtherTime[i])
        {
            if (!paradoxTriggered)
            {
                viewPropagation = true;
                allegro_message("PARADOX! (foo)", allegro_error);
            }
            paradoxTriggered = true;
            break;
        }
    }  
}

void Guy::AddParadoxCheck(int time, int boxCarry, int otherTime)
{
    // adds paradox data at time from data at time
    paradoxCheckTime[paradoxChecks] = time;
    paradoxCheckX[paradoxChecks] = int(x[time]);
    paradoxCheckY[paradoxChecks] = int(y[time]);
    paradoxCheckXspeed[paradoxChecks] = int(xSpeed[time]);
    paradoxCheckYspeed[paradoxChecks] = int(ySpeed[time]);
    paradoxCheckCarrying[paradoxChecks] = boxCarry;
    paradoxCheckOtherTime[paradoxChecks] = otherTime;
    paradoxChecks++;
}

void Guy::AddImportantTime(int time, int boxCarry, int otherTime)
{
    // adds important time data at time
    importantTime[time] = true;
    importantX[time] = int(x[time]);
    importantY[time] = int(y[time]);
    importantXspeed[time] = int(xSpeed[time]);
    importantYspeed[time] = int(ySpeed[time]);
    importantCarrying[time] = boxCarry;
    importantOtherTime[time] = otherTime;
}

bool Guy::TimeDiffersFromImportantTime(int time, int otherTime)
{
    // checks if data at time differs from important data at time
    return (importantTime[time] and (importantX[time] != int(x[time]) or importantY[time] != int(y[time]) or importantXspeed[time] != int(xSpeed[time]) or importantYspeed[time] != int(ySpeed[time]) or importantCarrying[time] != carryingBox[time] or importantOtherTime[time] != otherTime));
}

void Guy::TimeChangeHousekeeping(int oldTime,int oldTimeDir,int newTime,int newTimeDirection)
{
    // fixes any achronal variables that could mess with things during time travel
    if (oldTimeDir != newTimeDirection and requireReverseCheck)
    {
        requireReverseCheck = false;
        return;
    }
    if (oldTimeDir != timeDirection and requireReverseCheck and newTime*timeDirection > requireReverseCheck*timeDirection)
    {
        requireReverseCheck = false;
    }
}

void Guy::ResetParadoxChecking()
{
    paradoxChecks = 0;
}

//
bool Guy::GetActive(int time)
{
    return (time*timeDirection > startAbsTime*timeDirection and (!endAbsTime or time*timeDirection <= endAbsTime*timeDirection));
}

void Guy::SetStart(double newX,double newY,double newXspeed,double newYspeed,int newCarryingBox, int oldCarryBoxId,int rel_time,int abs_time, int direction, int newSubimage)
{
    startRelTime = rel_time;
    startAbsTime = abs_time;
    x[abs_time] = newX;
    y[abs_time] = newY;
    xSpeed[abs_time] = newXspeed;
    ySpeed[abs_time] = newYspeed;
    subimage[abs_time] = newSubimage;
    
    timeDirection = direction;
    
    if (oldCarryBoxId == -1)
    {
        carryBoxId[abs_time] = -1;
        carryingBox[abs_time] = true;
    }
    else
    {
        if (newCarryingBox and !carryingBox[abs_time]) // create box
        {
            for (int i = 0; i < boxCount; ++i)
            {
                if (DeadBox[i])
                {
                    box[i] = MintConditionBox;
                    box[i].SetTimeDirection(1);
                    carryBoxId[abs_time] = i;
                    box[i].SetId(i);
                    carryingBox[abs_time] = true;
                    DeadBox[i] = false;
                    break;
                }
            }
            if (!carryingBox[abs_time])
            {
                carryBoxId[abs_time] = boxCount;
                box[boxCount].SetId(boxCount);
                box[boxCount].SetTimeDirection(timeDirection);
                carryingBox[abs_time] = true;
                boxCount++;
            }
        }
        else if (!newCarryingBox and carryingBox[abs_time]) // destroy a box
        {
            DeadBox[carryBoxId[abs_time]] = true;
            carryingBox[abs_time] = false;
        }
}
    
    // set time offset based on direction, start abs time and start relative time
    if (direction == 1)
    {
        // personal time = current time - start abs time + start relative time
        timeOffset = -abs_time + rel_time;
    }
    else if (direction == -1)
    {
        // personal time = -current time + start abs time + start relative time
        timeOffset = abs_time + rel_time;
    }
}

void Guy::unDrawSprite()
{
    if (prevDrawX)
    {
        blit(background ,buffer ,prevDrawX-BLOCK_SIZE,prevDrawY-BLOCK_SIZE,prevDrawX,prevDrawY,23,32);
        //rectfill( buffer ,prevDrawX , prevDrawY, prevDrawX+23, prevDrawY+31, makecol ( 0, 0, 0));
        if (prevDrawHead)
        {
            blit(background ,buffer ,prevDrawX-BLOCK_SIZE+BOX_CARRY_OFFSET_X,prevDrawY-BLOCK_SIZE+BOX_CARRY_OFFSET_Y,prevDrawX+BOX_CARRY_OFFSET_X,prevDrawY+BOX_CARRY_OFFSET_Y,Box::BOX_WIDTH,Box::BOX_HEIGHT);
            masked_blit(foreground ,buffer ,prevDrawX+BOX_CARRY_OFFSET_X,prevDrawY+BOX_CARRY_OFFSET_Y,prevDrawX+BOX_CARRY_OFFSET_X,prevDrawY+BOX_CARRY_OFFSET_Y,Box::BOX_WIDTH,Box::BOX_HEIGHT);
            prevDrawHead = false;   
        }
        prevDrawX = 0;
    }
}

void Guy::DrawSprite(int time)
{
    if (GetActive(time)) 
    {
        int drawX = int(x[time]);
        int drawY = int(y[time]);
        
        if (timeDirection == absoluteTimeDirection)
        {
            subimage[time] = subimage[time-timeDirection]+1;
            if (subimage[time] > 14)
            {
                subimage[time] = 0;
            }
        }
        
        if (!draw_moving)
        {
            if (draw_facing)
            {
                draw_sprite( buffer, guy_right_stop, drawX,drawY);
                subimage[time] = 4;
            }
            else
            {
                draw_sprite( buffer, guy_left_stop, drawX,drawY);
                subimage[time] = 4;
            }
        }
        else
        {
            if (draw_facing)
            {
                masked_blit(guy_right ,buffer ,36*subimage[time]+8,0,drawX,drawY,23,32);
            }
            else
            {
                masked_blit(guy_left ,buffer ,36*subimage[time],0,drawX,drawY,23,32);
            }   
        }
        
        if (carryingBox[time] > 0)
        {
            draw_sprite( buffer, box_sprite, drawX+BOX_CARRY_OFFSET_X,drawY+BOX_CARRY_OFFSET_Y);
            if (carryingBox[time] == -1)
            {
                textout_ex( buffer, font, "R", drawX+BOX_CARRY_OFFSET_X+10, drawY+BOX_CARRY_OFFSET_Y+10, makecol( 255, 255, 0), makecol( 0, 0, 0) );
            }
            prevDrawHead = true;
        }
        
        if (requireReverseCheck)
        {
            textout_ex( buffer, font, ":(", drawX+5, drawY+10, makecol( 255, 255, 0), makecol( 0, 0, 0) );   
        }
        
        prevDrawX = drawX;
        prevDrawY = drawY;
        
    }
}

