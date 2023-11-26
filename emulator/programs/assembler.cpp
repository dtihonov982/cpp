#include <fstream>
#include <iostream>

#include "Token.h"
#include "Parser.h"

using LabelTable = std::unordered_map<std::string, int>;

LabelTable getLabelTable(const std::vector<token::TokenPtr>& tokens) {
    int i = 0;
    LabelTable table;
    for (const auto& t: tokens) {
        if (t->what == token::Token::LABEL) {
            token::Label& label = static_cast<token::Label&>(*t);
            table[label.getName()] = i;
        }
        else {
            i++;
        }
    }

    return table;
}

std::vector<token::TokenPtr> getOnlyCommands(std::vector<token::TokenPtr>& tokens) {
    std::vector<token::TokenPtr> commands;
    for (auto&t: tokens)
        if (t->what == token::Token::COMMAND) {
            commands.push_back(t);
        }
    return commands;
}

int main(int argc, char**argv) {
    if (argc != 2)
        return 1;
    std::ifstream is(argv[1]);

    auto tokens = token::tokenize(is);

    auto table = getLabelTable(tokens);
    auto comms = getOnlyCommands(tokens);

    parser::Parser parser(table);
    auto prog = parser.parseCommands(comms);

    return 0;
}
