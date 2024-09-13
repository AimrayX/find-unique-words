#include <fstream>
#include <iostream>
#include <time.h>
#include <algorithm>

#include <unistd.h>

#include "function.h"

int main()
{
    clock_t tStart = clock();
    std::vector<std::vector<std::pair<std::string, unsigned int>>> result = findWords();

    std::ofstream MyFile("result.txt");

    for (size_t i = 0; i < result.size(); i++)
    {
        std::vector<std::pair<std::string, unsigned int>> tempResult = result[i];

        std::vector<std::pair<std::string, unsigned int>>::iterator its;
        for (its = tempResult.begin(); its != tempResult.end(); its++)
        {
            MyFile << (its)->first << "\n";
        }
        MyFile << "\n";
    }

    MyFile.close();
    std::cout << result.size() << std::endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    return 0;
}

std::vector<std::pair<std::string, unsigned int>> importDictionary(std::string fileName)
{

    std::ifstream inputFile(fileName);

    if (!inputFile.is_open())
    {
        std::cerr << "Error opening the file!" << std::endl;
    }

    std::string line;
    std::vector<std::pair<std::string, unsigned int>> myMap;

    while (getline(inputFile, line, '\n'))
    {
        if (line.size() == 5)
        {
            if ((line[0] == '.' || line[1] == '.' || line[2] == '.' || line[3] == '.' || line[4] == '.') || (line[0] == '-' || line[1] == '-' || line[2] == '-' || line[3] == '-' || line[4] == '-') || (line[0] == ' ' || line[1] == ' ' || line[2] == ' ' || line[3] == ' ' || line[4] == ' ') || (line[0] == '\'' || line[1] == '\'' || line[2] == '\'' || line[3] == '\'' || line[4] == '\'') || (line[0] == '/' || line[1] == '/' || line[2] == '/' || line[3] == '/' || line[4] == '/') || (line[0] == ',' || line[1] == ',' || line[2] == ',' || line[3] == ',' || line[4] == ',') || (line[0] == '&' || line[1] == '&' || line[2] == '&' || line[3] == '&' || line[4] == '&') || (std::isdigit(line[0]) || std::isdigit(line[1]) || std::isdigit(line[2]) || std::isdigit(line[3]) || std::isdigit(line[4])) || checkMultiLetters(line))
            {
                continue;
            }
            if (convertToBitwise(line) < 1073741824)
            {                            
                myMap.push_back({line, convertToBitwise(line)});
            }
            else
            {
                std::cout << "didn't include word" << std::endl;
            }
        }
    }

    std::vector<std::pair<std::string, unsigned int>>::iterator myMapIt1;
    std::vector<std::pair<std::string, unsigned int>>::iterator myMapIt2;
    std::vector<std::pair<std::string, unsigned int>>::iterator myMapTempErase;
    std::pair<std::string, unsigned int> myMapTempSwitchValue;

    std::cout << "starting filtering for anagrams: size = " << myMap.size() << std::endl;
    for (myMapIt1 = myMap.begin(); myMapIt1 != std::prev(myMap.end()); myMapIt1++)
    {
        for (myMapIt2 = std::next(myMapIt1); myMapIt2 != myMap.end(); myMapIt2++)
        {
            if ((myMapIt2)->first != "" && is_anagram((myMapIt1)->first, (myMapIt2)->first))
            {
                myMapTempErase = myMapIt2;         
                myMapIt2 = myMap.erase(myMapTempErase);
            }
        }       
    }

    std::cout << "finished filtering for anagrams: size = " << myMap.size() << std::endl;
    std::sort(myMap.begin(), myMap.end(), 
        [](const std::pair<std::string, unsigned int>& a, const std::pair<std::string, unsigned int>& b) {
            return a.second > b.second;
        });

    /*
    std::ofstream MyFile("sortedDict.txt");
    std::vector<std::pair<std::string, unsigned int>>::iterator its;
    for (its = myMap.begin(); its != myMap.end(); its++)
    {
        MyFile << (its)->first << "\n";
    }
    MyFile.close();
    */

    return myMap;
}

