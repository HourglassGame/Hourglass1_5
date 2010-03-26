#include "Box.h"
#include "PropManager.h"
#include <math.h>
#include <iostream> 

extern BITMAP* box_sprite;

extern BITMAP* background;
extern BITMAP* foreground;
extern BITMAP* buffer;

extern int maxTime; // max abs time

// wall segment count within level
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 19;
const int BLOCK_SIZE = 32;

extern bool wall[LEVEL_WIDTH][LEVEL_HEIGHT];
extern int boxCount;
extern Box box[];

extern PropManager propManager;

extern int absoluteTime;
extern int absoluteTimeDirection;

extern bool DeadBox[];


Box::Box() :
GRAVITY(0.17)
{

}

double Box::GetX(int time)
{
    return x[time];
}
double Box::GetY(int time)
{
    return y[time];
}

double Box::GetXspeed(int time)
{
    return xSpeed[time];
}
double Box::GetYspeed(int time)
{
    return ySpeed[time];
}

void Box::SetId(int newId)
{
    id = newId;
}

void Box::SetRequireReverseCheck(int state)
{
    requireReverseCheck = state;
}

void Box::SetExist(int abs_time,bool state)
{
    exist[abs_time] = state;
}
bool Box::GetExist(int abs_time)
{
    return exist[abs_time];
}

void Box::SetCollideable(bool state)
{
    collideable = state;
}
bool Box::GetCollideable()
{
    return collideable;
}

bool Box::GetRequireCheck()
{
    return requireReverseCheck;
}

bool Box::CanDropBox(double newX,double newY,double newXspeed,double newYspeed,int abs_time)
{
    bool dropped = true;
    
    // check for being dropped into a wall
    //up
    if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newX - floor(newX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_WIDTH) and wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
    {
        //right
        if ( wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] and wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)])
        {
            newX = floor((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)*BLOCK_SIZE - BOX_COLLISION_WIDTH;
            if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] and ((newX - floor(newX/BLOCK_SIZE)*BLOCK_SIZE <= BLOCK_SIZE-BOX_COLLISION_WIDTH) or wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
            {
                newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
            }
        }
        //left
        else if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] and wall[int(newX/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)])
        {
            newX = (floor(newX/BLOCK_SIZE) + 1)*BLOCK_SIZE;
            if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] and ((newX - floor(newX/BLOCK_SIZE)*BLOCK_SIZE <= BLOCK_SIZE-BOX_COLLISION_WIDTH) or wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
            {
                newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
            }
        }
        else
        {
            newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
        }
    }
    
    //check box collision
    for (int i = 0; i < boxCount; ++i)
    {
        if (!DeadBox[i] and box[i].GetExist(abs_time) and !box[i].GetRequireCheck())
        {
        // boxes are stepped through in height order so getting current position is all good!
            double boxX = box[i].GetX(abs_time);
            double boxY = box[i].GetY(abs_time);
            if (( newX < boxX+BOX_WIDTH) and (newX+BOX_COLLISION_WIDTH > boxX) and ( newY+BOX_COLLISION_HEIGHT > boxY) and (newY < boxY+BOX_COLLISION_HEIGHT) ) 
            {
                dropped = false;
            }
        }
    }
    
    return dropped;
}

bool Box::DropBox(double newX,double newY,double newXspeed,double newYspeed,int abs_time)
{
    bool dropped = true;
    
    // check for being dropped into a wall
    //up
    if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newX - floor(newX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_WIDTH) and wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
    {
        //right
        if ( wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] and wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)])
        {
            newX = floor((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)*BLOCK_SIZE - BOX_COLLISION_WIDTH;
            if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] and ((newX - floor(newX/BLOCK_SIZE)*BLOCK_SIZE <= BLOCK_SIZE-BOX_COLLISION_WIDTH) or wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
            {
                newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
            }
        }
        //left
        else if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] and wall[int(newX/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)])
        {
            newX = (floor(newX/BLOCK_SIZE) + 1)*BLOCK_SIZE;
            if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] and ((newX - floor(newX/BLOCK_SIZE)*BLOCK_SIZE <= BLOCK_SIZE-BOX_COLLISION_WIDTH) or wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
            {
                newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
            }
        }
        else
        {
            newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
        }
    }
    
    //check box collision
    for (int i = 0; i < boxCount; ++i)
    {
        if (!DeadBox[i] and box[i].GetExist(abs_time) and !box[i].GetRequireCheck())
        {
        // boxes are stepped through in height order so getting current position is all good!
            double boxX = box[i].GetX(abs_time);
            double boxY = box[i].GetY(abs_time);
            if (( newX < boxX+BOX_WIDTH) and (newX+BOX_COLLISION_WIDTH > boxX) and ( newY+BOX_COLLISION_HEIGHT > boxY) and (newY < boxY+BOX_COLLISION_HEIGHT) ) 
            {
                dropped = false;
            }
        }
    }
    
    if (dropped)
    {
        exist[abs_time] = true;
        x[abs_time] = newX;
        y[abs_time] = newY;
        xSpeed[abs_time] = newXspeed;
        ySpeed[abs_time] = newYspeed;
    }
    
    return dropped;
}

