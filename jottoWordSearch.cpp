#include <fstream>
#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <chrono>

#include "jottoWordSearch.h"

std::mutex queueMutex;
std::condition_variable queueCondition;
bool finished = false;

int main()
{
    
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
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
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

    
    std::ofstream MyFile("sortedDict.txt");
    std::vector<std::pair<std::string, unsigned int>>::iterator its;
    for (its = myMap.begin(); its != myMap.end(); its++)
    {
        MyFile << (its)->first << "\n";
    }
    MyFile.close();
    

    return myMap;
}

std::vector<std::vector<std::pair<std::string, unsigned int>>> findWords()
{
    std::vector<std::pair<std::string, unsigned int>> dictVec = importDictionary("words_alpha.txt");
    int dictVecSize = dictVec.size();
    std::cout << "***********\n\n" << dictVecSize << "\n\n***********" << std::endl;

    std::vector<std::vector<std::pair<std::string, unsigned int>>> wordLists;
    std::vector<std::pair<std::string, unsigned int>>::iterator it;
    std::vector<std::pair<std::string, unsigned int>>::iterator secondToLastDictVecIt = std::prev(dictVec.end()); 

    std::queue<std::vector<std::pair<std::string, unsigned int>>::iterator> taskQueue;
    const int numThreads = 16;
    std::vector<std::thread> threads;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(workerThread, std::ref(taskQueue), std::ref(dictVec), std::ref(wordLists));
    }
    
    //int iter = 0;
    for (it = dictVec.begin(); it != secondToLastDictVecIt; it++)
    {
        //iter++;
        //std::cout << iter << " / " << dictVecSize << std::endl;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(it);
        }

        queueCondition.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        finished = true;
    }
    queueCondition.notify_all();

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Algorithm took: " << elapsed_seconds.count() << " seconds" << std::endl;
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

std::vector<std::vector<std::pair<std::string, unsigned int>>> findNodes(std::promise<std::vector<std::vector<std::pair<std::string, unsigned int>>>>&& promiseObj, std::vector<std::pair<std::string, unsigned int>>::iterator& it, std::vector<std::pair<std::string, unsigned int>>& dictVec) {
    
    std::vector<std::pair<std::string, unsigned int>>::iterator secondToLastDictVecIt = std::prev(dictVec.end());
    std::vector<std::pair<std::string, unsigned int>> tempList;
    std::vector<std::pair<std::string, unsigned int>>::iterator itr;
    std::vector<std::pair<std::string, unsigned int>>::iterator currentElement;

    std::vector<std::pair<std::string, unsigned int>>::iterator node2;
    std::vector<std::pair<std::string, unsigned int>>::iterator node3;
    std::vector<std::pair<std::string, unsigned int>>::iterator node4;

    std::vector<std::vector<std::pair<std::string, unsigned int>>> tempWordLists;

    int tempListSize = 1;
    unsigned int temp;
    temp = (it)->second;

    tempList.push_back(*it);

    for (itr = std::next(it); itr != dictVec.end() && tempListSize < 5; itr++)
        {                       
            if ((temp & (itr)->second) == 0)
            {
                tempList.push_back(*itr);
                temp |= (itr)->second;
                tempListSize += 1;

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
                    //std::cout << "Found\n" << std::endl;
                }
            }

            if (tempListSize == 5)
            {
                //std::vector<std::pair<std::string, unsigned int>>::iterator itf;
                //for (itf = tempList.begin(); itf != tempList.end(); itf++)
                //{
                    //std::cout << "\n*************************" << (itf)->first << "**************************\n";
                //}

                if (node2 == secondToLastDictVecIt)
                {
                    tempWordLists.push_back(tempList);
                    promiseObj.set_value(tempWordLists);
                    return tempWordLists;
                }
                else
                {
                    tempWordLists.push_back(tempList);
                    tempList.pop_back();
                    tempListSize -= 1;
                    temp &= (~((itr)->second));     
                } 
            }
            else if (tempListSize == 2 && itr == secondToLastDictVecIt)
            {
                if (node2 == secondToLastDictVecIt)
                {
                    promiseObj.set_value(tempWordLists);
                    return tempWordLists;
                }

                tempList.pop_back();
                tempListSize -= 1;
                temp &= (~((node2)->second));
                itr = node2;
            }
            else if (tempListSize == 3 && itr == secondToLastDictVecIt)
            {
                if (node3 == secondToLastDictVecIt)
                {
                    itr = node2;
                }
                tempList.pop_back();
                tempListSize -= 1;
                temp &= (~((node3)->second));
                itr = node3;
            }
            else if (tempListSize == 4 && itr == secondToLastDictVecIt)
            {
                if (node4 == secondToLastDictVecIt)
                {
                    itr = node3;
                }
                tempList.pop_back();
                tempListSize -= 1;
                temp &= (~((node4)->second));
                itr = node4;
            }
        }
    promiseObj.set_value(tempWordLists);
    return tempWordLists;
}

void workerThread(std::queue<std::vector<std::pair<std::string, unsigned int>>::iterator>& taskQueue,
                  std::vector<std::pair<std::string, unsigned int>>& dictVec,
                  std::vector<std::vector<std::pair<std::string, unsigned int>>>& wordLists) {
    while (true) {
        std::vector<std::pair<std::string, unsigned int>>::iterator task;

        // Wait for a task to become available
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondition.wait(lock, [&taskQueue]() { return !taskQueue.empty() || finished; });

            if (finished && taskQueue.empty()) return;

            // Get the next task
            task = taskQueue.front();
            taskQueue.pop();
        }

        // Process the task
        std::promise<std::vector<std::vector<std::pair<std::string, unsigned int>>>> promiseObj;
        std::future<std::vector<std::vector<std::pair<std::string, unsigned int>>>> futureObj = promiseObj.get_future();
        
        // Pass the task to `findNodes`
        findNodes(std::move(promiseObj), task, dictVec);

        // Get the result from the future
        std::vector<std::vector<std::pair<std::string, unsigned int>>> completeNodes = futureObj.get();

        // Insert results into wordLists
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            wordLists.insert(wordLists.end(), completeNodes.begin(), completeNodes.end());
        }
    }
}