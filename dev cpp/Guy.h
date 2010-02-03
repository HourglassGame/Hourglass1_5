#include <allegro.h>

class Guy{

public:
    Guy();

    double GetX(int time);
    double GetY(int time);
    double GetXspeed(int time);
    double GetYspeed(int time);
    
    int GetOrder();
    void SetOrder(int newOrder);

    void DrawSprite(int time); 
    void Guy::unDrawSprite();
    
    void SetStart(double newX,double newY,double newXspeed,double newYspeed,int rel_time,int abs_time); 
    void ForwardTimeStep(int time);

protected:
    
    int prevDrawX;
    int prevDrawY;
    
    // position in absolute time
    double x[5400];
    double y[5400];
    double xSpeed[5400];
    double ySpeed[5400];
    
    // start times, for spawning
    int startAbsTime;
    int startRelTime;
    
    int timeOffset; // difference between start rel and abs time
    
    // end times, for paradox checking
    int endAbsTime;
    int endRelTime;
    
    // 1 = backward, 2 = pause, 3 = forward
    int timeDirection;
    
    bool playerCharacter; // ie the oldest instance
    
    int order; // order that the guy is in. ie 1 = oldest, 2 = second oldest...
        
    int subimage;
    
    // animation variables
    bool draw_moving;
    bool draw_facing; // left = false, right = true
        
};
