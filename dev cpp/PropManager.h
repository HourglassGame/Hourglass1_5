#include "Hourglass_Allegro.h"

class PropManager{

public:
    PropManager();
    
    void PlayerTravel(int start_time, int propDirection);
    void AddPropagation(int start_time, int propDirection);
    bool UpdatePropagation(); // returns whether propagation is over
     
    int GetQueuedProps();
     
private:
    
    int reverseJumpToFutureTime; // indicates that a reverse jump to future has happened    
    
    int presentTime; // the time that the player is at
    int presentDirection; // direction the player is traveling in
        
    int propStartTime[5000];
    int propDirection[5000]; // -1 = backward, 0 = pause, 1 = forward
    
    int newPlayerTime; // time that the player wants to go to
    int newPlayerDirection; // direction that the player wants to go to
    
    int queuedProps;
    
    int futureReversePropStartTime[5000];
    int futureProps;
};

    
