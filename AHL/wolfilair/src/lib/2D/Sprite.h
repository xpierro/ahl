#ifndef SPRITE_H_
#define SPRITE_H_
#include "../texture/PNG.h"

const float allImage[8] = {0., 1.,//Top Left
                          0., 0.,//Bottom Left
                          1., 0.,//Bottom Right
                          1., 1.};//Top Right

namespace PS3 {

class Sprite{
private:
    int pos[2];
    GLfloat coord[12];
    PNG* image;

public:
    Sprite();
    Sprite(string path);

    PNG* getImage() const;
    void setX(int x);
    void setY(int x);

    int getX() const;
    int getY() const;

    void setCoord();
    float* getCoord(); // Cannot be const, is the name well chosen ?

};
}
#endif
