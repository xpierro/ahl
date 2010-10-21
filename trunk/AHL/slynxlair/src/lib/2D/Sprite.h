#ifndef SPRITE_H_
#define SPRITE_H_
#include "../texture/PNG.h"

namespace PS3 {

/*
*  Sprite houses a 2D storage for images (Currently only PNG)
*/

class Sprite{
protected:

    int pos[2]; /* (x,y) coordinate system. Uses 0,0 for top left of the image and sprite instead of OpenGL's bottom left */
    GLfloat coord[12]; /* the array to hold the coordinates of the square to attach the image to */

public:
    PNG image; /* The image associated with the Sprite */
    Sprite();
    Sprite(string path);


    void setX(int x); /* set pos[0] */
    void setY(int x); /* set pos[1] */
    int getX(); /* return pos[0]*/
    int getY(); /* return pos[1] */

    virtual void setCoord(); /* converts pos into screen coordinates */
    float* getCoord(); /* returns coord*/

};
}
#endif
