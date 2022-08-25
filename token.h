//
// Created by nolan on 2022-06-15.
//

#ifndef SCANNER_TOKEN_H
#define SCANNER_TOKEN_H

#include <iostream>

class token {
public:
    explicit token(std::string);

private:
    void split(std::string, int, bool); //splits the string at the passed index, stores second half in identifier if identifier is passed, stores in value otherwise

public:
    std::string text; //stores the type of token being stored
    std::string identifier; //used to store name of an identifier, null if token is not an identifier
    std::string value; //used to store the value of a number, null if token is not a number
};


#endif //SCANNER_TOKEN_H