std::vector<std::vector<std::pair<std::string, unsigned int>>> findWords()
{
    clock_t tStart = clock();
    std::vector<std::pair<std::string, unsigned int>> dictVec = importDictionary("dict.txt");
    int dictVecSize = dictVec.size();
    std::cout << "***********\n\n" << dictVecSize << "\n\n***********" << std::endl;
    

    std::vector<std::pair<std::string, unsigned int>> tempList;
    std::vector<std::vector<std::pair<std::string, unsigned int>>> wordLists;

    std::vector<std::pair<std::string, unsigned int>>::iterator currentElement;
    std::vector<std::pair<std::string, unsigned int>>::iterator it;
    std::vector<std::pair<std::string, unsigned int>>::iterator itr;
    std::vector<std::pair<std::string, unsigned int>>::iterator secondToLastDictVecIt = std::prev(dictVec.end());

    std::vector<std::pair<std::string, unsigned int>>::iterator node2;
    std::vector<std::pair<std::string, unsigned int>>::iterator node3;
    std::vector<std::pair<std::string, unsigned int>>::iterator node4;

    bool doesAppear = false;
    int i = 0;
    int tempListSize = 0;

    for (it = dictVec.begin(); it != secondToLastDictVecIt; it++)
    {
        i++;
        tempList.push_back(*it);
        tempListSize += 1;
        if (i % 10 == 0)
        {
            std::cout << i << " / " << dictVecSize << std::endl;
            printf("%.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
        }
        
        for (itr = std::next(it); itr != dictVec.end() && tempListSize < 5; itr++)
        {       
            
            for (currentElement = tempList.begin(); currentElement != tempList.end(); currentElement++)
            {
                if (((currentElement)->second & (itr)->second) == 0)
                {
                    doesAppear = false;
                }
                else
                {
                    doesAppear = true;
                    break;
                }
            }

            if (!doesAppear)
            {
                tempList.push_back(*itr);
                tempListSize += 1;
                doesAppear = true;

                if (tempListSize == 2)
                {
                    node2 = itr;
                }
                else if (tempListSize == 3)
                {
                    node3 = itr;
                }
                else if (tempListSize == 4)
                {
                    node4 = itr;
                }
                else if (tempListSize == 5)
                {
                    std::cout << "Found\n" << std::endl;
                }
            }

            doesAppear = false;

            if (tempListSize == 5)
            {
                wordLists.push_back(tempList);
                std::cout << "completed list of 5\n";

                std::vector<std::pair<std::string, unsigned int>>::iterator itf;
                for (itf = tempList.begin(); itf != tempList.end(); itf++)
                {
                    std::cout << "\n\n*************************" << (itf)->first << "**************************\n\n";
                }

            }
            else if (tempListSize == 2 && itr == secondToLastDictVecIt)
            {
                if (node2 == secondToLastDictVecIt)
                {
                    break;
                }

                tempList.erase(std::prev(tempList.end()));
                tempListSize -= 1;
                itr = std::next(node2);
            }
            else if (tempListSize == 3 && itr== secondToLastDictVecIt)
            {
                if (node3 == secondToLastDictVecIt)
                {
                    break;
                }
                tempList.erase(std::prev(tempList.end()));
                tempListSize -= 1;
                itr = std::next(node3);
            }
            else if (tempListSize == 4 && itr == secondToLastDictVecIt)
            {
                if (node4 == secondToLastDictVecIt)
                {
                    break;
                }
                tempList.erase(std::prev(tempList.end()));
                tempListSize -= 1;
                itr = std::next(node4);
            }
        }
        tempList.clear();
        tempListSize = 0;
    }
    return wordLists;
}

int convertToBitwise(std::string word)
{

    unsigned int wordAsInt = 0;
    for (size_t i = 0; i < 5; i++)
    {
        switch (word[i])
        {
        case 'a':
        case 'A':
            wordAsInt |= 0b00000000000000000000000000000001;
            break;

        case 'e':
        case 'E':
            wordAsInt |= 0b00000000000000000000000000000010;
            break;

        case 's':
        case 'S':
            wordAsInt |= 0b00000000000000000000000000000100;
            break;

        case 'i':
        case 'I':
            wordAsInt |= 0b00000000000000000000000000001000;
            break;

        case 'o':
        case 'O':
            wordAsInt |= 0b00000000000000000000000000010000;
            break;

        case 'r':
        case 'R':
            wordAsInt |= 0b00000000000000000000000000100000;
            break;

        case 'u':
        case 'U':
            wordAsInt |= 0b00000000000000000000000001000000;
            break;

        case 'n':
        case 'N':
            wordAsInt |= 0b00000000000000000000000010000000;
            break;

        case 'l':
        case 'L':
            wordAsInt |= 0b00000000000000000000000100000000;
            break;

        case 't':
        case 'T':
            wordAsInt |= 0b00000000000000000000001000000000;
            break;

        case 'y':
        case 'Y':
            wordAsInt |= 0b00000000000000000000010000000000;
            break;

        case 'c':
        case 'C':
            wordAsInt |= 0b00000000000000000000100000000000;
            break;

        case 'd':
        case 'D':
            wordAsInt |= 0b00000000000000000001000000000000;
            break;

        case 'h':
        case 'H':
            wordAsInt |= 0b00000000000000000010000000000000;
            break;

        case 'm':
        case 'M':
            wordAsInt |= 0b00000000000000000100000000000000;
            break;

        case 'p':
        case 'P':
            wordAsInt |= 0b00000000000000001000000000000000;
            break;

        case 'g':
        case 'G':
            wordAsInt |= 0b00000000000000010000000000000000;
            break;

        case 'k':
        case 'K':
            wordAsInt |= 0b00000000000000100000000000000000;
            break;

        case 'b':
        case 'B':
            wordAsInt |= 0b00000000000001000000000000000000;
            break;

        case 'w':
        case 'W':
            wordAsInt |= 0b00000000000010000000000000000000;
            break;

        case 'f':
        case 'F':
            wordAsInt |= 0b00000000000100000000000000000000;
            break;

        case 'v':
        case 'V':
            wordAsInt |= 0b00000000001000000000000000000000;
            break;

        case 'z':
        case 'Z':
            wordAsInt |= 0b00000000010000000000000000000000;
            break;

        case 'j':
        case 'J':
            wordAsInt |= 0b00000000100000000000000000000000;
            break;

        case 'x':
        case 'X':
            wordAsInt |= 0b00000001000000000000000000000000;
            break;

        case 'q':
        case 'Q':
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

bool checkMultiLetters(std::string word)
{
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = i + 1; j < 5; j++)
        {
            if (std::tolower(word[i]) == std::tolower(word[j]))
            {
                return true;
            }
        }
    }

    return false;
}

bool is_anagram(std::string s1, std::string s2)
{
  std::sort(s1.begin(), s1.end());
  std::sort(s2.begin(), s2.end());
  return s1 == s2;
}
