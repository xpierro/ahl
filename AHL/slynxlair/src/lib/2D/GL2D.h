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

namespace PS3{

class GL2D : public GL
{
private:
    static float leftBound, rightBound, bottomBound, topBound;
public:
    GL2D();
    ~GL2D();

    static void preDraw(void);
    static void drawSprite(Sprite &sprite);

    static void scrollX(float amount);
    static void scrollY(float amount);

    static void setBoundaries(void);
    static void setBoundaries(float l, float r, float b, float t);

    static float getLeftBound(void);
    static float getRightBound(void);
    static float getBottomBound(void);
    static float getTopBound(void);
};
}
#endif /* GL_H_ */
