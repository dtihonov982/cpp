#ifndef REGISTERS_H
#define REGISTERS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include "Definitions.h"

namespace reg {

enum RegId { r0 = 0, r1, r2, r3, rip = 10, rsp = 15, rbp};
enum FlagId { ZF = 6, SF = 7 };

class Registers {
public:
    Registers() {
    }

    Word& get(RegId id) {
        return regs_[id];
    }

    void set(RegId id, Word data) {
        regs_[id] = data;
    }

    bool get(FlagId id) {
        return flags_[id];
    }

    void set(FlagId id) {
        flags_[id] = true;
    }

    void set(FlagId id, bool p) {
        flags_[id] = p;
    }

    void resetFlags() {
        flags_.reset();
    }

    void dump() {
        //Registers values stores in inordered map so need to store order of printing somethere else.
        static const std::vector<std::pair<RegId, std::string>> regsRepr {
                    {r0, "r0"}, {r1, "r1"}, {r2, "r2"}, {r3, "r3"}
                  , {rip, "rip"}, {rsp, "rsp"}, {rbp, "rbp"}
                };
        //Maybe dumps will get ostream argument in future...
        auto& os = std::cout;
        os << "Registers: \n";
        for (auto& [id, repr]: regsRepr) {
            os << repr << ": " << regs_[id] << '\n';
        }
        //os << '\n';
        os << "Flags: " << flags_.to_string() << '\n';
    }
private:
    std::unordered_map<RegId, Word> regs_;
    std::bitset<FLAGS_COUNT> flags_;
};

} //namespace reg
#endif
