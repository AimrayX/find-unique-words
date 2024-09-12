#include <string>
#include <vector>
#include <map>

std::map<std::string, unsigned int>importDictionary(std::string fileName);

std::vector<std::map<std::string, unsigned int>> findWords();

int convertToBitwise(std::string word);

bool checkMultiLetters(std::string word);

bool is_anagram(std::string s1, std::string s2);