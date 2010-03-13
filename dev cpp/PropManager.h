#include <allegro.h>

class PropManager{

public:
    PropManager();
    
    void CreatePropagation(int start_time, int end_time);
    bool UpdatePropagation(); // returns whether propagation is over
    
private:
        
    int propStartTime[50];
    int propEndTime[50];
    
    int queuedProps;
    
    
};

    
