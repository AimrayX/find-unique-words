#include <string>
#include <vector>
#include <map>

std::vector<std::pair<std::string, unsigned int>> importDictionary(std::string fileName);

std::vector<std::vector<std::pair<std::string, unsigned int>>> findWords();

int convertToBitwise(std::string word);

bool checkMultiLetters(std::string word);

bool is_anagram(std::string s1, std::string s2);