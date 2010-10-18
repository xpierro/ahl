#include "Sprite.h"
#include "../GL.h"
#include "../texture/PNG.h"

namespace PS3 {
Sprite::Sprite(){}

Sprite::Sprite(string path)
{
    image.make(path);
    pos[0] = 0;
    pos[1] = 0;
}

void Sprite::setX(int x)
{
    pos[0] = x;
}
void Sprite::setY(int x)
{
    pos[1] = x;
}
int Sprite::getX()
{
    return pos[0];
}
int Sprite::getY()
{
    return pos[1];
}

void Sprite::convertToFloat()
{
    coord[0] = (pos[0]/(GL::getWidth()/2)-1);
    coord[1] = (pos[1]/(GL::getHeight()/2)-1);
    coord[2] = 0; //TOP LEFT OF SQUARE

    coord[3] = ((pos[0]/(GL::getWidth()/2)-1) + (image.getWidth()/(GL::getWidth()/2)-1));
    coord[4] = (pos[1]/(GL::getHeight()/2)-1);
    coord[5] = 0; //TOP RIGHT OF SQUARE

    coord[6] = (pos[1]/(GL::getWidth()/2)-1);
    coord[7] = ((pos[1]/(GL::getHeight()/2)-1) + (image.getHeight()/(GL::getHeight()/2)-1));
    coord[8] = 0; //BOTTOM LEFT OF SQUARE

    coord[9] = ((pos[1]/(GL::getWidth()/2)-1) + (image.getWidth()/(GL::getWidth()/2)-1));
    coord[10]= ((pos[1]/(GL::getHeight()/2)-1) + (image.getHeight()/(GL::getHeight()/2)-1));
    coord[11]= 0; //BOTTOM RIGHT OF SQUARE
}

float* Sprite::getCoord()
{
    convertToFloat();
    return coord;
}

}
