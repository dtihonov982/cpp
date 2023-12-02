#include "Parser.h"

namespace parser {

    cmd::ICommandPtr End_parse (const token::Command& token, const LabelTable& table) {
        return std::make_unique<cmd::End>();
    }
    
    cmd::ICommandPtr MovRI_parse (const token::Command& token, const LabelTable& table) {
        auto arg1 = token.getArg(0);
        auto arg2 = token.getArg(1);
        auto reg = reg::getRegIdByName(arg1);
        auto word = std::stoi(arg2);
        return std::make_unique<cmd::MovRI>(reg, word);
    }

    cmd::ICommandPtr CallI_parse(const token::Command& token, const LabelTable& table) {
        auto labelName = token.getArg(0);
        auto search = table.find(labelName);
        if (search != table.end())
            return std::make_unique<cmd::CallI>(search->second);
        else
            throw Exception("Can't find label \"", labelName, "\".\n");
    }

    cmd::ICommandPtr JlessI_parse(const token::Command& token, const LabelTable& table) {
        auto labelName = token.getArg(0);
        auto search = table.find(labelName);
        if (search != table.end())
            return std::make_unique<cmd::JlessI>(search->second);
        else
            throw Exception("Can't find label \"", labelName, "\".\n");
    }

    cmd::ICommandPtr PushR_parse(const token::Command& token, const LabelTable& table) {
        auto regName = token.getArg(0);
        auto regId = reg::getRegIdByName(regName);
        return std::make_unique<cmd::PushR>(regId);
    }

    cmd::ICommandPtr PopR_parse(const token::Command& token, const LabelTable& table) {
        auto regName = token.getArg(0);
        auto regId = reg::getRegIdByName(regName);
        return std::make_unique<cmd::PopR>(regId);
    }

    cmd::ICommandPtr MovRR_parse(const token::Command& token, const LabelTable& table) {
        auto regDstName = token.getArg(0);
        auto regSrcName = token.getArg(1);
        auto dstRegId = reg::getRegIdByName(regDstName);
        auto srcRegId = reg::getRegIdByName(regSrcName);
        return std::make_unique<cmd::MovRR>(dstRegId, srcRegId);
    }

    cmd::ICommandPtr Ret_parse(const token::Command& token, const LabelTable& table) {
        return std::make_unique<cmd::Ret>();
    }

    cmd::ICommandPtr Leave_parse(const token::Command& token, const LabelTable& table) {
        return std::make_unique<cmd::Leave>();
    }

    cmd::ICommandPtr AddRR_parse(const token::Command& token, const LabelTable& table) {
        auto regDstName = token.getArg(0);
        auto regSrcName = token.getArg(1);
        auto dstRegId = reg::getRegIdByName(regDstName);
        auto srcRegId = reg::getRegIdByName(regSrcName);
        return std::make_unique<cmd::AddRR>(dstRegId, srcRegId);
    }

    cmd::ICommandPtr AddRI_parse(const token::Command& token, const LabelTable& table) {
        auto regDstName = token.getArg(0);
        auto dstRegId = reg::getRegIdByName(regDstName);
        auto value = std::stoi(token.getArg(1));
        return std::make_unique<cmd::AddRI>(dstRegId, value);
    }

    cmd::ICommandPtr CmpRI_parse(const token::Command& token, const LabelTable& table) {
        auto regLhsName = token.getArg(0);
        auto lhsRegId = reg::getRegIdByName(regLhsName);
        auto value = std::stoi(token.getArg(1));
        return std::make_unique<cmd::CmpRI>(lhsRegId, value);
    }

const std::unordered_map<std::string, ParsingHandler> Parser::parsers_ {
    {"end", End_parse}
    ,{"movri", MovRI_parse}
    ,{"calli", CallI_parse}
    ,{"pushr", PushR_parse}
    ,{"popr", PushR_parse}
    ,{"movrr", MovRR_parse}
    ,{"ret", Ret_parse}
    ,{"leave", Leave_parse}
    ,{"addrr", AddRR_parse}
    ,{"addri", AddRI_parse}
    ,{"jli", JlessI_parse}
    ,{"cmpri", CmpRI_parse}
};

} // namespace parser
