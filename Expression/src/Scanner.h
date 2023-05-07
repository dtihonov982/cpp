#ifndef SCANNER_H
#define SCANNER_H

#include "Expression.h"

class Scanner {
public:
    static const int maxLengthOfWord;
	static bool getFirstWord(const std::string& input, std::string& word, ExprPartType& type);
	static Expression getExpression(std::string input);
};

#endif
