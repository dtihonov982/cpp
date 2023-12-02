#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Commands.h"
#include "Token.h"
#include "Exception.h"

namespace parser {

class Parser;

using LabelTable = std::unordered_map<std::string, int>;

using ParsingHandler = cmd::ICommandPtr (*) (const token::Command&, const LabelTable&);

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
            return parser(c, table_);
        }
        else {
            throw Exception("Can't recognize token \"", c.getName(), "\".\n");
        }
    }
    
private:
    LabelTable table_;
    static const std::unordered_map<std::string, ParsingHandler> parsers_;
};


} // namespace parser
