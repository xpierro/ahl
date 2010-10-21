#ifndef SPRITEA_H_
#define SPRITEA_H_
#include "Sprite.h"
#include "../texture/PNG.h"
#include <vector>

namespace PS3{

class SpriteA : public Sprite
{
private:
    int currentFrame; /* The current frame of the animation */
    bool PAUSED; /* holds the status of whether the animation is paused or not */

public:
    vector <PNG> frame; /* Holds all the frames of the sprite */


    SpriteA();

    /*
    ** The constructor takes 3 arguments that combine for a pattern to load the images:
    ** path -- holds the first part of the path ex: USRDIR/images/sprite (without the number and .png extension)
    ** s -- First number of the images you're loading ex: sprite1.png, so 1
    ** e -- Last number of the images you're loading ex: sprite39.png, so 39
    ** This will load the images through the range provided: sprite1.png, sprite2.png etc etc.
    */
    SpriteA(string path, int s, int e);

    ~SpriteA();

    /*
    ** Overloades the setCoord() from the sprite class to use vector frame for calculations
    */
    void setCoord();

    void start(void); /* sets PAUSED to false */
    void start(int f); /*sets PAUSED to false and starts the animation on a certain frame*/

    void stop(void); /* sets PAUSED to true */
    void stop(int f); /* sets PAUSED to true and sets the frame */

    bool isPaused(void); /* returns PAUSED */

    void increment(void); /* Increments the animation and starts it over if the end is reached */
    int getFrame(void); /* returns the current fram */

};
}
#endif
