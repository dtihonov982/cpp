#ifndef EMULATOR_H
#define EMULATOR_H

#include <vector>
#include <array>

#include "Definitions.h"
#include "Commands.h"
#include "Registers.h"
#include "Memory.h"
#include "Translate.h"

//Stack is a simple interface to registers and memory for instructions Push, Pop, Call etc.
class Stack {
public:
    Stack(Memory& mem, reg::Registers& regs)
    : regs_(regs)
    , mem_(mem) {
        getRsp() = mem_.size();
    }

    Word pop() {
        auto& rsp = getRsp();
        assert(rsp < mem_.size());
        Word data = mem_.get(rsp);
        rsp++;
        return data;
    }

    void push(Word data) {
        auto& rsp = getRsp();
        rsp--;
        mem_.set(rsp, data);
    }

private:
    Word& getRsp() {
        return regs_.get(reg::rsp);
    }

    Memory& mem_;
    reg::Registers& regs_;
};

class Emulator: public ICommandVisitor {
public:
    Emulator(Memory& mem)
    : mem_(mem)
    , stack_(mem_, regs_) {
    }

    void run();

    void dump() {
        std::cout << "--------------Processor dump ---------------\n";
        regs_.dump();
        mem_.dump();
    }
    const reg::Registers& getRegs() {
        return regs_;
    }
private:
    void initFrame() {
        regs_.set(reg::rbp, mem_.size());
    }

    // After each operations call this to set flags.
    void setFlagsFromResult(Word result) {
        regs_.resetAllFlags();
        regs_.set(reg::ZF, result == 0);
        regs_.set(reg::SF, result <  0);
    }

    // This is helper for operations je, jz etc.
    void jump(Address addr) {
        regs_.set(reg::rip, addr);
    }

    Opcode loadInstruction() {
        assert(isActive_);
        Word instrAddr = regs_.get(reg::rip);
        assert(instrAddr <= mem_.size() - OPCODE_WC);
        Opcode opc;
        for (int i = 0; i < OPCODE_WC; ++i) {
            opc[i] = mem_.get(instrAddr + i);
        }
        return opc;
    }

    // Implementation ----------------------------------
    void visit(cmd::AddRR& cm) override {
        reg::RegId dst = cm.dst_reg;
        reg::RegId src = cm.src_reg;
        Word dst_v = regs_.get(dst);
        Word src_v = regs_.get(src);
        dst_v += src_v;
        regs_.set(dst, dst_v);
        setFlagsFromResult(dst_v);
    }

    void visit(cmd::AddRI& cm) override {
        reg::RegId dst = cm.dst_reg;
        Word dst_v = regs_.get(dst);
        Word src_v = cm.val_;
        dst_v += src_v;
        regs_.set(dst, dst_v);
        setFlagsFromResult(dst_v);
    }

    void visit(cmd::MovRI& cm) override {
        reg::RegId dst = cm.dst_reg;
        regs_.set(dst, cm.val_);
    }

    void visit(cmd::MovRR& cm) override {
        auto src_v = regs_.get(cm.src_reg);
        regs_.set(cm.dst_reg, src_v);
    }

    void visit(cmd::MovRRm& cm) override {
        auto addr = regs_.get(cm.src_reg);
        auto val = mem_.get(addr);
        regs_.set(cm.dst_reg, val);
    }

    void visit(cmd::MovRM& cm) override {
        auto src_v = mem_.get(cm.src_addr);
        regs_.set(cm.dst_reg, src_v);
    }

    void visit(cmd::End& cm) override {
        assert(isActive_);
        isActive_ = false;
    }

    void visit(cmd::CmpRI& cm) override {
        reg::RegId reg = cm.reg_;
        Word reg_v = regs_.get(reg);
        Word tmp = reg_v - cm.val_;
        setFlagsFromResult(tmp);
    }

    void visit(cmd::JmpR& cm) override {
        auto addr = regs_.get(cm.reg);
        jump(addr);
    }

    void visit(cmd::JmpI& cm) override {
        jump(cm.addr_);
    }

    void visit(cmd::Jz& cm) override {
        if (regs_.get(reg::ZF))
            jump(cm.addr_);
    }

    void visit(cmd::JlessI& cm) override {
        if (regs_.get(reg::SF))
            jump(cm.addr_);
    }

    void visit(cmd::CallI& cm) override {
        auto curr_rip = regs_.get(reg::rip);
        auto ret_addr = curr_rip + OPCODE_WC; //address of next command after call
        stack_.push(ret_addr);
        jump(cm.addr_);

    }

    void visit(cmd::PushR& cm) override {
        auto src_v = regs_.get(cm.src_reg);
        stack_.push(src_v);
    }

    void visit(cmd::PopR& cm) override {
        regs_.set(cm.dst_reg, stack_.pop());
    }

    void visit(cmd::Leave& cm) override {
        cmd::MovRR c1 {reg::rsp, reg::rbp};
        visit(c1);
        cmd::PopR  c2 {reg::rbp};
        visit(c2);
    }
    void visit(cmd::Ret& cm) override {
        auto retAddr = stack_.pop();
        jump(retAddr);
    }
    // ---------------------------------------------------

    bool isActive_ = true;
    Memory& mem_;
    reg::Registers regs_;
    Stack stack_;
};

#endif
