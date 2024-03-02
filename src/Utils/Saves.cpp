//
// Created by athena on 28/02/24.
//


#include "../../include/Utils/Saves.h"


template<typename T>
void Saves<T>::save(T obj, const std::string& filename) {

    std::ofstream ofs(filename);

    {
        boost::archive::text_oarchive oa(ofs);
        oa & obj;
    }
}

template<typename T>
void Saves<T>::load(T* obj, const std::string &filename) {

        std::ifstream ifs(filename);

        boost::archive::text_iarchive ia(ifs);
        ia >> obj;

}


