#include "Box.h"
#include <math.h>
#include <iostream> 

extern BITMAP* box_sprite;

extern BITMAP* background;
extern BITMAP* foreground;
extern BITMAP* buffer;

// wall segment count within level
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 19;
const int BLOCK_SIZE = 32;

extern bool wall[LEVEL_WIDTH][LEVEL_HEIGHT];
extern int boxCount;
extern Box box[];

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
        if (!DeadBox[i] and box[i].GetExist(abs_time))
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

bool Box::GetSupported()
{
    return supported;   
}
int Box::GetTimeDirection()
{
    return timeDirection;
}

void Box::UpdateExist(int time)
{
    exist[time] = exist[time-1];
}

void Box::PhysicsStep(int time)
{
    
    // only move if existing
    if (GetActive(time))
    {
        
        supported = false;
        
        double oldX = x[time-1];
        double oldY = y[time-1];
        
        // add GRAVITY
        ySpeed[time] = ySpeed[time-1] + GRAVITY;
        xSpeed[time] = xSpeed[time-1];
        
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
                supported = true;
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
                if (i != id and box[i].GetActive(time) and box[i].GetCollideable())
                {
                    // boxes are stepped through in height order so getting current position is all good!
                    double boxX = box[i].GetX(time);
                    double boxY = box[i].GetY(time);
                    if (( newX <= boxX+BOX_WIDTH) and (newX+BOX_COLLISION_WIDTH >= boxX) and ( newY+BOX_COLLISION_HEIGHT >= boxY) and (oldY+BOX_COLLISION_HEIGHT <= boxY) ) 
                    {
                        xSpeed[time] = box[i].GetXspeed(time);
                        ySpeed[time] = box[i].GetYspeed(time);
                        newY = boxY-BOX_COLLISION_HEIGHT;
                        supported = true;
                    }
                }
            }
        }
        
        collideable = true;
        
        // set new locations
        x[time] = newX;
        y[time] = newY; 
       
    }
    
}

bool Box::GetActive(int time)
{
    return (!DeadBox[id] and exist[time]);
}


void Box::SetStart(double newX,double newY,double newXspeed,double newYspeed,int abs_time)
{
    exist[abs_time] = true;
    x[abs_time] = newX;
    y[abs_time] = newY;
    xSpeed[abs_time] = newXspeed;
    ySpeed[abs_time] = newYspeed;
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
        
        prevDrawX = drawX;
        prevDrawY = drawY;
        
    }
}

