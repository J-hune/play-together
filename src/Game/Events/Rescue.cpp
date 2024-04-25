//
// Created by athena on 18/04/24.
//

#include "../../../include/Game/Events/Rescue.h"

Rescue::Rescue(float x,float y,float h,float w,Point first):x(x),y(y),h(h),w(w),currentPoint(first) {}

float Rescue::getX() const { return x;}
float Rescue::getY() const { return y;}
float Rescue::getW() const { return w;}
float Rescue::getH() const { return h;}

Point Rescue::getNextposition(){
    Point givePoint = currentPoint;
    currentPoint = Point(currentPoint.x+50,currentPoint.y);
    return givePoint;
}


SDL_FRect Rescue::getBoundingBox() const {
    return {x, y, w, h};
}