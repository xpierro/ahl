#include "../texture/PNG.h"

namespace PS3 {

class Sprite{
private:

    int pos[2];
    float coord[12];

public:
    PNG image;
    Sprite();
    Sprite(PNG &png);


    void setX(int x);
    void setY(int x);
    int getX();
    int getY();

    void convertToFloat();
    float* getCoord();

};
}
