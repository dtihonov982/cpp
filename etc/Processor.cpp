//TODO: JMP_I, JMP_R
#include <iostream>
#include <memory>
#include <cassert>

#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <bitset>

#define MEM_SIZE 64
#define OPCODE_WC 3     //Opcode words count
#define TIME_LIMIT 1000
#define FLAGS_COUNT 8

using Address   = int;
using Word      = int;
using Block     = std::vector<Word>;
using Opcode    = std::array<Word, OPCODE_WC>;

namespace reg {

enum RegId { r0 = 0, r1, r2, r3, rip = 10, rsp = 15, rbp};
enum FlagId { ZF = 6 };

class Registers {
public:
    Registers() {
    }

    Word get(RegId id) {
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

    void reset(FlagId id) {
        flags_[id] = false;
    }

    void dump() {
        static const std::vector<std::pair<RegId, std::string>> regsRepr {
                    {r0, "r0"}, {r1, "r1"}, {r2, "r2"}, {r3, "r3"}
                  , {rip, "rip"}, {rsp, "rsp"}, {rbp, "rbp"}
                };
        auto& os = std::cout;
        os << "Registers: \n";
        for (auto& [id, repr]: regsRepr) {
            os << repr << ": " << regs_[id] << '\t';
        }
        os << '\n';
        os << "Flags: " << flags_.to_string() << '\n';
    }
private:
    std::unordered_map<RegId, Word> regs_;
    std::bitset<FLAGS_COUNT> flags_;
};

} //namespace reg

class Memory {
public:
    Memory(const std::vector<Word>& data) {
        data_.fill(0);
        assert(data.size() < MEM_SIZE);
        std::copy(data.begin(), data.end(), data_.begin());
    }

    Word get(Address address) {
        return data_[address];
    }

    void set(Address address, Word word) {
        data_[address] = word;
    }

    size_t size() const {
        return data_.size();
    }

    void dump() {
        auto& os = std::cout;
        os << "Memory: \n";
        for (size_t i = 0, k = 0; i < MEM_SIZE; ) {
            os << i << ":\t"; 
            while (i < MEM_SIZE && k != 3) {
                os << data_[i++] << '\t';
                ++k;
            }
            k = 0;
            std::cout << '\n';
        }
    }

private:
    std::array<Word, MEM_SIZE> data_;
};

namespace cmd {
    struct AddRR;
    struct AddRI;
    struct CmpRI;
    struct End;
    struct Jmp;
    struct Jz;
    struct MovRI;
    struct MovRR;
    struct PushR;
    struct PopR;
} //namespace cmd 

class ICommandVisitor {
public:
    virtual void visit(cmd::End& cm)   = 0;
    virtual void visit(cmd::AddRR& cm) = 0;
    virtual void visit(cmd::AddRI& cm) = 0;
    virtual void visit(cmd::MovRI& cm) = 0;
    virtual void visit(cmd::MovRR& cm) = 0;
    virtual void visit(cmd::CmpRI& cm)   = 0;
    virtual void visit(cmd::Jmp& cm)   = 0;
    virtual void visit(cmd::Jz& cm)   = 0;
    virtual void visit(cmd::PushR& cm)   = 0;
    virtual void visit(cmd::PopR& cm)   = 0;

    virtual ~ICommandVisitor() {}
};

namespace cmd {
    enum Code { 
        END    = 0
      , ADD_RR = 10
      , ADD_RI = 11
      , MOV_RR = 20
      , MOV_RI = 21
      , CMP_RI = 31
      , JMP    = 50
      , JZ
      , PUSH_R = 70
      , POP_R  
    };

    class ICommand {
    protected:
        ICommand(Code code) : code_(code) {}
    public: 
        Code code_;
        virtual void accept(ICommandVisitor* cv) = 0;
        virtual ~ICommand() {}
    };

    using ICommandPtr = std::unique_ptr<cmd::ICommand>;

    struct End: public ICommand {
        End()
        : ICommand(END) {
        }
        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }
    };

    struct AddRR: public ICommand {
        reg::RegId dst_reg;
        reg::RegId src_reg;

