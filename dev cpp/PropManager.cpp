#include "PropManager.h"

// variables (mainly) read from main
extern bool propagating;
extern int absoluteTime;
extern int absoluteTimeDirection;
extern int relativeTime;

// variables that main must know
extern bool changeTime;
extern int newTime;
extern int newTimeDirection;
extern bool waitForDraw;

extern int maxTime; // max abs time

PropManager::PropManager()
{
}

int PropManager::GetQueuedProps()
{
    return queuedProps;   
}

void PropManager::AddPropagation(int start_time, int direction)
{
    
    // if the start time is in the relative past
    if ((queuedProps and start_time*presentDirection < presentTime*presentDirection) or start_time*absoluteTimeDirection < absoluteTime*absoluteTimeDirection)
    {
        if (!queuedProps)
        {
            // if there are no queued propagations record the time and direction as the present time and present direction 
            // then create a propagation in the past
            presentDirection = absoluteTimeDirection;
            presentTime = absoluteTime+absoluteTimeDirection;
            
            propStartTime[queuedProps] = start_time;
            propDirection[queuedProps] = direction;
            
            changeTime = true;
            newTime = start_time;
            newTimeDirection = direction;
            
            queuedProps++;
        }
        else
        {
            if (propDirection[queuedProps-1] == direction) // if the new and current propagation is in the same direction
            {
                if (start_time*direction < absoluteTime*direction)
                {
                    // push the propagation start time into the past relative to the propagation direction
                    propStartTime[queuedProps-1] = start_time;
                    newTime = start_time;
                }
                // if the new propagation is in the future relative to the propagation direction there is no need to create a new one
            }
            else
            {
                // if the progation directions are not the same a new progation will be needed
                propStartTime[queuedProps-1] = absoluteTime; // once the new one is done the current one will resume at this point
                
                propStartTime[queuedProps] = start_time;
                propDirection[queuedProps] = direction;
                
                changeTime = true;
                newTime = start_time;
                newTimeDirection = direction;
                
                queuedProps++;
            }
        }
        
    }
    else
    {
        // if the propagation is in the relative future store it.
        if (absoluteTimeDirection == -direction)
        {
            futureReversePropStartTime[futureProps] = start_time;
            futureProps++;
        }
    }
    
}

void PropManager::PlayerTravel(int start_time, int direction)
{
    
    
    // if the start time is in the relative past (or at the present)
    if (start_time*absoluteTimeDirection <= absoluteTime*absoluteTimeDirection)
    {
        if (absoluteTimeDirection != direction) // if time is to be reversed
        {
            // set the present in the past in the opposite direction
            presentTime = start_time;
            presentDirection = direction;
            
            // propagate from the departure time to the end
            propStartTime[queuedProps] = start_time;
            propDirection[queuedProps] = -presentDirection;
            
            changeTime = true;
            newTime = start_time;
            newTimeDirection = -presentDirection;
            
            queuedProps++;
        }
        else // if time is not to be reversed
        {
            // no propagation required
            changeTime = true;
            newTime = start_time+direction;
            newTimeDirection = direction;
        }
    }
    else // if the start time is in the relative future
    {
        if (absoluteTimeDirection != direction) // if time is to be reversed
        {
            // set the present to the departure time
            presentTime = absoluteTime;
            presentDirection = direction;
            
            // once propagation is done the present will be set to this time
            reverseJumpToFutureTime = start_time;
            
            // propagate what is now the past in reverse
            propStartTime[queuedProps] = absoluteTime;
            propDirection[queuedProps] = absoluteTimeDirection;
            
            changeTime = true;
            newTime = absoluteTime;
            newTimeDirection = absoluteTimeDirection;
            
            queuedProps++;
        }
        else // if time is not to be reversed
        {
            // set present to future
            presentTime = start_time;
            presentDirection = direction;
            
            // propagate to the future
            propStartTime[queuedProps] = absoluteTime;
            propDirection[queuedProps] = absoluteTimeDirection;
            
            changeTime = true;
            newTime = absoluteTime;
            newTimeDirection = absoluteTimeDirection;
            
            queuedProps++;
        }
    }
    
}


bool PropManager::UpdatePropagation()
{
    
    if (queuedProps) // if there are propagations queued
    {
        if (propDirection[queuedProps-1] == presentDirection) // if the most recent propagation is relative forwards
        {
            if (presentTime*presentDirection <= absoluteTime*presentDirection) // if the abs time is in the relative past
            {
                queuedProps--; // finish current propagation
                if (queuedProps)
                {
                    // start the next propagation
                    changeTime = true;
                    newTime = propStartTime[queuedProps-1];
                    newTimeDirection = propDirection[queuedProps-1];
                }
                else
                {
                    // propagation is over
                    propagating = false;
                    return true;
                }
            }
        }
        else // if the most recent propagation is relative reverse
        {
            if ( (absoluteTime == 1 and propDirection[queuedProps-1] == -1) or (absoluteTime == maxTime and propDirection[queuedProps-1] == 1) )
            // abs time has reached the far past
            {
                queuedProps--; // finish current propagation
                if (queuedProps)
                {
                    // start the next propagation
                    changeTime = true;
                    newTime = propStartTime[queuedProps-1];
                    newTimeDirection = propDirection[queuedProps-1];
                }
                else
                {
                    // propagation is over return to present
                    if (reverseJumpToFutureTime)
                    {
                        // set a different present if the player jump triggered a reverse future propagation
                        presentTime = reverseJumpToFutureTime;
                        reverseJumpToFutureTime = 0;
                    }
                    changeTime = true;
                    newTime = presentTime;
                    newTimeDirection = presentDirection;
                    
                    propagating = false;
                    return true;
                }
            }
        }
        
    }
    return false;

}
