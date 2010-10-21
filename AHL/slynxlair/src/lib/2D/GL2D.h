/*
 * GL.h
 *
 *  Created on: 19 oct. 2010
 *      Author: Slynk
 */


#ifndef GL2D_H_
#define GL2D_H_

#include <psgl/psgl.h>
#include <psgl/psglu.h>
#include "../GL.h"
#include "Sprite.h"
#include "SpriteA.h"

namespace PS3{

/*
*  GL2D holds functions to simplify 2D development in openGL
*/

class GL2D : public GL
{
private:
    /*Variables housing the boundaries of the screen*/
    static float leftBound, rightBound, bottomBound, topBound;
public:
    GL2D();
    ~GL2D();

    static void preDraw(void); /*Call before you draw*/
    static void drawSprite(Sprite &sprite); /*Draws a sprite to the screen*/
    static void drawSprite(SpriteA &sprite); /*Draws an animated sprite to the screen*/

    static void scrollX(float amount); /* Moves the screen by amount (-amount for left / +amount for right*/
    static void scrollY(float amount); /* Moves the screen by amount (-amount for down / +amount for up*/

    static void setBoundaries(void); /* Defaults to 0->Screen Dimensions */
    static void setBoundaries(float l, float r, float b, float t); /* Sets the screen boundaries*/

    static float getLeftBound(void);
    static float getRightBound(void);
    static float getBottomBound(void);
    static float getTopBound(void);
};
}
#endif /* GL_H_ */
