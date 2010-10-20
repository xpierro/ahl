/*
 * GL.h
 *
 *  Created on: 19 oct. 2010
 *      Author: Slynk
 */

#include <psgl/psgl.h>
#include <psgl/psglu.h>
#include "GL2D.h"

namespace PS3{
    GL2D::GL2D() { }
    GL2D::~GL2D() { }

    float GL2D::leftBound, GL2D::rightBound, GL2D::bottomBound, GL2D::topBound;

    void GL2D::preDraw(void) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2Df( leftBound, rightBound, bottomBound, topBound );
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    void GL2D::drawSprite(Sprite &sprite) {
        glBindTexture(GL_TEXTURE_2D, sprite.getImage()->getTextureId());

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, sprite.getCoord());
        glTexCoordPointer(2, GL_FLOAT, 0, allImage);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void GL2D::scrollX(float amount) {
        leftBound += amount;
        rightBound += amount;
    }
    void GL2D::scrollY(float amount) {
        bottomBound += amount;
        topBound += amount;
    }

    void GL2D::setBoundaries(void) {
        leftBound = 0;
        rightBound = width;
        bottomBound = 0;
        topBound = height;
    }
    void GL2D::setBoundaries(float l, float r, float b, float t)
    {
        leftBound = l;
        rightBound = r;
        bottomBound = b;
        topBound = t;
    }

    float GL2D::getLeftBound(void)
    {
        return leftBound;
    }
    float GL2D::getRightBound(void)
    {
        return rightBound;
    }
    float GL2D::getBottomBound(void)
    {
        return bottomBound;
    }
    float GL2D::getTopBound(void)
    {
        return topBound;
    }
}
