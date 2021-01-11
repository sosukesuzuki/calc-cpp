#include <vector>
#include <regex>

std::vector<std::string> tokenize(std::string str) {
    std::regex pattern("[A-Za-z]+|[0-9]+|\\S");
    std::sregex_iterator itr{str.begin(), str.end(), pattern};
    std::sregex_iterator last{};
    std::vector<std::string> tokens;
    for (; itr != last; ++itr) {
      tokens.push_back((*itr).str());
    }
    return tokens;
}

bool is_number(std::string* token) {
    if (!token) {
        return false;
    }
    return regex_match(*token, std::regex("[0-9]+"));
}

bool is_name(std::string* token) {
    if (!token) {
        return false;
    }
    return regex_match(*token, std::regex("[A-Za-z]+"));
}
