#include "SpriteA.h"
#include "../texture/PNG.h"
#include <sstream>

namespace PS3 {


    SpriteA::SpriteA()
    {
        currentFrame = -1;
        PAUSED = true;
    }

    SpriteA::SpriteA(string path, int s, int e)
    {
        frame.resize((e-s)+1);
        stringstream temp;
        for(int i = 0; i < ((e-s)+1); i++)
        {
            temp << path << (i+s) <<".png";
            frame.at(i).make(temp.str()); // potential problem area
            temp.str(""); // this too
        }

        pos[0] = 0;
        pos[1] = 0;

        currentFrame = 0;
        PAUSED = false;
    }

    SpriteA::~SpriteA(){}

    void SpriteA::setCoord()
{
    coord[0] = pos[0];
    coord[1] = (GL::getHeight() - pos[1]);//0
    coord[2] = 0; //TOP LEFT OF SQUARE

    coord[3] = pos[0];//0
    coord[4] = (GL::getHeight() - pos[1]) - frame.at(currentFrame).getHeight();
    coord[5] = 0; //BOTTOM LEFT OF SQUARE

    coord[6] = pos[0] + frame.at(currentFrame).getWidth();
    coord[7] = (GL::getHeight() - pos[1]) - frame.at(currentFrame).getHeight();
    coord[8] = 0; //BOTTOM RIGHT OF SQUARE

    coord[9] = pos[0] + frame.at(currentFrame).getWidth();
    coord[10] = (GL::getHeight() - pos[1]);//0
    coord[11] = 0; //TOP RIGHT OF SQUARE
}


    void SpriteA::start(void)
    {
        PAUSED = false;
    }
    void SpriteA::start(int f)
    {
        PAUSED = false;
        currentFrame = f;
    }

    void SpriteA::stop(void)
    {
        PAUSED = true;
    }
    void SpriteA::stop(int f)
    {
        PAUSED = true;
        currentFrame = f;
    }

    bool SpriteA::isPaused(void)
    {
        return PAUSED;
    }

    void SpriteA::increment(void)
    {
        currentFrame++;
        if(currentFrame >= 39)
        {
            currentFrame = 0;
        }
    }

    int SpriteA::getFrame(void)
    {
        return currentFrame;
    }

}
