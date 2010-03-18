#include "PropManager.h"

extern bool propagating;
extern int absoluteTime;
extern int relativeTimeDirection;

const int MAX_TIME = 5399; // update from main

PropManager::PropManager()
{
}

void PropManager::AddPropagation(int start_time, int direction)
{
    
    // if the start time is in the relative past
    if ((queuedProps and start_time*presentDirection < presentTime*presentDirection) or start_time*relativeTimeDirection < absoluteTime*relativeTimeDirection)
    {
        if (!queuedProps)
        {
            // if there are no queued propagations record the time and direction as the present time and present direction 
            // then create a propagation in the past
            presentDirection = relativeTimeDirection;
            presentTime = absoluteTime;
            
            propStartTime[queuedProps] = start_time;
            propDirection[queuedProps] = relativeTimeDirection;
            
            absoluteTime = start_time;
            relativeTimeDirection = direction;
            
            propagating = true;
            queuedProps++;
        }
        else
        {
            if (propDirection[queuedProps-1] == direction)
            {
                // if the new and current propagation is in the same direction
                if (start_time*direction < absoluteTime*direction)
                {
                    // push the propagation start time into the past relative to the propagation direction
                    propStartTime[queuedProps-1] = start_time;
                    absoluteTime = start_time;
                }
                // if the new propagation is in the future relative to the propagation direction there is no need to create a new one
            }
            else
            {
                // if the progation directions are not the same a new progation will be needed
                propStartTime[queuedProps-1] = absoluteTime; // once the new one is done the current one will resume at this point
                
                propStartTime[queuedProps] = start_time;
                propDirection[queuedProps] = direction;
                
                absoluteTime = start_time;
                relativeTimeDirection = direction;
                queuedProps++;
            }
        }
        
    }
    else
    {
        // if the propagation is in the relative future store it.
        if (relativeTimeDirection == -direction)
        {
            futureReversePropStartTime[futureProps] = start_time;
            futureProps++;
        }
    }
    
}



bool PropManager::UpdatePropagation()
{
    if (queuedProps)
    {
        if (propDirection[queuedProps-1] == presentDirection)
        {
            if (presentTime*presentDirection <= absoluteTime*presentDirection)
            {
                queuedProps--;
                if (queuedProps)
                {
                    absoluteTime = propStartTime[queuedProps-1];
                    relativeTimeDirection = propDirection[queuedProps-1];
                }
                else
                {
                    propagating = false;
                    return true;
                }
            }
        }
        else
        {
            if ( (absoluteTime == 0 and propDirection[queuedProps-1] == -1) or (absoluteTime == MAX_TIME and propDirection[queuedProps-1] == 1) )
            {
                queuedProps--;
                if (queuedProps)
                {
                    absoluteTime = propStartTime[queuedProps-1];
                    relativeTimeDirection = propDirection[queuedProps-1];
                }
                else
                {
                    absoluteTime = presentTime;
                    relativeTimeDirection = presentDirection;
                    
                    propagating = false;
                    return true;
                }
            }
        }
        
    }
    return false;

}
