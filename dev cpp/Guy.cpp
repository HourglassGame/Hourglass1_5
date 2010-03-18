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

extern bool paradoxTriggered;

// physics magic numbers
const double MOVE_SPEED = 4;
const double JUMP_SPEED = 8;
const double GRAVITY = 0.17;

// box variables
const int BOX_CARRY_OFFSET_X = -4;
const int BOX_CARRY_OFFSET_Y = -32;

// width and height for purposes of collision detection
const int GUY_COLLISION_WIDTH = 24;
const int GUY_COLLISION_HEIGHT = 32;

// Input Storing
static bool leftMousePressed;
    
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
}

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

void Guy::SetId(int newId)
{
    id = newId;
}

void Guy::ForwardTimeStep(int time)
{
    // input is in relative time
    int personalTime = time-timeOffset;
    
    bool jump = false; // is it allowed to jump?
    double oldX = x[time-1];
    double oldY = y[time-1];
        
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
    ySpeed[time] = ySpeed[time-1] + GRAVITY;
        
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
        
    //check Box collision in Y direction
    if (ySpeed[time] > 0) // down
    {
        for (int i = 0; i < boxCount; ++i)
        {
            if (box[i].GetActive(time))
            {
                    double boxX = box[i].GetX(time);
                    double boxY = box[i].GetY(time);
                    if (( newX <= boxX+Box::BOX_WIDTH) and (newX+GUY_COLLISION_WIDTH >= boxX) and ( newY+GUY_COLLISION_HEIGHT >= boxY) and (oldY+GUY_COLLISION_HEIGHT <= boxY) )     
                    {
                        ySpeed[time] = box[i].GetYspeed(time);
                        newY = boxY-GUY_COLLISION_HEIGHT;
                        jump = true;
                    }
            }
        }
    }
        
    // set new locations
    x[time] = newX;
    y[time] = newY;
        
    // jump next step
    if (inputUp[personalTime] and jump)
    {
        ySpeed[time] = -JUMP_SPEED;
    }    
        
    
}

void Guy::UpdateBoxCarrying(int time)
{
     // input is in relative time
    int personalTime = time-timeOffset;
    
    //pickup or drop box
    if (inputDown[personalTime] and !inputDown[personalTime-1]) // down
    {
        if (carryingBox[time-1])
        {
            if (box[carryBoxId[time-1]].DropBox(x[time]+BOX_CARRY_OFFSET_X,y[time]+BOX_CARRY_OFFSET_Y,0,0,time) )
            {
                carryingBox[time] = false;
            }
            else
            {
                //box[carryBoxId[time-1]].SetExist(time,false);
                carryingBox[time] = true;
                carryBoxId[time] = carryBoxId[time-1];
            }
        }
        else
        {
            carryingBox[time] = false;
            for (int i = 0; i < boxCount; ++i)
            {
                if (box[i].GetActive(time))
                {
                    double boxX = box[i].GetX(time-1);
                    double boxY = box[i].GetY(time-1);
                    if (( x[time-1] < boxX+Box::BOX_WIDTH) and (x[time]+GUY_COLLISION_WIDTH > boxX) and ( y[time]+GUY_COLLISION_HEIGHT > boxY) and (y[time] < boxY+Box::BOX_HEIGHT) )     
                    {
                        box[i].SetExist(time,false);
                        carryingBox[time] = true;
                        carryBoxId[time] = i;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        if (carryingBox[time-1])
        {
            //box[carryBoxId[time-1]].SetCarried(time+1);
            carryingBox[time] = true;
            carryBoxId[time] = carryBoxId[time-1];
        }
        else
        {
            carryingBox[time] = false;
        }
    }
    
}

void Guy::UpdateTimeTravel(int time)
{
    int personalTime = time-timeOffset;
 // time travel
        
    if (inputSpecial[personalTime] == 1)
    {
        if (order+1 == guyCount)
        {
            int portTime = inputSpecialArg1[personalTime];
            guy[guyCount].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],carryingBox[time],relativeTime,portTime,1);
            guy[guyCount].SetOrder(guyCount);

            absoluteTime = portTime;
            propManager.AddPropagation(time,timeDirection);
            
            departureX = int(x[time]);
            departureY = int(y[time]);
            departureXspeed = int(xSpeed[time]);
            departureYspeed = int(ySpeed[time]);
            departureCarrying = carryingBox[time];
            depatureTimeDestination = portTime;
             
            guyCount++;
            
            endAbsTime = time;
            endRelTime = relativeTime;
        }
        else
        {
            endAbsTime = time;
            
            if (departureX != int(x[time]) or departureY != int(y[time]) or departureXspeed != int(xSpeed[time]) or departureYspeed != int(ySpeed[time]) or departureCarrying != carryingBox[time])
            {
                for (int i = 0; i < paradoxChecks; ++i)
                {
                    if (departureX == paradoxCheckX[i] and departureY == paradoxCheckY[i] and departureXspeed == paradoxCheckXspeed[i] and departureYspeed == paradoxCheckYspeed[i] and departureCarrying == paradoxCheckCarrying[i])
                    {
                        paradoxTriggered == true;
                        allegro_message("PARADOX! (foo)", allegro_error);
                        break;
                    }
                }
                
                guy[order+1].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],carryingBox[time],endRelTime,depatureTimeDestination,1);
 
                propManager.AddPropagation(depatureTimeDestination,timeDirection);
                
                paradoxCheckX[paradoxChecks] = departureX;
                paradoxCheckY[paradoxChecks] = departureY;
                paradoxCheckXspeed[paradoxChecks] = departureXspeed;
                paradoxCheckYspeed[paradoxChecks] = departureYspeed;
                paradoxCheckCarrying[paradoxChecks] = departureCarrying;
                paradoxChecks++;
                
                departureX = int(x[time]);
                departureY = int(y[time]);
                departureXspeed = int(xSpeed[time]);
                departureYspeed = int(ySpeed[time]);
                departureCarrying = carryingBox[time];
            }
            
            
        }
    }    
}

