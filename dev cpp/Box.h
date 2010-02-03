#include <allegro.h>

class Box{

public:
    Box();

    double GetX(int time);
    double GetY(int time);
    double GetXspeed(int time);
    double GetYspeed(int time);
    
    void SetId(int newId);

    void DrawSprite(int time); 
    void unDrawSprite();
    
    int GetStartAbsTime();
    int GetEndAbsTime();
    
    void SetStart(double newX,double newY,double newXspeed,double newYspeed,int abs_time); 
    void ForwardTimeStep(int time);

protected:
    
    int id; // array number in box array
    
    // where drawing occured last time
    int prevDrawX;
    int prevDrawY;

    // position in absolute time
    double x[5400];
    double y[5400];
    double xSpeed[5400];
    double ySpeed[5400];
    
    // start times, for spawning
    int startAbsTime;

    // end times, for paradox checking
    int endAbsTime;   
    int endType;
    // type of ending. For triggering or not triggering paradox detection.
    // 1 = picked up by guy.
};
