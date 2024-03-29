//
// Created by athena on 24/03/24.
//

#include "../../include/Game/SpecialBoxes.h"

SpecialBoxes ::SpecialBoxes()
        :x(0),y(0),hight(10),width(10) {}

SpecialBoxes ::SpecialBoxes(float X, float Y, float boxHeight, float boxWidth)
                :x(X),y(Y),hight(boxHeight),width(boxWidth) {}

const float SpecialBoxes::getX() { return x;}

const float SpecialBoxes::getY() { return y;}

float SpecialBoxes::getHight() { return hight;}

float SpecialBoxes::getWidth() { return  width;}
bool SpecialBoxes::operator==(SpecialBoxes& box) const{

     return this->x == box.getX()
            && this->y == box.getY()
            && this->width == box.getWidth()
            && this->hight == box.getHight();
}

