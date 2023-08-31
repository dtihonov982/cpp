#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "Commands.h"
#include "Emulator.h"
#include "Definitions.h"
#include <cassert>
#include <memory>
#include <unordered_map>

namespace tr { //tr means translate

class Encoder: public ICommandVisitor {
public:
    void visit(cmd::End& cm) override {
        Opcode opcode = {cm.code_, 0, 0};
        append(opcode);
    }

    void visit(cmd::Ret& cm) override {
        Opcode opcode = {cm.code_, 0, 0};
        append(opcode);
    }

    void visit(cmd::Leave& cm) override {
        Opcode opcode = {cm.code_, 0, 0};
        append(opcode);
    }

    void visit(cmd::AddRR& cm) override {
        Opcode opcode = {cm.code_, cm.dst_reg, cm.src_reg};
        append(opcode);
    }

    void visit(cmd::AddRI& cm) override {
        Opcode opcode = {cm.code_, cm.dst_reg, cm.val_};
        append(opcode);
    }

    void visit(cmd::MovRI& cm) override {
        Opcode opcode = {cm.code_, cm.dst_reg, cm.val_};
        append(opcode);
    }

    void visit(cmd::MovRR& cm) override {
        Opcode opcode = {cm.code_, cm.dst_reg, cm.src_reg};
        append(opcode);
    }

    void visit(cmd::JmpR& cm) override {
        Opcode opcode = {cm.code_, cm.reg, 0};
        append(opcode);
    }

    void visit(cmd::JmpI& cm) override {
        Opcode opcode = {cm.code_, cm.addr_, 0};
        append(opcode);
    }

    void visit(cmd::Jz& cm) override {
        Opcode opcode = {cm.code_, cm.addr_, 0};
        append(opcode);
    }

    void visit(cmd::JlessI& cm) override {
        Opcode opcode = {cm.code_, cm.addr_, 0};
        append(opcode);
    }

    void visit(cmd::CallI& cm) override {
        Opcode opcode = {cm.code_, cm.addr_, 0};
        append(opcode);
    }

    void visit(cmd::CmpRI& cm) override {
        Opcode opcode = {cm.code_, cm.reg_, cm.val_};
        append(opcode);
    }

    void visit(cmd::PushR& cm) override {
        Opcode opcode = {cm.code_, cm.src_reg, 0};
        append(opcode);
    }

    void visit(cmd::PopR& cm) override {
        Opcode opcode = {cm.code_, cm.dst_reg, 0};
        append(opcode);
    }

    void init() {
        buffer.clear();
    }

    Block getBinaryCode() {
        assert(buffer.size() % OPCODE_WC == 0);
        return buffer;
    }

    Block encode(cmd::list& prog) {
        init();
        for (auto cm: prog) {
            cm->accept(this);
        }
        return getBinaryCode();
    }
private:
    Block buffer;

    void append(Opcode opcode) {
        for (Word w: opcode) {
            buffer.push_back(w);
        }
    }
};

using DecodingHandler = cmd::ICommandPtr (*) (Opcode opc);

class Decoder {
public:
    static cmd::ICommandPtr decode(Opcode opc) {
        cmd::Code code = static_cast<cmd::Code>(opc[0]);
        auto search = decs.find(code);
        if (search != decs.end()) {
            auto dec = search->second;
            return dec(opc);
        }
        else {
            return {};
        }
    }

    static cmd::ICommandPtr End_dec(Opcode opc) {
        return std::make_unique<cmd::End>();
    }

    static cmd::ICommandPtr Ret_dec(Opcode opc) {
        return std::make_unique<cmd::Ret>();
    }

    static cmd::ICommandPtr Leave_dec(Opcode opc) {
        return std::make_unique<cmd::Leave>();
    }

    static cmd::ICommandPtr AddRR_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        reg::RegId src_reg = static_cast<reg::RegId>(opc[2]);
        return std::make_unique<cmd::AddRR>(dst_reg, src_reg);
    }

    static cmd::ICommandPtr AddRI_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        Word val = opc[2];
        return std::make_unique<cmd::AddRI>(dst_reg, val);
    }

    static cmd::ICommandPtr MovRI_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        Word val = opc[2];
        return std::make_unique<cmd::MovRI>(dst_reg, val);
    }

    static cmd::ICommandPtr MovRR_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        reg::RegId src_reg = static_cast<reg::RegId>(opc[2]);
        return std::make_unique<cmd::MovRR>(dst_reg, src_reg);
    }

    static cmd::ICommandPtr CmpRI_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        Word  val = static_cast<Word>(opc[2]);
        return cmd::ICommandPtr{new cmd::CmpRI{dst_reg, val}};
    }

    static cmd::ICommandPtr JmpR_dec(Opcode opc) {
        assert(opc[2] == 0);
        reg::RegId reg = static_cast<reg::RegId>(opc[1]);
        cmd::ICommand* cm = new cmd::JmpR{reg};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr JmpI_dec(Opcode opc) {
        assert(opc[2] == 0);
        cmd::ICommand* cm = new cmd::JmpI{opc[1]};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr Jz_dec(Opcode opc) {
        assert(opc[2] == 0);
        cmd::ICommand* cm = new cmd::Jz{opc[1]};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr JlessI_dec(Opcode opc) {
        assert(opc[2] == 0);
        cmd::ICommand* cm = new cmd::JlessI{opc[1]};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr CallI_dec(Opcode opc) {
        assert(opc[2] == 0);
        cmd::ICommand* cm = new cmd::CallI{opc[1]};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr PushR_dec(Opcode opc) {
        assert(opc[2] == 0);
        reg::RegId src_reg = static_cast<reg::RegId>(opc[1]);
        cmd::ICommand* cm = new cmd::PushR{src_reg};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr PopR_dec(Opcode opc) {
        assert(opc[2] == 0);
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        cmd::ICommand* cm = new cmd::PopR{dst_reg};
        return cmd::ICommandPtr{cm};
    }
private:
    static const std::unordered_map<cmd::Code, DecodingHandler> decs;
};

const std::unordered_map<cmd::Code, DecodingHandler> Decoder::decs {
      {cmd::END,          End_dec}
    , {cmd::ADD_RR,       AddRR_dec}
    , {cmd::ADD_RI,       AddRI_dec}
    , {cmd::MOV_RR,       MovRR_dec}
    , {cmd::MOV_RI,       MovRI_dec}
    , {cmd::JMP_R,        JmpR_dec} 
    , {cmd::JMP_I,        JmpI_dec} 
    , {cmd::JZ,           Jz_dec} 
    , {cmd::JL_I,         JlessI_dec} 
    , {cmd::CALL_I,       CallI_dec} 
    , {cmd::CMP_RI,       CmpRI_dec} 
    , {cmd::PUSH_R,       PushR_dec} 
    , {cmd::POP_R,        PopR_dec} 
    , {cmd::RET,          Ret_dec} 
    , {cmd::LEAVE,        Leave_dec} 
};

} //namespace tr

#endif
