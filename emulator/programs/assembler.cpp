#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>

void prepareLine(std::string& line) {
}

bool isLabel(const std::string& line) {
    return line.find(":") != std::string::npos;
}

std::string extractLabelFromDefinition(const std::string& line) {
    return line.substr(0, line.size() - 1);
}

int main(int argc, char**argv) {
    if (argc != 2)
        return 1;
    std::ifstream is(argv[1]);
    std::string line;
    int lineNumber = 0;
    std::unordered_map<std::string, int> labels;
    std::vector<std::string> lines;
    while (std::getline(is, line)) {
        prepareLine(line);
        if (line.empty())
            continue;
        if (isLabel(line)) {
            labels[extractLabelFromDefinition(line)] = lineNumber;
            continue;
        }
        lines.push_back(line);
        lineNumber++;
    }



    return 0;
}
