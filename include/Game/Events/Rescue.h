//
// Created by athena on 18/04/24.
//

#ifndef PLAY_TOGETHER_RESCUE_H
#define PLAY_TOGETHER_RESCUE_H


#include "../Player.h"

class Rescue {
private :
        float x,y,h,w;
        Point currentPoint;//first possible positions for player

public :
    explicit Rescue(float x,float y,float h,float w,Point first);
    Rescue() = default;

    float getX() const;

    float getY() const;

    float getH() const;

    float getW() const;

    Point getNextposition();

    SDL_FRect getBoundingBox() const;

};


#endif //PLAY_TOGETHER_RESCUE_H
