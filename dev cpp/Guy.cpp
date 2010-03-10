#include "Guy.h"
#include "Box.h"
#include "PropManager.h"
//#include "MetaGuy.h"
#include <math.h>
#include <iostream> 
#include <vector>
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
//extern Guy guy[];
extern std::vector<Guy> guys;
extern int boxCount;
//extern Box box[];
extern std::vector<Box> boxes;

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
    
static std::vector<bool> inputLeft;
static std::vector<bool> inputRight;
static std::vector<bool> inputUp;
static std::vector<bool> inputDown;
static std::vector<int> inputSpecial;
static std::vector<int> inputSpecialArg1;
//static bool inputLeft[9000];
//static bool inputRight[9000];
//static bool inputUp[9000];
//static bool inputDown[9000];

// store attempted events, eg: jump to X, or jump X back in time.
// 0 = no action
// 1 = free chronoport with time arg1
//static int inputSpecial[9000];
//static int inputSpecialArg1[9000];

Guy::Guy()
{
    departureX = -1;
    departureY = -1;
    departureXspeed = -1;
    departureYspeed = -1;
    departureCarrying = false;
}

void Guy::StoreInput(int time)
{
    if (time >= inputLeft.size())
    {
        inputLeft.resize((time+1)*RESIZE_FACTOR);
    }
    if (time >= inputRight.size())
    {
        inputRight.resize((time+1)*RESIZE_FACTOR);
    }
    if (time >= inputUp.size())
    {
        inputUp.resize((time+1)*RESIZE_FACTOR);
    }
    if (time >= inputDown.size())
    {
        inputDown.resize((time+1)*RESIZE_FACTOR);
    }
    if (time >= inputSpecial.size())
    {
        inputSpecial.resize((time+1)*RESIZE_FACTOR);
    }
    if (time >= inputSpecialArg1.size())
    {
        inputSpecialArg1.resize((time+1)*RESIZE_FACTOR);
    }
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
    if (time >= x.size())
    {
        x.resize((time+1)*RESIZE_FACTOR);
    }
    if (time >= y.size())
    {
        y.resize((time+1)*RESIZE_FACTOR);
    }
    if (time >= xSpeed.size())
    {
        xSpeed.resize((time+1)*RESIZE_FACTOR);
    }
    if (time >= ySpeed.size())
    {
        ySpeed.resize((time+1)*RESIZE_FACTOR);
    }
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
            if (boxes[i].GetActive(time))
            {
                    double boxX = boxes[i].GetX(time);
                    double boxY = boxes[i].GetY(time);
                    if (( newX <= boxX+Box::BOX_WIDTH) and (newX+GUY_COLLISION_WIDTH >= boxX) and ( newY+GUY_COLLISION_HEIGHT >= boxY) and (oldY+GUY_COLLISION_HEIGHT <= boxY) )     
                    {
                        ySpeed[time] = boxes[i].GetYspeed(time);
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
    if (time >= carryingBox.size())
    {
        carryingBox.resize((time+1)*RESIZE_FACTOR);
    }
    if (time >= carryBoxId.size())
    {
        carryBoxId.resize((time+1)*RESIZE_FACTOR);
    }
     // input is in relative time
    int personalTime = time-timeOffset;
    
    //pickup or drop box
    if (inputDown[personalTime] and !inputDown[personalTime-1]) // down
    {
        if (carryingBox[time-1])
        {
            if (boxes[carryBoxId[time-1]].DropBox(x[time]+BOX_CARRY_OFFSET_X,y[time]+BOX_CARRY_OFFSET_Y,0,0,time) )
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
                if (boxes[i].GetActive(time))
                {
                    double boxX = boxes[i].GetX(time-1);
                    double boxY = boxes[i].GetY(time-1);
                    if (( x[time-1] < boxX+Box::BOX_WIDTH) and (x[time]+GUY_COLLISION_WIDTH > boxX) and ( y[time]+GUY_COLLISION_HEIGHT > boxY) and (y[time] < boxY+Box::BOX_HEIGHT) )     
                    {
                        boxes[i].SetExist(time,false);
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
            //allegro_message("d2p");
            int portTime = inputSpecialArg1[personalTime];
            //Guy newGuy;// = Guy();
          //  newGuy.SetStart(x[time],y[time],xSpeed[time],ySpeed[time],carryingBox[time],relativeTime,portTime);
            //guys.push_back(Guy());
            //allegro_message("guyCount = %d",guyCount);
            //allegro_message("guys.size() = %d",guys.size());
            //allegro_message("guys.capacity() = %d",guys.capacity());
            guys[guyCount].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],carryingBox[time],relativeTime,portTime);
            //try
              //  {
                //guys.at(guyCount).SetStart(double(50),double(50),double(0),double(0),false,int(5),int(50));
                //}
                //catch(...)
                //{
                  //  allegro_message("An Exception Occured");
                  //  allegro_message("%s",e.what());
                //}
            //allegro_message("grrrr");
            guys[guyCount].SetOrder(guyCount);
            //allegro_message("grrrr");
            if (absoluteTime < portTime)
            {
                propManager.CreatePropagation(absoluteTime,portTime);
            }
            else
            {
                absoluteTime = portTime;
            }
            //allegro_message("grrrr2");
            //allegro_message("%f",x[time]);
            //allegro_message("%d",int(x[time]));
            //allegro_message("%d",departureX);
            departureX = int(x[time]);
            //allegro_message("grrrr2.1");
            departureY = int(y[time]);
            //allegro_message("grrrr3");
            departureXspeed = int(xSpeed[time]);
            departureYspeed = int(ySpeed[time]);
            //allegro_message("grrrr4.5");
            departureCarrying = carryingBox[time];
            depatureTimeDestination = portTime;
            //allegro_message("grrrr5");
            guyCount++;
            
            endAbsTime = time;
            endRelTime = relativeTime;
        }
        else if (order + 1 < guys.size())
        {
            //allegro_message("d1p");
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
                
                //guys[order+1].SetStart(x[time],y[time],xSpeed[time],ySpeed[time],carryingBox[time],endRelTime,depatureTimeDestination);
                try
                {
                guys.at(order+1).SetStart(double(50),double(50),double(0),double(0),false,int(5),int(50));
                }
                catch(...)
                {
                    allegro_message("An Exception Occured");
                  //  allegro_message("%s",e.what());
                }
                if (absoluteTime > depatureTimeDestination)
                {
                    propManager.CreatePropagation(depatureTimeDestination,absoluteTime);
                }
                
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
        else
        {
            allegro_message("dop");
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

void Guy::SetStart(double newX,double newY,double newXspeed,double newYspeed,bool newCarryingBox,int rel_time,int abs_time)
{
    if (abs_time >= x.size())
    {
        x.resize((abs_time+1)*RESIZE_FACTOR);
    }
    if (abs_time >= y.size())
    {
        y.resize((abs_time+1)*RESIZE_FACTOR);
    }
    if (abs_time >= xSpeed.size())
    {
        xSpeed.resize((abs_time+1)*RESIZE_FACTOR);
    }
    if (abs_time >= ySpeed.size())
    {
        ySpeed.resize((abs_time+1)*RESIZE_FACTOR);
    }
    if (abs_time >= carryBoxId.size())
    {
        carryBoxId.resize((abs_time+1)*RESIZE_FACTOR);
    }
    if (abs_time >= carryingBox.size())
    {
        carryingBox.resize((abs_time+1)*RESIZE_FACTOR);
    }
    //allegro_message("grrrr1");
    startRelTime = rel_time;
    startAbsTime = abs_time;
    x[abs_time] = newX;
    y[abs_time] = newY;
    xSpeed[abs_time] = newXspeed;
    ySpeed[abs_time] = newYspeed;
    
    if (newCarryingBox and !carryingBox[abs_time]) // create box
    {
        for (int i = 0; i < boxCount; ++i)
        {
            if (DeadBox[i])
            {
                boxes[i] = MintConditionBox;
                carryBoxId[abs_time] = i;
                //box[i].SetStart(0,0,0,0,abs_time);
                boxes[i].SetId(i);
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
            boxes[boxCount].SetId(boxCount);
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

