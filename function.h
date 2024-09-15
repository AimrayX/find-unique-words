#include <string>
#include <vector>
#include <map>

std::vector<std::pair<std::string, unsigned int>> importDictionary(std::string fileName);

std::vector<std::vector<std::pair<std::string, unsigned int>>> findWords();

std::vector<std::vector<std::pair<std::string, unsigned int>>> findNodes(std::vector<std::pair<std::string, unsigned int>>::iterator& it, std::vector<std::pair<std::string, unsigned int>>& dictVec);

int convertToBitwise(std::string word);

bool checkMultiLetters(std::string word);

bool is_anagram(std::string s1, std::string s2);