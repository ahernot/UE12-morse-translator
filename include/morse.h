
#include <string>
#include <map>
#include <queue>


std::map<std::string, std::string> emptyMap;

std::string charToMorse (std::string& c,
                         std::map<std::string, std::string>& conversion_map,
                         std::map<std::string, std::string>& preconversion_map = emptyMap);

std::queue<std::string> stringToMorse (std::string& message,
                                       std::map<std::string, std::string>& conversionMap,
                                       std::map<std::string, std::string>& preconversionMap = emptyMap);

