#include "Hourglass_Allegro.h"

class Box{

public:
    Box();
    
    // box variables
    static const int BOX_WIDTH = 32;
    static const int BOX_HEIGHT = 32;
    
    double GetX(int time);
    double GetY(int time);
    double GetXspeed(int time);
    double GetYspeed(int time);
    
    bool GetActive(int time);
    
    void SetId(int newId);

    void DrawSprite(int time); 
    void unDrawSprite();

    int GetTimeDirection();
    void SetTimeDirection(int newDirection);
    
    bool GetSupported(int time);
    
    void SetStart(double newX,double newY,double newXspeed,double newYspeed,int abs_time, int direction); 
    
    void TimeChangeHousekeeping(int oldTime,int oldTimeDir,int newTime,int newTimeDirection);
    
    void SetExist(int abs_time,bool state);
    bool GetExist(int abs_time);
    
    void SetCollideable(bool state);
    bool GetCollideable();
    
    bool GetRequireCheck();
    
    bool DropBox(double newX,double newY,double newXspeed,double newYspeed,int abs_time);
    static bool CanDropBox(double newX,double newY,double newXspeed,double newYspeed,int abs_time);
    
    void UpdateExist(int time);
    
    void PhysicsStep(int time);
    void ReversePhysicsStep(int time);

private:
    
    int id; // array number in box array
    
    // where drawing occured last time
    int prevDrawX;
    int prevDrawY;

    // position in absolute time
    double x[5400];
    double y[5400];
    double xSpeed[5400];
    double ySpeed[5400];
    
    bool collideable; // has the box steped so that it can now be hit
    
    // for undetermined reverse time guys. set in ReversePhysicsStep
    int requireReverseCheck;
    
    bool exist[5400];
    
    int timeDirection;
    
    bool supported[5400]; // checked every frame before guys and other boxes land, no large array

    // collision width and height. it is the same as BOX_WIDTH but it makes pretty code and portable from guy physics ;D.    
    static const int BOX_COLLISION_WIDTH = BOX_WIDTH;
    static const int BOX_COLLISION_HEIGHT = BOX_HEIGHT;
    
    //static const 
	double GRAVITY;// = 0.17;
};
