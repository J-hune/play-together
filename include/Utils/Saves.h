//
// Created by athena on 28/02/24.
//

#ifndef PLAY_TOGETHER_SAVES_H
#define PLAY_TOGETHER_SAVES_H

#include <string>
#include <fstream>
#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;


template <class T>
class Saves {
public :
    /**
     *@brief it save the given object in a file named
     * like the given parameter filename
     * @param obj
     * @param filename
     */
    void static save(T obj,const std::string& filename);

    /**
     * @brief form the given file we extract/load the object
     * @param filename
     */
    T static load(const std::string& filename);
};


#endif //PLAY_TOGETHER_SAVES_H
