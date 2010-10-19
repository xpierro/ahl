#include "../texture/PNG.h"

namespace PS3 {

class Sprite{
private:

    int pos[2];
    GLfloat coord[12];

public:
    PNG image;
    Sprite();
    Sprite(string path);


    void setX(int x);
    void setY(int x);
    int getX();
    int getY();

    void setCoord();
    float* getCoord();

};
}
