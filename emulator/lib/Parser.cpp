#include "Parser.h"

namespace parser {

    cmd::ICommandPtr End_parse (const token::Command& token, const LabelTable& table) {
        return std::make_unique<cmd::End>();
    }
    
    cmd::ICommandPtr MovRI_parse (const token::Command& token, const LabelTable& table) {
        auto arg1 = token.getArgs()[0];
        auto arg2 = token.getArgs()[1];
        auto reg = reg::getRegIdByName(arg1);
        auto word = std::stoi(arg2);
        return std::make_unique<cmd::MovRI>(reg, word);
    }

    cmd::ICommandPtr CallI_parse(const token::Command& token, const LabelTable& table) {
        auto labelName = token.getArgs()[0];
        auto search = table.find(labelName);
        if (search != table.end())
            return std::make_unique<cmd::CallI>(search->second);
        else
            throw Exception("Can't find label \"", labelName, "\".\n");
    }

const std::unordered_map<std::string, ParsingHandler> Parser::parsers_ {
    {"end", End_parse},
    {"movri", MovRI_parse},
    {"calli", CallI_parse}

};

} // namespace parser
