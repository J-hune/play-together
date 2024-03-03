//
// Created by athena on 28/02/24.
//


#include "../../include/Utils/Saves.h"


template <class T>
void Saves<T>::save(T obj, const std::string& filename) {

    std::ofstream ofs(filename);

    {
        boost::archive::text_oarchive oa(ofs);
        oa & obj;
    }
}

template<class T>
T Saves<T>::load(const std::string &filename) {
        T obj;
        std::ifstream ifs(filename);

        boost::archive::text_iarchive ia(ifs);
        ia >> obj;
        return obj;
}