        AddRR(reg::RegId dst, reg::RegId src)
        : ICommand(ADD_RR)
        , dst_reg(dst)
        , src_reg(src) {
        }

        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }
    };

    struct AddRI: public ICommand {
        reg::RegId dst_reg;
        Word val_;

        AddRI(reg::RegId dst, Word val)
        : ICommand(ADD_RI)
        , dst_reg(dst)
        , val_(val) {
        }

        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }
    };

    struct MovRI: public ICommand {
        reg::RegId dst_reg;
        Word val_;

        MovRI(reg::RegId dst, Word val)
        : ICommand(MOV_RI)
        , dst_reg(dst)
        , val_(val) {
        }

        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }
    };

    struct MovRR: public ICommand {
        reg::RegId dst_reg;
        reg::RegId src_reg;

        MovRR(reg::RegId dst, reg::RegId src)
        : ICommand(MOV_RR)
        , dst_reg(dst)
        , src_reg(src) {
        }

        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }
    };

    struct Jmp: public ICommand {
        Address addr_;

        Jmp(Address addr)
        : ICommand(JMP)
        , addr_(addr) {
        }

        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }        
    };

    struct CmpRI: public ICommand {
        reg::RegId reg_;
        Word val_;

        CmpRI(reg::RegId reg, Word val)
        : ICommand(CMP_RI)
        , reg_(reg)
        , val_(val) {
        }

        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }        
    };

    struct Jz: public ICommand {
        Address addr_;

        Jz(Address addr)
        : ICommand(JZ)
        , addr_(addr) {
        }

        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }        
    };

    struct PushR: public ICommand {
        reg::RegId src_reg;

        PushR(reg::RegId reg)
        : ICommand(PUSH_R)
        , src_reg(reg) {
        }

        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }        
    };

    struct PopR: public ICommand {
        reg::RegId dst_reg;

        PopR(reg::RegId reg)
        : ICommand(POP_R)
        , dst_reg(reg) {
        }
        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }        
    };

    using list = std::vector<cmd::ICommand*>;
}


namespace tr { //translate

class Encoder: public ICommandVisitor {
public:
    void visit(cmd::End& cm) override {
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

    void visit(cmd::Jmp& cm) override {
        Opcode opcode = {cm.code_, cm.addr_, 0};
        append(opcode);
    }

    void visit(cmd::Jz& cm) override {
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
            DecodingHandler dec = search->second;
            return dec(opc);
        }
        else {
            return {};
        }
    }

    static cmd::ICommandPtr End_dec(Opcode opc) {
        cmd::ICommand* cm = new cmd::End;
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr AddRR_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        reg::RegId src_reg = static_cast<reg::RegId>(opc[2]);
        cmd::ICommand* cm = new cmd::AddRR{dst_reg, src_reg};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr AddRI_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        Word val = opc[2];
        cmd::ICommand* cm = new cmd::AddRI{dst_reg, val};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr MovRI_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        Word val = opc[2];
        cmd::ICommand* cm = new cmd::MovRI{dst_reg, val};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr MovRR_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        reg::RegId src_reg = static_cast<reg::RegId>(opc[2]);
        cmd::ICommand* cm = new cmd::MovRI{dst_reg, src_reg};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr CmpRI_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        Word  val = static_cast<Word>(opc[2]);
        return cmd::ICommandPtr{new cmd::CmpRI{dst_reg, val}};
    }

    static cmd::ICommandPtr Jmp_dec(Opcode opc) {
        assert(opc[2] == 0);
        cmd::ICommand* cm = new cmd::Jmp{opc[1]};
        return cmd::ICommandPtr{cm};
    }

    static cmd::ICommandPtr Jz_dec(Opcode opc) {
        assert(opc[2] == 0);
        cmd::ICommand* cm = new cmd::Jz{opc[1]};
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
    , {cmd::JMP,          Jmp_dec} 
    , {cmd::JZ,           Jz_dec} 
    , {cmd::CMP_RI,       CmpRI_dec} 
    , {cmd::PUSH_R,       PushR_dec} 
    , {cmd::POP_R,        PopR_dec} 
};

} //namespace tr

class Processor: public ICommandVisitor {
public:
    
