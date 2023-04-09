#include <string>
#include <regex>
#include <utility>
#include <iostream>
#include <memory>

enum class ExprPartType { oper, func, number, lpar, rpar, ref };

std::ostream& operator<<(std::ostream& os, ExprPartType type) {
    switch (type) {
    case ExprPartType::oper:
        os << "operator";
        break;
    case ExprPartType::func:
        os << "function";
        break;
    case ExprPartType::number:
        os << "number";
        break;
    case ExprPartType::lpar:
        os << "left parenthesis";
        break;
    case ExprPartType::rpar:
        os << "right parenthesis";
        break;
    case ExprPartType::ref:
        os << "reference";
        break;
    }
    return os;
}

class ExprPart {
public:
    ExprPart(ExprPartType type_): type(type_) {}
    ExprPart getType() { return type; }
    virtual ~ExprPart() {}
private:
    ExprPartType type;
};

class Number: public ExprPart {
public:
    Number(int n): ExprPart(ExprPartType::number), value(n) {}
    int getValue() const { return value; }
private:
    int value;
};

std::ostream& operator<<(std::ostream& os, const Number& n) {
	os << n.getValue();
	return os;
}

struct RPar: public ExprPart {
    RPar(): ExprPart(ExprPartType::rpar) {}
};

std::ostream& operator<<(std::ostream& os, const RPar& rpar) {
	os << ')';
	return os;
}

struct LPar: public ExprPart {
    LPar(): ExprPart(ExprPartType::lpar) {}
};

std::ostream& operator<<(std::ostream& os, const LPar& lpar) {
	os << '(';
	return os;
}

class BinOperator: public ExprPart {
public:
    BinOperator(): ExprPart(ExprPartType::oper) {}
    virtual int eval(int lhs, int rhs) = 0;
};

class BinPlus: public BinOperator {
public:
    int eval(int lhs, int rhs) override {
        return lhs + rhs;
    }
};


using Expression = std::vector<std::unique_ptr<ExprPart>>;
class Scanner {
public:
	static const std::vector<std::pair<ExprPartType, std::regex>> regexes;
	
	static const int maxLengthOfWord = 255;
	static bool getFirstWord(const std::string& input, std::string& word, ExprPartType& type) {
    	std::smatch matches;
    	for (auto& [currType, re]: regexes) {
        	if (std::regex_search(input, matches, re)) {
        		word = matches[0];
        		type = currType;
        		return true;
        	}
    	}
    	return false;	
	}
	static Expression getExpression(std::string input) {		
		//deleting spaces
		input.erase(std::remove_if(input.begin(), input.end(), [] (char x) { return x == ' '; }), input.end());
		Expression result;
		std::string word;
		ExprPartType type;
        int value;
		for (int i = 0, recognizedWordLength; i < input.size(); i += recognizedWordLength) {
            //getting short part of whole string that may be too long for fast regex matching
			std::string current = input.substr(i, maxLengthOfWord);
			if (getFirstWord(current, word, type)) {
				recognizedWordLength = word.length();
				switch (type) {
				case ExprPartType::number:
					value = std::stoi(word);
					result.emplace_back(new Number{value});
                    break;
                case ExprPartType::lpar:
                    result.emplace_back(new LPar{});
                    break;
                case ExprPartType::rpar:
                    result.emplace_back(new RPar{});
                    break;
				}
			}
			else {
				throw std::invalid_argument("Can`t recognize \"" + current + "\"");
			}
		}
		return result;
	}
};

const std::vector<std::pair<ExprPartType, std::regex>> Scanner::regexes = {
		{ ExprPartType::number, std::regex("^\\d+") }, 
		{ ExprPartType::oper, std::regex("^[\\+\\-\\*/]") }, 
		{ ExprPartType::lpar, std::regex("^\\(") }, 
		{ ExprPartType::rpar, std::regex("^\\)") }, 
		{ ExprPartType::func, std::regex("^\\w+") }, 
		{ ExprPartType::ref, std::regex("^\\[\\w+\\]") }
};


int main(int argc, char** argv) {
	if (argc == 1) 
		return 0;
	std::string input{argv[1]};
    Expression exprv = Scanner::getExpression(input);
    for (auto&& exprPtr: exprv) {
        std::cout << static_cast<Number&>(*exprPtr) << std::endl;
    }
	return 0;
}
		
			
	

