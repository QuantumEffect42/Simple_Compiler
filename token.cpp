//
// Created by nolan on 2022-06-15.
//

#include "token.h"

token::token(std::string passedText) {
    std::cout << "At beginning of token constructor" << std::endl;
    if(passedText[0] == 'i'){ //checks for identifiers to split at correct index
        split(passedText, 2, true);
    }
    else if(passedText[0] == 'n'){ //checks for numbers to split at correct index
        split(passedText, 6, false);
    }
    else{
        text = passedText;
    }
    std::cout << "At end of token constructor" << std::endl;
}

void token::split(std::string passedText, int index, bool name) { //custom splitter to parse input properly

    for(int i = 0; i < index; i++){
        text.push_back(passedText[i]); //pushes the type of token into text
    }

    if(name){
        for(int i = index; i < passedText.length(); i++){
            this->identifier.push_back(passedText[i]);
        }
    }
    else{
        for(int i = index; i < passedText.length(); i++){
            this->identifier.push_back(passedText[i]);
        }
    }
}
