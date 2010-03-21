#include "Hourglass_Allegro.h"

class Guy{

public:
    Guy();

    double GetX(int time);
    double GetY(int time);
    double GetXspeed(int time);
    double GetYspeed(int time);
    
    bool GetActive(int time);
    
    int GetStartTime();
    int GetTimeDirection();
 
    int GetOrder();
    void SetOrder(int newOrder);
    
    void ResetParadoxChecking();
    
    void SetId(int newId);

    void DrawSprite(int time); 
    void unDrawSprite();
    
    void SetStart(double newX,double newY,double newXspeed,double newYspeed, bool newCarryingBox, int rel_time,int abs_time, int direction); 
    
    void PhysicsStep(int time);
    void UpdateBoxCarrying(int time);
    void UpdateTimeTravel(int time);

    static void StoreInput(int time);

private:

    int id; // array number in guy array
    
    // where drawing occured last time
    int prevDrawX;
    int prevDrawY;
    bool prevDrawHead; 
    //Was something (arrow, box etc..) drawn above my head? 
    //Then I guess I'd better erase 'else it will clutter up the next frame.
    
    // position in absolute time
    double x[5400];
    double y[5400];
    double xSpeed[5400];
    double ySpeed[5400];
    
    // departure variables
    int departureX;
    int departureY;
    int departureXspeed;
    int departureYspeed;
    bool departureCarrying;
    
    int depatureTimeDestination;
    
    int paradoxCheckX[50];
    int paradoxCheckY[50];
    int paradoxCheckXspeed[50];
    int paradoxCheckYspeed[50];
    bool paradoxCheckCarrying[50];
    int paradoxChecks;
    
    // start times, for spawning
    int startAbsTime;
    int startRelTime;
    
    bool carryingBox[5400]; // Am I carrying a box? Well, am I?
    int carryBoxId[5400]; // ID of box
    
    int timeOffset; // difference between start rel and abs time
    
    // end times, for paradox checking
    int endAbsTime;
    int endRelTime;
    
    // -1 = backward, 0 = pause, 1 = forward
    int timeDirection;
    
    bool playerCharacter; // ie the oldest instance
    
    int order; // order that the guy is in. ie 1 = oldest, 2 = second oldest...
        
    int subimage[5400];
    
    // animation variables
    bool draw_moving;
    bool draw_facing; // left = false, right = true
        
        


     
};
