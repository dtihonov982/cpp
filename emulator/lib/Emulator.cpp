#include "Emulator.h"

void Emulator::run() {
    initFrame();
    int t;
    for (t = 0; isActive_ && t < TIME_LIMIT; ++t) {
        Word rip_before = regs_.get(reg::rip);
        Opcode opc = loadInstruction();
        auto cm = tr::Decoder::decode(opc);

        //Command execution
        cm->accept(this);

        Word rip_after = regs_.get(reg::rip);
        //If command doesn't modifies rip, then go to the next command in memory
        if (rip_before == rip_after)
            regs_.set(reg::rip, rip_before += OPCODE_WC);
    }
    if (t >= TIME_LIMIT) std::cout << "Time limit exceeded! ";
    //std::cout << "Clock: " << t << '\n';
}