    Processor(Memory& mem, reg::Registers& regs)
    : mem_(mem)
    , regs_(regs) {
    }

    void run() {
        initStack();
        for (int t = 0; isActive_ && t < TIME_LIMIT; ++t) {
            Word rip_before = regs_.get(reg::rip);
            Opcode opc = getNextInstruction();
            auto cm = tr::Decoder::decode(opc);
            cm->accept(this);
            Word rip_after = regs_.get(reg::rip);
            //If command doesn't modifies rip, then go to the next command in memory
            if (rip_before == rip_after)
                regs_.set(reg::rip, rip_before += OPCODE_WC);
        }
    }

    void initStack() {
        regs_.set(reg::rsp, MEM_SIZE - 1);
        regs_.set(reg::rbp, MEM_SIZE - 1);
    }

    Opcode getNextInstruction() {
        assert(isActive_);
        Word instrAddr = regs_.get(reg::rip);
        assert(instrAddr <= mem_.size() - OPCODE_WC);
        Opcode opc;
        for (int i = 0; i < OPCODE_WC; ++i) {
            opc[i] = mem_.get(instrAddr + i);
        }
        return opc;
    }

    //TODO: SF flag
    void visit(cmd::AddRR& cm) override {
        reg::RegId dst = cm.dst_reg;
        reg::RegId src = cm.src_reg;
        Word dst_v = regs_.get(dst);
        Word src_v = regs_.get(src);
        dst_v += src_v;
        regs_.set(dst, dst_v);
        if (dst_v == 0)
            regs_.set(reg::ZF);
    }

    void visit(cmd::AddRI& cm) override {
        reg::RegId dst = cm.dst_reg;
        Word dst_v = regs_.get(dst);
        Word src_v = cm.val_;
        dst_v += src_v;
        regs_.set(dst, dst_v);
        if (dst_v == 0)
            regs_.set(reg::ZF);
    }

    void visit(cmd::MovRI& cm) override {
        reg::RegId dst = cm.dst_reg;
        regs_.set(dst, cm.val_);
    }

    void visit(cmd::MovRR& cm) override {
        auto src_v = regs_.get(cm.src_reg);
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
        if (tmp == 0)
            regs_.set(reg::ZF);
    }

    void visit(cmd::Jmp& cm) override {
        regs_.set(reg::rip, cm.addr_);
    }

    void visit(cmd::Jz& cm) override {
        if (regs_.get(reg::ZF))
            regs_.set(reg::rip, cm.addr_);
    }

    void visit(cmd::PushR& cm) override {
        //SUB RSP, 8
        auto addrInRsp = regs_.get(reg::rsp);
        addrInRsp--; 
        regs_.set(reg::rsp, addrInRsp);

        //MOV [RSP], R0
        auto src_v = regs_.get(cm.src_reg);
        mem_.set(addrInRsp, src_v); 
    }

    void visit(cmd::PopR& cm) override {
        //MOV R0, [RSP]
        auto addrInRsp = regs_.get(reg::rsp);
        auto valueInStack = mem_.get(addrInRsp);
        regs_.set(cm.dst_reg, valueInStack);
        //ADD RSP, 8
        ++addrInRsp;
        regs_.set(reg::rsp, addrInRsp);
    }

    void dump() {
        std::cout << "--------------Processor dump ---------------\n";
        regs_.dump();
        mem_.dump();
    }
private:
    bool isActive_ = true;
    Word rip;
    Memory mem_;
    reg::Registers regs_;
};

int main() {
    using namespace cmd;
    using namespace reg;

    Block encodedProg = {
        MOV_RI, r0, 1
      , MOV_RI, r1, 2
      , MOV_RI, r2, 3
      , PUSH_R, r0, 0
      , PUSH_R, r1, 0
      , PUSH_R, r2, 0
      , POP_R,  r0, 0
      , POP_R,  r1, 0
      , POP_R,  r2, 0
    };

    Memory mem{encodedProg};
    reg::Registers regs;
    Processor proc{mem, regs};
    proc.dump();
    proc.run();
    proc.dump();

    return 0;
}
