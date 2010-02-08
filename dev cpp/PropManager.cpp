#include "PropManager.h"

extern bool propagating;
extern int absoluteTime;

PropManager::PropManager()
{
}

void PropManager::CreatePropagation(int start_time, int end_time)
{
    propStartTime[queuedProps] = start_time;
    propEndTime[queuedProps] = end_time;
    
    if (queuedProps)
    {
        propStartTime[queuedProps-1] = absoluteTime;// unscaleable bit
    }
    absoluteTime = start_time;
    propagating = true;
    
    queuedProps++;
}



bool PropManager::UpdatePropagation()
{
    if (queuedProps)
    {
        propagating = true;
        if (propEndTime[queuedProps-1] <= absoluteTime)
        {
            queuedProps--;
            if (queuedProps)
            {
                absoluteTime = propStartTime[queuedProps-1];
            }
            else
            {
                propagating = false;
            }
        }
        
        return false;
    }
    else
    {
        return true;
        propagating = false;
    }
}
