//
// Created by athena on 28/02/24.
//

#ifndef PLAY_TOGETHER_SAVES_H
#define PLAY_TOGETHER_SAVES_H

#include <string>
#include <fstream>
#include <iostream>
#include "../Game/Game.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;
template <typename T>


class Saves {
public :
    void save(T obj,const std::string& filename);//later make it static
    void load(T* obj,const std::string& filename);
};


#endif //PLAY_TOGETHER_SAVES_H
