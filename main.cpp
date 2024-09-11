#include <fstream>
#include <iostream>

#include "function.h"


int main() {

    std::vector<std::string> dict = importDictionary("dict.txt");
    std::cout << convertToBitwise(dict[0]) << std::endl;


}

std::vector<std::string> importDictionary(std::string fileName) {

    std::ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
    }

    std::string line;
    std::vector<std::string> arr;

    while (getline(inputFile, line, '\n')) {
        if (line.size() == 5 )
        {
            if ((line[0] == '.' || line[1] == '.' || line[2] == '.' || line[3] == '.' || line[4] == '.') 
            || (line[0] == '-' || line[1] == '-' || line[2] == '-' || line[3] == '-' || line[4] == '-') 
            || (line[0] == ' ' || line[1] == ' ' || line[2] == ' ' || line[3] == ' ' || line[4] == ' ')
            || (line[0] == '\'' || line[1] == '\'' || line[2] == '\'' || line[3] == '\'' || line[4] == '\'')
            || (line[0] == ',' || line[1] == ',' || line[2] == ',' || line[3] == ',' || line[4] == ',')
            || (line[0] == '&' || line[1] == '&' || line[2] == '&' || line[3] == '&' || line[4] == '&')
            || (std::isdigit(line[0]) || std::isdigit(line[1]) || std::isdigit(line[2]) || std::isdigit(line[3]) || std::isdigit(line[4]))) {
                continue;
            }
            
            arr.push_back(line);
        }
    }

    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] <<", ";
    }
    std::cout << std::endl;
    
    return arr;
    
}

std::vector<std::string> findWords() {

    std::vector<std::string> dictVec = importDictionary("dict.txt");
    


}

int convertToBitwise(std::string word) {
    unsigned int wordAsInt = 0;
    for (size_t i = 0; i != 4; i++)
    {
        switch (word[i])
        {
        case 'a':
        case 'A':
            wordAsInt |= 0b00000000000000000000000000000001;
            break;
        
        case 'b':
        case 'B':
            wordAsInt |= 0b00000000000000000000000000000010;
            break;

        case 'c':
        case 'C':
            wordAsInt |= 0b00000000000000000000000000000100;
            break;

        case 'd':
        case 'D':
            wordAsInt |= 0b00000000000000000000000000001000;
            break;

        case 'e':
        case 'E':
            wordAsInt |= 0b00000000000000000000000000010000;
            break;

        case 'f':
        case 'F':
            wordAsInt |= 0b00000000000000000000000000100000;
            break;

        case 'g':
        case 'G':
            wordAsInt |= 0b00000000000000000000000001000000;
            break;

        case 'h':
        case 'H':
            wordAsInt |= 0b00000000000000000000000010000000;
            break;

        case 'i':
        case 'I':
            wordAsInt |= 0b00000000000000000000000100000000;
            break;

        case 'j':
        case 'J':
            wordAsInt |= 0b00000000000000000000001000000000;
            break;

        case 'k':
        case 'K':
            wordAsInt |= 0b00000000000000000000010000000000;
            break;

        case 'l':
        case 'L':
            wordAsInt |= 0b00000000000000000000100000000000;
            break;

        case 'm':
        case 'M':
            wordAsInt |= 0b00000000000000000001000000000000;
            break;

        case 'n':
        case 'N':
            wordAsInt |= 0b00000000000000000010000000000000;
            break;

        case 'o':
        case 'O':
            wordAsInt |= 0b00000000000000000100000000000000;
            break;

        case 'p':
        case 'P':
            wordAsInt |= 0b00000000000000001000000000000000;
            break;

        case 'q':
        case 'Q':
            wordAsInt |= 0b00000000000000010000000000000000;
            break;

        case 'r':
        case 'R':
            wordAsInt |= 0b00000000000000100000000000000000;
            break;

        case 's':
        case 'S':
            wordAsInt |= 0b00000000000001000000000000000000;
            break;

        case 't':
        case 'T':
            wordAsInt |= 0b00000000000010000000000000000000;
            break;

        case 'u':
        case 'U':
            wordAsInt |= 0b00000000000100000000000000000000;
            break;

        case 'v':
        case 'V':
            wordAsInt |= 0b00000000001000000000000000000000;
            break;

        case 'w':
        case 'W':
            wordAsInt |= 0b00000000010000000000000000000000;
            break;

        case 'x':
        case 'X':
            wordAsInt |= 0b00000000100000000000000000000000;
            break;

        case 'y':
        case 'Y':
            wordAsInt |= 0b00000001000000000000000000000000;
            break;

        case 'z':
        case 'Z':
            wordAsInt |= 0b00000010000000000000000000000000;
            break;

        default:
            std::cerr << "Couldn't match a letter" << std::endl;
            break;
        }
    }
    
    return wordAsInt;
}