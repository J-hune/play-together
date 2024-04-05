//
// Created by athena on 24/03/24.
//

#include <iostream>
#include "../../../include/Game/Objects/SpecialBoxes.h"

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

void SpecialBoxes::applySpecialBoxPower(Player *player) {
    srand(time(nullptr));
    int state =  rand()%4;
    switch (state) {
        case 0:
            std::cout<<"case 0"<<std::endl;
            getSlower(player);
            break;
        case 1:
            std::cout<<"case 1"<<std::endl;
            getFaster(player);
            break;
        case 2:
            std::cout<<"case 2"<<std::endl;
            changeSize(player);
            break;
        case 3:
            std::cout<<"case 3"<<std::endl;
            changeFigure(player);
            break;
        default:
            std::cout<<"a problem occurred during the rand in SpecialBoxAction"<<std::endl;
    }

}

void SpecialBoxes::getSlower(Player* player){
    player->setSpeed( player->getSpeed()/2);
}
void SpecialBoxes::getFaster(Player* player){
    player->setSpeed( player->getSpeed()*2);
}

void SpecialBoxes::changeFigure(Player* player){
    //if the player ID is smaller than 0 it means that the player is alone and the default value (2) is set
    //if the playerID and SSprite are the same then the default textured(0) is places
    int id =  player->getPlayerID() == player->getSpriteID() ?
            0 : (player->getPlayerID() < 0 ? 2 : player->getPlayerID())%5;
    //in the case that an error has occurred and the player ID is bigger than 4 we reset at least the sprite within
    // the values that we have (reason of %5)

    //we set the SpriteID to the new one that we had set
    player->setSpriteID((short)id);
    player->setSpriteTextureByID(id);
}
void SpecialBoxes::changeSize(Player* player){
    float nSizeH = player->getH() > MIN_HIGHT ? player->getH()/2 : player->getH()*2;
    float nSizeW = player->getW() > MIN_WHIDTH ? player->getW()/2 : player->getW()*2;

    player->setH(nSizeH);
    player->setW(nSizeW);

}
SDL_FRect SpecialBoxes::getBoundingBox() const {
    return {x, y, width, hight};
}

