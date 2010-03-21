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
    if (ySpeed[time] > 0) // down
    {
        for (int i = 0; i < boxCount; ++i)
        {
            if (box[i].GetActive(time))
            {
                double boxX = box[i].GetX(time);
                double boxY = box[i].GetY(time);
                double oldBoxY = box[i].GetY(time+1);
                int boxTimeDirection = box[i].GetTimeDirection();
                if ( (newX <= boxX+Box::BOX_WIDTH) and (newX+GUY_COLLISION_WIDTH >= boxX) and ( newY+GUY_COLLISION_HEIGHT >= boxY) and (oldY+GUY_COLLISION_HEIGHT <= oldBoxY) and (box[i].GetSupported(time) or boxTimeDirection != timeDirection) )     
                {
                    //allegro_message("bla 1");
                    ySpeed[time] = box[i].GetYspeed(time)*boxTimeDirection*timeDirection;
                    newY = boxY-GUY_COLLISION_HEIGHT;
                    jump = true;
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
        newXspeed = -MOVE_SPEED;
    }
    else if (inputRight[personalTime])
    {
        newXspeed = MOVE_SPEED;
    }
    else
    {
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
    if (newYspeed > 0) // down
    {
        for (int i = 0; i < boxCount; ++i)
        {
            if (box[i].GetActive(time))
            {
                double boxX = box[i].GetX(time);
                double boxY = box[i].GetY(time);
                double oldBoxY = box[i].GetY(time+1);
                int boxTimeDirection = box[i].GetTimeDirection();
                if ( (newX <= boxX+Box::BOX_WIDTH) and (newX+GUY_COLLISION_WIDTH >= boxX) and ( newY+GUY_COLLISION_HEIGHT >= boxY) and (oldY+GUY_COLLISION_HEIGHT <= oldBoxY) and (box[i].GetSupported(time) or boxTimeDirection != timeDirection) )     
                {
                    newYspeed = box[i].GetYspeed(time)*boxTimeDirection*timeDirection;
                    newY = boxY-GUY_COLLISION_HEIGHT;
                    jump = true;
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
            requireReverseCheck = false;
            propManager.AddPropagation(time-timeDirection,timeDirection);
        }
    }
    else
    {
        requireReverseCheck = true;
    }
    
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
                    }
                    carryingBox[time] = 1; // do not drop box
                    carryBoxId[time] = carryBoxId[time-timeDirection];
                }
                
            }
            else // If I am carrying a box with an opposite direction as I am
            { 
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
                                box[i].SetId(i);
                                DeadBox[i] = false;
                                createdBox = true;
                                break;
                            }
                        }
                        if (!createdBox)
                        {
                            box[boxCount].SetId(boxCount);
                            dropReverseBoxID[time] = boxCount;
                            boxCount++;
                        }
                        //
                        box[dropReverseBoxID[time]].DropBox(x[time]+BOX_CARRY_OFFSET_X,y[time]+BOX_CARRY_OFFSET_Y,0,0,time); // set box start positon
                        propManager.AddPropagation(time-absoluteTimeDirection, -absoluteTimeDirection); // propagate into the past
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
                    }
                    carryingBox[time] = 1; // set carrying box
                }
            }
        }
        else // if I am not carrying a box
        {
            int oldCarryingBox = carryingBox[time];
            carryingBox[time] = 0;
            //check against all boxes for ones able to be picked up.
            for (int i = 0; i < boxCount; ++i)
            {
                
                if (box[i].GetActive(time))
                {
                    double boxX = box[i].GetX(time);
                    double boxY = box[i].GetY(time);
                    if (( x[time] < boxX+Box::BOX_WIDTH) and (x[time]+GUY_COLLISION_WIDTH > boxX) and ( y[time]+GUY_COLLISION_HEIGHT > boxY) and (y[time] < boxY+Box::BOX_HEIGHT) )     
                    {
                        box[i].SetExist(time,false);
                        
                        if (oldCarryingBox != 2 and (box[i].GetTimeDirection() != absoluteTimeDirection or timeDirection != absoluteTimeDirection) )
                        {
                            propManager.AddPropagation(time-absoluteTimeDirection, -absoluteTimeDirection); // propagate into past
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
                        
                        carryingBox[time] = 2;
                        break;
                    }
                }
 
            }
            if (oldCarryingBox == 2 and carryingBox[time] != 2 and carryBoxId[time] == -1)
            {
                propManager.AddPropagation(time-absoluteTimeDirection, -absoluteTimeDirection); // propagate into past
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

void Guy::UpdateTimeTravel(int time)
{
    int personalTime = time*timeDirection+timeOffset;
 // time travel
        
    if (inputSpecial[personalTime] == 1)
    {
        if (order+1 == guyCount)
        {
            int portTime = inputSpecialArg1[personalTime];
            guy[guyCount].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],(carryingBox[time] > 0),relativeTime,portTime,timeDirection);
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
                
                guy[order+1].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],(carryingBox[time] > 0),endRelTime,depatureTimeDestination,timeDirection);
 
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
    return (time*timeDirection > startAbsTime*timeDirection and (!endAbsTime or time*timeDirection <= endAbsTime*timeDirection));
}

void Guy::ResetParadoxChecking()
{
    paradoxChecks = 0;
}

void Guy::SetStart(double newX,double newY,double newXspeed,double newYspeed,int newCarryingBox,int rel_time,int abs_time, int direction)
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
            carryingBox[abs_time] = true;
            boxCount++;
        }
    }
    else if (!newCarryingBox and carryingBox[abs_time]) // destroy a box
    {
        DeadBox[carryBoxId[abs_time]] = true;
        carryingBox[abs_time] = false;
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
        
        subimage[time] = subimage[time-timeDirection]+1;
        if (subimage[time] > 14)
        {
            subimage[time] = 0;
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
            prevDrawHead = true;
        }
        
        prevDrawX = drawX;
        prevDrawY = drawY;
        
    }
}

