#include "Guy.h"
#include "MetaGuy.h"
#include <math.h>
#include <iostream> 

extern BITMAP* guy_left;
extern BITMAP* guy_right;
extern BITMAP* guy_left_stop;
extern BITMAP* guy_right_stop;
extern BITMAP* buffer;

// wall segment count within level
const int level_width = 32;
const int level_height = 21;
const int block_size = 32;

extern bool wall[level_width][level_height];
extern MetaGuy metaguy;
extern Guy guy[];

extern int guyCount;

extern int relativeTime;
extern int absoluteTime;
extern int propagationAim;

const double moveSpeed = 3.6;
const double jumpSpeed = 8;
const double gravity = 0.17;

const int cWidth = 24;
const int cHeight = 32;

Guy::Guy()
{
    
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
void Guy::ForwardTimeStep(int time)
{
    // input is in relative time
    int absTime = time-timeOffset;
    
    // only move if past start time
    if (time > startAbsTime and (!endAbsTime or time <= endAbsTime))
    {
        bool jump = false; // is it allowed to jump?
        double oldX = x[time-1];
        double oldY = y[time-1];
        
        // set xspeed from input
        if (metaguy.inputLeft[absTime])
        {
            draw_moving = true;
            draw_facing = false;
            xSpeed[time] = -moveSpeed;
        }
        else if (metaguy.inputRight[absTime])
        {
            draw_moving = true;
            draw_facing = true;
            xSpeed[time] = moveSpeed;
        }
        else
        {
            draw_moving = false;
            xSpeed[time] = 0;
        }
        
        // add gravity
        ySpeed[time] = ySpeed[time-1] + gravity;
        
        // new positions for collision checking
        double newX = oldX + xSpeed[time];
        double newY = oldY + ySpeed[time];
        
        //check wall collision in Y direction
        if (ySpeed[time] > 0) // down
        {
            if (wall[int(oldX/block_size)][int((newY+cHeight)/block_size)] or ((oldX - floor(oldX/block_size)*block_size > block_size-cWidth) and wall[int((oldX+cWidth)/block_size)][int((newY+cHeight)/block_size)]))
            {
                ySpeed[time] = 0;
                newY = floor((newY+cHeight)/block_size)*block_size - cHeight;
                jump = true;
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
        
        // set new locations
        x[time] = newX;
        y[time] = newY;
        
        // jump next step
        if (metaguy.inputUp[absTime] and jump)
        {
            ySpeed[time] = -jumpSpeed;
        }
        
        // time travel
        
        if (metaguy.inputSpecial[absTime] == 1)
        {
            if (order == guyCount)
            {
                int portTime = metaguy.inputSpecialArg1[absTime];
                guy[guyCount].SetStart(guy[guyCount-1].GetX(absoluteTime),guy[guyCount-1].GetY(absoluteTime),guy[guyCount-1].GetXspeed(absoluteTime),guy[guyCount-1].GetYspeed(absoluteTime),relativeTime,portTime);
                guy[guyCount].SetOrder(guyCount+1);
                if (absoluteTime < portTime)
                {
                    propagationAim = portTime;
                }
                else
                {
                    absoluteTime = portTime;
                }
                guyCount++;
                endAbsTime = time;
            }
            else
            {
                endAbsTime = time;
            }
        }     
        
    }
    
}


void Guy::SetStart(double newX,double newY,double newXspeed,double newYspeed,int rel_time,int abs_time)
{
    startRelTime = rel_time;
    startAbsTime = abs_time;
    x[abs_time] = newX;
    y[abs_time] = newY;
    xSpeed[abs_time] = newXspeed;
    ySpeed[abs_time] = newYspeed;
    
    timeOffset = abs_time - rel_time;
}

void Guy::unDrawSprite()
{
    if (prevDrawX)
    {
        rectfill( buffer ,prevDrawX , prevDrawY, prevDrawX+23, prevDrawY+31, makecol ( 0, 0, 0));
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
        
        prevDrawX = drawX;
        prevDrawY = drawY;
        
        subimage++;
        if (subimage > 14)
        {
            subimage = 0;
        }
    }
}

