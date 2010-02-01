#include <allegro.h>

class Guy{

public:
    Guy();

    int GetX();
    int GetY();

    void DrawSprite(BITMAP* destinationBitmap, BITMAP* drawSprite); 

protected:
    
    int x;
    int y;
        
    int subimage;
        
};
