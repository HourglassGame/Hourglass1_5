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

const int MAX_TIME = 3000; // should be 5400 for 3 minutes, 3000 is nice for now

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
            
            propagating = true;
            relativeTime++;
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
                    changeTime = true;
                    newTime = propStartTime[queuedProps-1];
                    newTimeDirection = propDirection[queuedProps-1];
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
            if ( (absoluteTime == 1 and propDirection[queuedProps-1] == -1) or (absoluteTime == MAX_TIME and propDirection[queuedProps-1] == 1) )
            {
                queuedProps--;
                if (queuedProps)
                {
                    changeTime = true;
                    newTime = propStartTime[queuedProps-1];
                    newTimeDirection = propDirection[queuedProps-1];
                }
                else
                {
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