bool Guy::GetActive(int time)
{
    return (time > startAbsTime and (!endAbsTime or time <= endAbsTime));
}

void Guy::ResetParadoxChecking()
{
    paradoxChecks = 0;
}

void Guy::SetStart(double newX,double newY,double newXspeed,double newYspeed,bool newCarryingBox,int rel_time,int abs_time, int direction)
{
    startRelTime = rel_time;
    startAbsTime = abs_time;
    x[abs_time] = newX;
    y[abs_time] = newY;
    xSpeed[abs_time] = newXspeed;
    ySpeed[abs_time] = newYspeed;
    
    timeDirection = direction;
    
    if (newCarryingBox and !carryingBox[abs_time]) // create box
    {
        for (int i = 0; i < boxCount; ++i)
        {
            if (DeadBox[i])
            {
                box[i] = MintConditionBox;
                carryBoxId[abs_time] = i;
                //box[i].SetStart(0,0,0,0,abs_time);
                box[i].SetId(i);
                //box[i].SetExist(abs_time,false);
                carryingBox[abs_time] = true;
                DeadBox[i] = false;
                break;
            }
        }
        if (!carryingBox[abs_time])
        {
            carryBoxId[abs_time] = boxCount;
            //box[boxCount].SetStart(0,0,0,0,abs_time);
            box[boxCount].SetId(boxCount);
            //box[boxCount].SetExist(abs_time,false);
            carryingBox[abs_time] = true;
            boxCount++;
        }
    }
    else if (!newCarryingBox and carryingBox[abs_time]) // destroy a box
    {
        DeadBox[carryBoxId[abs_time]] = true;
        carryingBox[abs_time] = false;
    }
    
    timeOffset = abs_time - rel_time;
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
    if (time > startAbsTime and (!endAbsTime or time <= endAbsTime)) 
    {
        int drawX = int(x[time]);
        int drawY = int(y[time]);
        
        if (!draw_moving)
        {
            if (draw_facing)
            {
                draw_sprite( buffer, guy_right_stop, drawX,drawY);
                subimage = 4;
            }
            else
            {
                draw_sprite( buffer, guy_left_stop, drawX,drawY);
                subimage = 4;
            }
        }
        else
        {
            if (draw_facing)
            {
                masked_blit(guy_right ,buffer ,36*subimage+8,0,drawX,drawY,23,32);
            }
            else
            {
                masked_blit(guy_left ,buffer ,36*subimage,0,drawX,drawY,23,32);
            }   
        }
        
        if (carryingBox[time])
        {
            draw_sprite( buffer, box_sprite, drawX+BOX_CARRY_OFFSET_X,drawY+BOX_CARRY_OFFSET_Y);
            prevDrawHead = true;
        }
        
        prevDrawX = drawX;
        prevDrawY = drawY;
        
        subimage++;
        if (subimage > 14)
        {
            subimage = 0;
        }
    }
}

