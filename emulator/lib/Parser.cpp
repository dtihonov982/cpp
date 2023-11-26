#include "Parser.h"

namespace parser {

const std::unordered_map<std::string, ParsingHandler> Parser::parsers_ {
    {"end", End_parse},
    {"movri", MovRI_parse}
};

} // namespace parser
