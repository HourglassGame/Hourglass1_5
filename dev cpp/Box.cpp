#include "Box.h"
#include <math.h>
#include <iostream> 

extern BITMAP* box_sprite;

extern BITMAP* background;
extern BITMAP* foreground;
extern BITMAP* buffer;

// wall segment count within level
const int level_width = 32;
const int level_height = 21;
const int block_size = 32;

extern bool wall[level_width][level_height];
extern int boxCount;
extern Box box[];

const double gravity = 0.17;

// box variables
const int boxWidth = 32;
const int boxHeight = 32;

// collision width and height. it is the same as boxWidth but it makes pretty code and portable from guy physics ;D.
const int cWidth = 32;
const int cHeight = 32;

Box::Box()
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

void Box::SetCarried(int abs_time)
{
    carried[abs_time] = true;
}
bool Box::GetCarried(int abs_time)
{
    return carried[abs_time];
}

void Box::DropBox(double newX,double newY,double newXspeed,double newYspeed,int abs_time)
{
    carried[abs_time] = false;
    x[abs_time] = newX;
    y[abs_time] = newY;
    xSpeed[abs_time] = newXspeed;
    ySpeed[abs_time] = newYspeed;
}

int Box::GetStartAbsTime()
{
    return startAbsTime;
}
int Box::GetEndAbsTime()
{
    return endAbsTime;
}

void Box::ForwardTimeStep(int time)
{
    
    // only move if past start time
    if (!carried[time] and time > startAbsTime and (!endAbsTime or time <= endAbsTime))
    {
        carried[time+1] = false;
        
        double oldX = x[time-1];
        double oldY = y[time-1];
        
        // add gravity
        ySpeed[time] = ySpeed[time-1] + gravity;
        xSpeed[time] = xSpeed[time-1];
        
        // new positions for collision checking
        double newX = oldX + xSpeed[time];
        double newY = oldY + ySpeed[time];
        
        //check wall collision in Y direction
        if (ySpeed[time] > 0) // down
        {
            if (wall[int(oldX/block_size)][int((newY+cHeight)/block_size)] or ((oldX - floor(oldX/block_size)*block_size > block_size-cWidth) and wall[int((oldX+cWidth)/block_size)][int((newY+cHeight)/block_size)]))
            {
                ySpeed[time] = 0;
                xSpeed[time] = 0;
                newY = floor((newY+cHeight)/block_size)*block_size - cHeight;
            }
        }
        else if (ySpeed[time] < 0) // up
        {
            if (wall[int(oldX/block_size)][int(newY/block_size)] or ((oldX - floor(oldX/block_size)*block_size > block_size-cWidth) and wall[int((oldX+cWidth)/block_size)][int(newY/block_size)]))
            {
                ySpeed[time] = 0;
                newY = (floor(newY/block_size) + 1)*block_size;
            }
        }
        
        //check wall collision in X direction
        if (xSpeed[time] > 0) // right
        {
            if ( wall[int((newX+cWidth)/block_size)][int(newY/block_size)] or ((newY - floor(newY/block_size)*block_size > block_size-cHeight) and wall[int((newX+cWidth)/block_size)][int((newY+cHeight)/block_size)]))
            {
                xSpeed[time] = 0;
                newX = floor((newX+cWidth)/block_size)*block_size - cWidth;
            }
        }
        else if (xSpeed[time] < 0) // left
        {
            if (wall[int(newX/block_size)][int(newY/block_size)] or ((newY - floor(newY/block_size)*block_size > block_size-cHeight) and wall[int(newX/block_size)][int((newY+cHeight)/block_size)]))
            {
                xSpeed[time] = 0;
                newX = (floor(newX/block_size) + 1)*block_size;
            }
        }
        
        //check Box collision in Y direction
        if (ySpeed[time] > 0) // down
        {
            for (int i = 0; i < boxCount; ++i)
            {
                if (i != id and time > box[i].GetStartAbsTime() and (!box[i].GetEndAbsTime() or time <= box[i].GetEndAbsTime() ))
                {
                    // boxes are stepped through in height order so getting current position is all good!
                    double boxX = box[i].GetX(time);
                    double boxY = box[i].GetY(time);
                    if (( newX <= boxX+boxWidth) and (newX+cWidth >= boxX) and ( newY+cHeight >= boxY) and (oldY+cHeight <= boxY) ) 
                    {
                        xSpeed[time] = 0;
                        ySpeed[time] = 0;
                        newY = boxY-cHeight;
                    }
                }
            }
        }
        
        // set new locations
        x[time] = newX;
        y[time] = newY; 
        
        if (time == endAbsTime)
        {
            // paradox checking goes here eventually
            endAbsTime = 0;
        }
        
    }
    
}


void Box::SetStart(double newX,double newY,double newXspeed,double newYspeed,int abs_time)
{
    startAbsTime = abs_time;
    x[abs_time] = newX;
    y[abs_time] = newY;
    xSpeed[abs_time] = newXspeed;
    ySpeed[abs_time] = newYspeed;
}

void Box::SetEnd(int newEndType, int abs_time)
{
    endAbsTime = abs_time;
    endType = newEndType;
}

void Box::unDrawSprite()
{
    if (prevDrawX)
    {
        blit(background ,buffer ,prevDrawX-block_size,prevDrawY-block_size,prevDrawX,prevDrawY,32,32);
        prevDrawX = 0;
    }
}

void Box::DrawSprite(int time)
{
    if (!carried[time] and time > startAbsTime and (!endAbsTime or time <= endAbsTime)) 
    {
        int drawX = int(x[time]);
        int drawY = int(y[time]);
        
        draw_sprite( buffer, box_sprite, drawX,drawY);
        
        prevDrawX = drawX;
        prevDrawY = drawY;
        
    }
}

