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

RegId getRegIdByName(const std::string& name);

// CPU Registers
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

    void set(FlagId id, bool p) {
        flags_[id] = p;
    }

    void resetAllFlags() {
        flags_.reset();
    }

    void dump() {
        // Registers values store in unordered map, so it is need to store an order of printing somewhere else.
        static const std::vector<std::pair<RegId, std::string>> regsRepr {
                    {r0, "r0"}, {r1, "r1"}, {r2, "r2"}, {r3, "r3"}
                  , {rip, "rip"}, {rsp, "rsp"}, {rbp, "rbp"}
                };
        std::cout << "Registers: \n";
        for (auto& [id, repr]: regsRepr) {
            std::cout << repr << ": " << regs_[id] << '\n';
        }
        std::cout << "Flags: " << flags_.to_string() << '\n';
    }
private:
    // This map stores values of CPU registers
    std::unordered_map<RegId, Word> regs_;
    // flag register
    std::bitset<FLAGS_COUNT> flags_;
};

} //namespace reg
#endif
