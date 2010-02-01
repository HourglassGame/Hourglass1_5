#include "Guy.h"

Guy::Guy( ){

    x = 50;
    y = 200;
    

}

int Guy::GetX(){

    return x;

}

int Guy::GetY(){

    return y;

}

void Guy::DrawSprite(BITMAP* destinationBitmap, BITMAP* drawSprite){
    
    rectfill( destinationBitmap ,x , y, x+33, y+32, makecol ( 0, 0, 0));
    masked_blit(drawSprite,destinationBitmap,36*subimage,0,x,y,33,32);
    
    subimage++;
    if (subimage > 14)
    {
        subimage = 0;
    }
}

