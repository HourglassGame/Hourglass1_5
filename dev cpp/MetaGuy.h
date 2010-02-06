#include <allegro.h>

/*
There will be one instance of this class 
It stores input in RELATIVE TIME
This way, when the travel positions change, input etc.. do not need moving
The contents of this class should be the only thing required to construct a replay (well, and the level file).
*/

class MetaGuy{

public:
    MetaGuy();
    
    bool inputLeft[9000];
    bool inputRight[9000];
    bool inputUp[9000];
    bool inputDown[9000];
    
    int inputSpecial[9000];
    int inputSpecialArg1[9000];
    // store attempted events, eg: jump to X, or jump X back in time.
    // 0 = no action
    // 1 = free chronoport with time arg1
    
    void GetInput(int time);

protected:
        
};
