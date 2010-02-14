#include <allegro.h>

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
    
    int GetStartAbsTime();
    int GetEndAbsTime();
    
    bool GetSupported();
    
    void SetStart(double newX,double newY,double newXspeed,double newYspeed,int abs_time); 
    void SetEnd(int newEndType, int abs_time);
    
    void SetExist(int abs_time,bool state);
    bool GetExist(int abs_time);
    
    bool DropBox(double newX,double newY,double newXspeed,double newYspeed,int abs_time);
    
    void UpdateExist(int time);
    
    void ForwardTimeStep(int time);

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
    
    bool exist[5400];
    
    bool supported; // checked every frame before guys and other boxes land, no large array
    
    // start times, for spawning
    int startAbsTime;

    // end times, for paradox checking
    int endAbsTime;   
    int endType;
    // type of ending. For triggering or not triggering paradox detection.
    // 1 = taken through time while carried.


    // collision width and height. it is the same as BOX_WIDTH but it makes pretty code and portable from guy physics ;D.    
    static const int BOX_COLLISION_WIDTH = BOX_WIDTH;
    static const int BOX_COLLISION_HEIGHT = BOX_HEIGHT;
    
    static const double GRAVITY = 0.17;
};
