#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <vector>
#include <array>

#define MEM_SIZE 128
#define OPCODE_WC 3     //Opcode words count
#define TIME_LIMIT 1'000'000
#define FLAGS_COUNT 8

using Address   = int;
using Word      = int;
using Block     = std::vector<Word>;
using Opcode    = std::array<Word, OPCODE_WC>;

#endif
