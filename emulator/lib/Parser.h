#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Commands.h"
#include "Token.h"

namespace parser {

using ParsingHandler = cmd::ICommandPtr (*) (const token::Command&);

using LabelTable = std::unordered_map<std::string, int>;

class Parser {
public:
    Parser() {}
    Parser(const LabelTable& table): table_(table) {}

    std::vector<cmd::ICommandPtr> parseCommands(const std::vector<token::TokenPtr>& cmdTokens) {
        std::vector<cmd::ICommandPtr> prog;
        for (auto& c: cmdTokens) {
            token::Command& cmdToken = static_cast<token::Command&>(*c);
            auto cmd = parseCommand(cmdToken);
            prog.push_back(std::move(cmd));
        }
        
        return prog;
    }

    cmd::ICommandPtr parseCommand(const token::Command& c) {
        auto search = parsers_.find(c.getName());
        if (search != parsers_.end()) {
            auto parser = search->second;
            return parser(c);
        }
        else {
            return {};
        }
    }
    
    static cmd::ICommandPtr End_parse (const token::Command& token) {
        return std::make_unique<cmd::End>();
    }
    
    static cmd::ICommandPtr MovRI_parse (const token::Command& token) {
        auto arg1 = token.getArgs()[0];
        auto arg2 = token.getArgs()[1];
        auto reg = reg::getRegIdByName(arg1);
        auto word = std::stoi(arg2);
        return std::make_unique<cmd::MovRI>(reg, word);
    }

private:
    LabelTable table_;
    static const std::unordered_map<std::string, ParsingHandler> parsers_;
};


} // namespace parser
