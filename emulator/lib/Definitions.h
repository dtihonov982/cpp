#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <vector>
#include <array>

#define MEM_SIZE 128 // Size of RAM
#define OPCODE_WC 3     //Opcode words count
#define TIME_LIMIT 1'000'000 // After this the Emulator stops
#define FLAGS_COUNT 8 // Flag register width in bits

using Address   = int; // Address in RAM
using Word      = int; // Minimal piece of data in RAM
using Block     = std::vector<Word>;
using Opcode    = std::array<Word, OPCODE_WC>; // Code of operation

#endif
