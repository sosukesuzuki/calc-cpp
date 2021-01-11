#ifndef LEXER_H
#define LEXER_H

#include <vector>

std::vector<std::string> tokenize(std::string str);

bool is_number(std::string* token);

bool is_name(std::string* token);

#endif