bool Box::GetSupported(int time)
{
    return supported[time];   
}

void Box::SetTimeDirection(int newDirection)
{
    timeDirection = newDirection;
}
int Box::GetTimeDirection()
{
    return timeDirection;
}

void Box::UpdateExist(int time)
{
    exist[time] = exist[time-timeDirection];
}

void Box::PhysicsStep(int time)
{
        
    supported[time] = false;
        
    double oldX = x[time-timeDirection];
    double oldY = y[time-timeDirection];
        
    // add GRAVITY
    ySpeed[time] = ySpeed[time-timeDirection] + GRAVITY;
    xSpeed[time] = xSpeed[time-timeDirection];
        
    // new positions for collision checking
    double newX = oldX + xSpeed[time];
    double newY = oldY + ySpeed[time];
        
    //check wall collision in Y direction
    if (ySpeed[time] > 0) // down
    {
        if (wall[int(oldX/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)] or ((oldX - floor(oldX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_WIDTH) and wall[int((oldX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            ySpeed[time] = 0;
            xSpeed[time] = 0;
            newY = floor((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)*BLOCK_SIZE - BOX_COLLISION_HEIGHT;
            supported[time] = true;
        }
    }
    else if (ySpeed[time] < 0) // up
    {
        if (wall[int(oldX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((oldX - floor(oldX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_WIDTH) and wall[int((oldX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
        {
            ySpeed[time] = 0;
            newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
        }
    }
        
    //check wall collision in X direction
    if (xSpeed[time] > 0) // right
    {
        if ( wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newY - floor(newY/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_HEIGHT) and wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            xSpeed[time] = 0;
            newX = floor((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)*BLOCK_SIZE - BOX_COLLISION_WIDTH;
        }
    }
    else if (xSpeed[time] < 0) // left
    {
        if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newY - floor(newY/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_HEIGHT) and wall[int(newX/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)]))
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
            int boxTimeDirection = box[i].GetTimeDirection();
            if (i != id and box[i].GetActive(time) and (box[i].GetCollideable() or boxTimeDirection !=timeDirection) and !box[i].GetRequireCheck())
            {
                // boxes are stepped through in height order so getting current position is all good!
                double boxX = box[i].GetX(time);
                double boxY = box[i].GetY(time);
                double oldBoxY = box[i].GetY(time-timeDirection);
                if (( newX <= boxX+BOX_WIDTH) and (newX+BOX_COLLISION_WIDTH >= boxX) and ( newY+BOX_COLLISION_HEIGHT >= boxY) and (oldY+BOX_COLLISION_HEIGHT <= oldBoxY) ) 
                {
                    xSpeed[time] = box[i].GetXspeed(time)*boxTimeDirection*timeDirection;
                    ySpeed[time] = box[i].GetYspeed(time)*boxTimeDirection*timeDirection;
                    newY = boxY-BOX_COLLISION_HEIGHT;
                    supported[time] = true;
                }
            }
        }
    }
        
    collideable = true;
        
    // set new locations
    x[time] = newX;
    y[time] = newY; 
    
}

void Box::ReversePhysicsStep(int time)
{
    double oldX = x[time-timeDirection];
    double oldY = y[time-timeDirection];    
    
    bool newSupported = false;
       
    // add GRAVITY 
    double newYspeed = ySpeed[time-timeDirection] + GRAVITY;;
    double newXspeed = xSpeed[time-timeDirection];
        
    // new positions for collision checking
    double newX = oldX + newXspeed;
    double newY = oldY + newYspeed;
        
    //check wall collision in Y direction
    if (newYspeed > 0) // down
    {
        if (wall[int(oldX/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)] or ((oldX - floor(oldX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_WIDTH) and wall[int((oldX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            newYspeed = 0;
            newXspeed = 0;
            newY = floor((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)*BLOCK_SIZE - BOX_COLLISION_HEIGHT;
            newSupported = true;
        }
    }
    else if (newYspeed < 0) // up
    {
        if (wall[int(oldX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((oldX - floor(oldX/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_WIDTH) and wall[int((oldX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)]))
        {
            newYspeed = 0;
            newY = (floor(newY/BLOCK_SIZE) + 1)*BLOCK_SIZE;
        }
    }
        
    //check wall collision in X direction
    if (newXspeed > 0) // right
    {
        if ( wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newY - floor(newY/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_HEIGHT) and wall[int((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            newXspeed = 0;
            newX = floor((newX+BOX_COLLISION_WIDTH)/BLOCK_SIZE)*BLOCK_SIZE - BOX_COLLISION_WIDTH;
        }
    }
    else if (newXspeed < 0) // left
    {
        if (wall[int(newX/BLOCK_SIZE)][int(newY/BLOCK_SIZE)] or ((newY - floor(newY/BLOCK_SIZE)*BLOCK_SIZE > BLOCK_SIZE-BOX_COLLISION_HEIGHT) and wall[int(newX/BLOCK_SIZE)][int((newY+BOX_COLLISION_HEIGHT)/BLOCK_SIZE)]))
        {
            newXspeed = 0;
            newX = (floor(newX/BLOCK_SIZE) + 1)*BLOCK_SIZE;
        }
    }
        
    //check Box collision in Y direction
    if (newYspeed > 0) // down
    {
        for (int i = 0; i < boxCount; ++i)
        {
            int boxTimeDirection = box[i].GetTimeDirection();
            if (i != id and box[i].GetActive(time) and (box[i].GetCollideable() or boxTimeDirection != timeDirection) and !box[i].GetRequireCheck())
            {
                // boxes are stepped through in height order so getting current position is all good!
                double boxX = box[i].GetX(time);
                double boxY = box[i].GetY(time);
                double oldBoxY = box[i].GetY(time-timeDirection);
                if (( newX <= boxX+BOX_WIDTH) and (newX+BOX_COLLISION_WIDTH >= boxX) and ( newY+BOX_COLLISION_HEIGHT >= boxY) and (oldY+BOX_COLLISION_HEIGHT <= oldBoxY) ) 
                {
                    newXspeed = box[i].GetXspeed(time)*boxTimeDirection*timeDirection;
                    newYspeed = box[i].GetYspeed(time)*boxTimeDirection*timeDirection;
                    newY = boxY-BOX_COLLISION_HEIGHT;
                    newSupported = true;
                }
            }
        }
    }
        
    collideable = true;
    
    // check against old data
    if (newX == x[time] and newY == y[time] and newXspeed == xSpeed[time] and newYspeed == ySpeed[time] and newSupported == supported[time])
    {
        if (requireReverseCheck)
        {
            requireReverseCheck = 0;
            propManager.AddPropagation(time,timeDirection);
        }
    }
    else
    {
        if (requireReverseCheck and (absoluteTime == 1 and timeDirection == 1) or (absoluteTime == maxTime and timeDirection == -1))
        {
            // propagate new position if end of time is reached regardless of reverse check
            requireReverseCheck = false;
            propManager.AddPropagation(time,timeDirection);   
        }
        else
        {
            requireReverseCheck = time;
        }
    }
    
}

bool Box::GetActive(int time)
{
    return (!DeadBox[id] and exist[time]);
}

void Box::TimeChangeHousekeeping(int oldTime,int oldTimeDir,int newTime,int newTimeDirection)
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

void Box::SetStart(double newX,double newY,double newXspeed,double newYspeed,int abs_time,int direction)
{
    exist[abs_time] = true;
    x[abs_time] = newX;
    y[abs_time] = newY;
    xSpeed[abs_time] = newXspeed;
    ySpeed[abs_time] = newYspeed;
    timeDirection = direction;
}

void Box::unDrawSprite()
{
    if (prevDrawX)
    {
        blit(background ,buffer ,prevDrawX-BLOCK_SIZE,prevDrawY-BLOCK_SIZE,prevDrawX,prevDrawY,32,32);
        prevDrawX = 0;
    }
}

void Box::DrawSprite(int time)
{
    if (GetActive(time)) 
    {
        int drawX = int(x[time]);
        int drawY = int(y[time]);
        
        draw_sprite( buffer, box_sprite, drawX,drawY);
        
        if (timeDirection != absoluteTimeDirection)
            {
                textout_ex( buffer, font, "R", drawX+10, drawY+20, makecol( 255, 255, 0), makecol( 0, 0, 0) );
            }
        
        if (requireReverseCheck)
        {
            textout_ex( buffer, font, ":(", drawX+10, drawY+10, makecol( 255, 255, 0), makecol( 0, 0, 0) );   
        }
        
        prevDrawX = drawX;
        prevDrawY = drawY;
        
    }
}

