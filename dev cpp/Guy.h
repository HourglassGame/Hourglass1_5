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
    
    void SetStart(double newX,double newY,double newXspeed,double newYspeed, int newCarryingBox, int oldCarryBoxId, int rel_time,int abs_time, int direction, int newSubimage); 
    
    void TimeChangeHousekeeping(int oldTime,int oldTimeDir,int newTime,int newTimeDirection);
    
    bool InsideWall(int time);
    
    void PhysicsStep(int time);
    void ReversePhysicsStep(int time);
    
    void UpdateBoxCarrying(int time);
    void UpdateTimeTravel(int time);
    
    void StartReverseCheck(int time);

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
    
    // paradox checking functions
    
    // checks data at time against paradox data at time for equal data. Equal data = paradox triggered
    void CheckForParadox(int time, int boxCarry, int otherTime); 
    // adds paradox data at time from data at time
    void AddParadoxCheck(int time, int boxCarry, int otherTime);
    // adds important time data at time
    void AddImportantTime(int time, int boxCarry, int otherTime);
    // checks if data at time differs from important data at time
    bool TimeDiffersFromImportantTime(int time, int otherTime);
    
    // important times, for paradox checking
    int importantTime[5400];
    int importantX[5400];
    int importantY[5400];
    int importantXspeed[5400];
    int importantYspeed[5400];
    int importantCarrying[5400];
    int importantOtherTime[5400];
    
    // previous paradox information
    int paradoxCheckTime[50];
    int paradoxCheckX[50];
    int paradoxCheckY[50];
    int paradoxCheckXspeed[50];
    int paradoxCheckYspeed[50];
    int paradoxCheckCarrying[50];
    int paradoxCheckOtherTime[50]; // for checking destination, -1 if no check is wanted
    int paradoxChecks;
    
    // for undetermined reverse time guys. set in ReversePhysicsStep
    int requireReverseCheck;
    
    // start times, for spawning
    int startAbsTime;
    int startRelTime;
    
    int carryingBox[5400]; // -1 = just dropped, 0 = not carrying, 1 = carrying, 2 = just picked up
    int carryBoxId[5400]; // ID of box, -1 = carrying reverse box
    int dropReverseBoxID[5400]; // ID of reverse box that was dropped 
    
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
