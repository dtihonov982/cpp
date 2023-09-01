#include <gtest/gtest.h>

#include "Commands.h"
#include "Registers.h"
#include "Memory.h"
#include "Emulator.h"

TEST(command, movRRm) {
    using namespace cmd;
    using namespace reg;
    //Fibonacci number n=19. r0 = f(r1)
    Block encodedProg = {
         MOV_RI,    r1, 11
        ,MOV_RRm,	r2,	r1   // mov r2, [r1]
        ,END,	    0,	0
        ,0,         101, 202
    };

    Memory mem{encodedProg};
    Emulator proc{mem};
    proc.run();
    auto regs = proc.getRegs();

    EXPECT_EQ(regs.get(reg::r2), 202);
}
