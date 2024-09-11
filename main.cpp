#include <fstream>
#include <iostream>

#include <unistd.h>

#include "function.h"


int main() {

    std::vector<std::map<std::string, unsigned int>> result = findWords();
    
    for (size_t i = result.size()-10; i < result.size(); i++)
    {
        std::map<std::string, unsigned int> tempResult = result[i];

        std::map<std::string, unsigned int>::iterator its;
        for (its = tempResult.begin(); its != tempResult.end(); its++) {
            std::cout << (its)->first << " : " << (its)->second << "\n";
        }
        std::cout << "\n\n";
    }
    
    std::cout << result.size() << std::endl;

}

std::map<std::string, unsigned int> importDictionary(std::string fileName) {

    std::ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
    }

    std::string line;
    std::vector<unsigned int> arr;
    std::vector<std::string> tempArr;
    std::map<std::string, unsigned int> myMap;

    while (getline(inputFile, line, '\n')) {
        if (line.size() == 5 )
        {
            if ((line[0] == '.' || line[1] == '.' || line[2] == '.' || line[3] == '.' || line[4] == '.') 
            || (line[0] == '-' || line[1] == '-' || line[2] == '-' || line[3] == '-' || line[4] == '-') 
            || (line[0] == ' ' || line[1] == ' ' || line[2] == ' ' || line[3] == ' ' || line[4] == ' ')
            || (line[0] == '\'' || line[1] == '\'' || line[2] == '\'' || line[3] == '\'' || line[4] == '\'')
            || (line[0] == '/' || line[1] == '/' || line[2] == '/' || line[3] == '/' || line[4] == '/')
            || (line[0] == ',' || line[1] == ',' || line[2] == ',' || line[3] == ',' || line[4] == ',')
            || (line[0] == '&' || line[1] == '&' || line[2] == '&' || line[3] == '&' || line[4] == '&')
            || (std::isdigit(line[0]) || std::isdigit(line[1]) || std::isdigit(line[2]) || std::isdigit(line[3]) || std::isdigit(line[4]))
            || checkMultiLetters(line)) {
                continue;
            }
            tempArr.push_back(line);
            arr.push_back(convertToBitwise(line));
            if (convertToBitwise(line) < 1073741824)
            {
                myMap.insert({line, convertToBitwise(line)});
            } else {
                std::cout << "didn't include word" << std::endl;
            }

        }
    }
    return myMap;   
}

std::vector<std::map<std::string, unsigned int>> findWords() {

    std::map<std::string, unsigned int> dictVec = importDictionary("dict.txt");

    std::map<std::string, unsigned int> tempList;
    std::vector<std::map<std::string, unsigned int>> wordLists;
    unsigned int bitwised = 0;
    unsigned int previous;

    std::map<std::string, unsigned int>::iterator currentElement;
    std::map<std::string, unsigned int>::iterator previousIt;
    std::map<std::string, unsigned int>::iterator it;
    std::map<std::string, unsigned int>::iterator itr;
    bool doesAppear = false;

    for (it = dictVec.begin(); std::next(it) != dictVec.end(); it++) {
        tempList.insert(*it);
        //std::cout << "For loop 1 " << std::endl;

        for (itr = std::next(it), previousIt = it, previous = (it)->second; itr != dictVec.end() && tempList.size() < 5; itr++) {
            //bitwised = previous & (itr)->second;
            //std::cout << "For loop 2 " << std::endl;

            for (currentElement = tempList.begin(); currentElement != tempList.end(); currentElement++)
            {
                //std::cout << "For loop 3 " << std::endl;
                if (((currentElement)->second & (itr)->second) == 0)
                {
                    
                    doesAppear = false;
                    //std::cout << (currentElement)->first << " & " << (itr)->first << " have " << ((currentElement)->second & (itr)->second) << ", zero overlap" << " list size = " << tempList.size() << "\n";
                    previous = (itr)->second;
                    previousIt = itr;

                    
                } else {
                    //std::cout << (currentElement)->first << " & " << (itr)->first << " have " << ((currentElement)->second & (itr)->second) << ", some overlap\n";
                    //bitwised = 0;
                    doesAppear = true;
                    break;
                }
            }

            if (!doesAppear)
            {
                std::cout << "written to tempList" << "\n";
                tempList.insert(*itr);
                doesAppear = true;
                std::cout << "\n\n\ninserted Element " << (itr)->first << " list size = " << tempList.size() << "\n\n\n";
                if (tempList.size() == 4)
                {
                    std::cout << "Nearly there" << std::endl;
                } else if (tempList.size() == 5)
                {
                    std::cout << "Found" << std::endl;
                }
                
            }
            
            doesAppear = false;
            
        }

        if (tempList.size() == 5)
        {
            wordLists.push_back(tempList);
            std::cout << "completed list of 5\n";

            std::map<std::string, unsigned int>::iterator itf;
            for (itf = tempList.begin(); itf != tempList.end(); itf++)
            {
                std::cout << "\n\n*************************" << (itf)->first << "**************************\n\n";
            }
        }
        tempList.clear();
    }
    return wordLists;
}

int convertToBitwise(std::string word) {
    
    unsigned int wordAsInt = 0;
    for (size_t i = 0; i < 5; i++)
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
            std::cerr << "Couldn't match a letter: " << word << std::endl;
            wordAsInt |= 0b01000000000000000000000000000000;
            break;
        }
    }
    
    return wordAsInt;
}

bool checkMultiLetters(std::string word) {
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = i+1; j < 5; j++)
        {
            if (std::tolower(word[i])  == std::tolower(word[j]))
            {
                return true;
            }
            
        }
        
    }

    return false;
}