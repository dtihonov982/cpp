#include <sstream>

#include "Token.h"

namespace token {

bool isLabel(const std::string& line) {
    return line.find(":") != std::string::npos;
}

std::string extractLabelFromDefinition(const std::string& line) {
    return line.substr(0, line.size() - 1);
}

std::string strip(const std::string& s) {
    if (s.empty())
        return s;
    size_t len = s.length();
    int i = 0;
    for (;i < len && isblank(s[i]); i++);
    int j = len - 1;
    for (;j >= 0 && isblank(s[j]); j--);
    return s.substr(i, j - i + 1);
}

std::vector<std::string> split(std::string_view input, char delim) {
    std::istringstream iss{input.data()};
    std::string token;
    std::vector<std::string> result;
    while(std::getline(iss, token, delim)) {
        result.push_back(token);
    }
    return result;
}

std::vector<TokenPtr> tokenize(std::istream& is) {
    std::string line;
    std::vector<TokenPtr> tokens;
    while (std::getline(is, line)) {
        line = strip(line);
        if (line.empty())
            continue;

        if (isLabel(line)) {
            std::string labelName = extractLabelFromDefinition(line); 
            TokenPtr token = std::make_shared<Label>(labelName);
            tokens.push_back(token);
            continue;
        }
        else { // Command
            auto raw_ts = split(line, ' ');
            std::vector<std::string> ts;
            for (auto& t: raw_ts) {
                if (strip(t).empty()) continue;
                // Delete comma
                if (*t.rbegin() == ',') t.erase(t.end() - 1);
                ts.push_back(t);
            }
            std::vector<std::string> args(ts.size() - 1);
            std::copy(ts.begin() + 1, ts.end(), args.begin());
            TokenPtr token = std::make_shared<Command>(ts[0], args);
            tokens.push_back(token);
        }
    }

    return tokens;
}

} // namespace token
