//
// Created by anais on 26/03/24.
//


#ifndef PLAY_TOGETHER_TEXTINPUT_H
#define PLAY_TOGETHER_TEXTINPUT_H


#include <string>

class TextInput {

private:
    std::string message;
public:
    void setMessage(std::string s);
    std::string getMessage();


};


#endif //PLAY_TOGETHER_TEXTINPUT_H
