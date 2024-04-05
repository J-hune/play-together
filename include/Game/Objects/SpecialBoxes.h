//
// Created by athena on 24/03/24.
//

#ifndef PLAY_TOGETHER_SPECIALBOXES_H
#define PLAY_TOGETHER_SPECIALBOXES_H

#include "Player.h"

class SpecialBoxes {
    private :
        float x; // the horizontal  position
        float y; // the vertical position
        float hight;
        float width;
        void getSlower(Player* player);
        void getFaster(Player* player);
        void changeFigure(Player* player);
        void changeSize(Player* player);
    public:

        SpecialBoxes();

        SpecialBoxes(float X, float Y, float boxHeight , float boxWidth);

        float getHight();

        float getWidth();


        float const getX();

        float const getY();

        bool operator==(SpecialBoxes& box) const;


       void applySpecialBoxPower(Player* player);
        [[nodiscard]] SDL_FRect getBoundingBox() const;

};


#endif //PLAY_TOGETHER_SPECIALBOXES_H
