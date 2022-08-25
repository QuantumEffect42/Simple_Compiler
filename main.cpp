#include <iostream>
#include <fstream>
#include <list>
#include "token.h"

/* REGEX FOR THE SIMPLE LANGUAGE
 *
 * assign --> =
 * plus --> +
 * minus --> -
 * times --> *
 * div --> /
 * left_parentheses --> (
 * right_parentheses --> )
 * id --> letter (letter | digit)*
 * number --> digit digit* | digit* (. digit | digit .) digit*
 * ML_comment --> /* (non-* | * non-/)* *+/
 * SL_comment --> // (non-newline)* newline
 * delimiter --> ;
 */

//IMPORTANT TO NOTE: Does not need to check if it makes sense, only needs to group into tokens

std::string keywordLookup(std::string id){
    bool match = true;
    std::string print = "print"; //strings to store the keywords

    if(id.length() != print.length()){ //handles the print keyword lookup
        for(int i = 0; i < id.length(); i++){
            if(id[i] != print[i]){ //checks if id matches with "print" keyword and breaks if discrepancy detected
                match = false;
                break;
            }
        }
        if(match){
            id = print;
        }
    }
    else{ //returns "id" if no keyword found
        id = "id";
    }
    return id;
}

std::string concatFile(std::fstream &programFile){
    std::string fileAsString;
    std::string temp;

    if(programFile.is_open()) {
        while (getline(programFile, temp)) {
            fileAsString.append(temp);
            fileAsString.push_back('\n');
        }
        std::cout << fileAsString << std::endl;
    }
    return fileAsString;
}

std::list<std::string> scan(std::string fileAsString){ //takes a fstream reference to begin scanning
    std::list<std::string> tokens; //list to store the tokens of the program

    int testInt = 0;
    //gets the lines from the program
    unsigned long fileSize = fileAsString.size();
    int i = 0;
    while(i < fileSize){ //main loop to
        //read and tokenizes single character tokens except division due to it being the start of comments as well
        if(fileAsString[i] == '=' || fileAsString[i] == '+' || fileAsString[i] == '-' || fileAsString[i] == '*' || fileAsString[i] == '(' || fileAsString[i] == ')' || fileAsString[i] == ';'){
            std::string token(1, fileAsString[i]); //creates a strings from the single char tokens

            tokens.push_back(token); //adds token to the list
            i++;
        }
        else if(fileAsString[i] == '/'){ //matches with comment or division
            //connects with comment
            if(fileAsString[i + 1] == '*' || fileAsString[i + 1] == '/'){
                i++;
                if(fileAsString[i] == '/') { //deals with single line comments
                    while (fileAsString[i] != '\n') {
                        i++;
                    }
                }
                else{ //deals with multiline comments
                    while(fileAsString[i] != '*' || fileAsString[i + 1] != '/'){
                        i++;
                    }
                    i += 2; //increment i by 2 to remove the '*' and '/' from the input stream
                }

                std::string token("comment");
                tokens.push_back(token);
            }
            else{ //stores the division symbol
                std::string token(1, fileAsString[i + 1]);
                i++;
            }
        }
        else if(fileAsString[i] >= 48 && fileAsString[i] <= 57){ //matches with a number
            char test = fileAsString[i];
            //detects if the previous character in the stream was a letter to determine if digit or identifier
            bool previousIsLetter = (fileAsString[i - 1] >= 65 && fileAsString[i - 1] <= 90) || (fileAsString[i - 1] >= 97 && fileAsString[i - 1] <= 122);

            if(!previousIsLetter){
                std::string value;
                while(fileAsString[i] != ' ' && fileAsString[i] != ';'){ //reads all digits in the number until there is a break
                    value.push_back(fileAsString[i]);
                    i++;
                }
                std::string token("number");
                tokens.push_back(token.append(value)); //appends the value of the number to the end of the end of the token for storage
            }
        }
        else if((fileAsString[i] >= 65 && fileAsString[i] <= 90) || (fileAsString[i] >= 97 && fileAsString[i] <= 122)){ //matches with an id
            std::string identifierName;
            while(fileAsString[i] != ' ' && fileAsString[i] != ';'){
                identifierName.push_back(fileAsString[i]);
                i++;
            }
            std::string token = keywordLookup(identifierName);
            if(token[0] == 'i') { //if a non keyword is returned, append the id name to the token for safe keeping
                tokens.push_back(token.append(identifierName));
            }
            else{ //otherwise, just store the keyword
                tokens.push_back(identifierName);
            }
        }
        else{
            i++;
        }
    }
    return tokens;
}

std::list<token>& objectifyTokens(std::list<std::string> rawTokens){
    std::list<token> *objectifiedTokens;
    std::list<std::string>::iterator iterator;

    for(iterator = rawTokens.begin(); iterator != rawTokens.end(); ++iterator){
        token temp(*iterator);
        objectifiedTokens->push_back(temp);
    }

    std::cout << "At end of objectifyTokens()" << std::endl;
    return *objectifiedTokens;
}

void printTokens(std::list<std::string> tokens){
    std::list<std::string>::iterator printer;
    for(printer = tokens.begin(); printer != tokens.end(); ++printer){
        std::cout << "'" << *printer << "' ";
    }
    std::cout << std::endl;
}


int main(int argc, char** argv) {

    std::list<std::string> tokens;
    std::list<token> objectifiedTokens;

    if(argc == 2) {
        std::string filename(argv[1]);

        std::fstream programFile(filename);

        if(!programFile){ //throws an error should the file not open correctly
            throw std::system_error(errno, std::system_category(), "Failed to open program file: "+filename);
        }
        else{
            std::string fileAsString = concatFile(programFile);
            tokens = scan(fileAsString);
            objectifiedTokens = objectifyTokens(tokens);
            printTokens(tokens);
            programFile.close();
        }
    }
    else{
        std::cerr << "Error: incorrect number of arguments. Provided: " << argc << " required: 1" << std::endl;
    }

    return 0;
}
