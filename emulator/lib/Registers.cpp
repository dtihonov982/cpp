#include <stdexcept>

#include "Registers.h"

namespace reg {

RegId getRegIdByName(const std::string& name) {
    static const std::unordered_map<std::string, RegId> names = {
        {"r0", r0},
        {"r1", r1},
        {"r2", r2},
        {"r3", r3},
        {"rip", rip},
        {"rsp", rbp},
        {"rbp", rbp}
    };

    auto search = names.find(name);
    if (search != names.end()) {
        return search->second;
    }
    else {
        throw std::runtime_error("Bad reg name.");
    }
}


} // namespace reg
