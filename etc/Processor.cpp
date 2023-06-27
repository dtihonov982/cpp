#include <iostream>
#include <array>
#include <algorithm>

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>

#include <cassert>

#define MEM_SIZE 32
#define OPCODE_WC 3
#define TIME_LIMIT 1000
#define FLAGS_COUNT 8

using Address   = int;
using Word      = int;
using Block     = std::vector<Word>;
using Opcode    = std::array<Word, OPCODE_WC>;

namespace reg {

enum RegId { r0 = 0, r1, r2, r3, rip = 100 };
enum FlagId { ZF = 6 };

class Registers {
public:
    Registers() {
        regs_ = {{reg::r0, 0}, {reg::r1, 0}, {reg::r2, 0}, {reg::r3, 0}, {reg::rip, 0}};
    }
    Word getReg(RegId id) {
        return regs_[id];
    }
    void setReg(RegId id, Word data) {
        regs_[id] = data;
    }
    bool getFlag(FlagId id) {
        return flags_[id];
    }
    void setFlag(FlagId id) {
        flags_[id] = true;
    }
    void resetFlag(FlagId id) {
        flags_[id] = false;
    }

    void dump() {
        auto& os = std::cout;
        os << "Registers: \n";
        for (auto [id, val]: regs_) {
            os << id << ": " << val << '\t';
        }
        os << '\n';
        os << "Flags: " << flags_.to_string() << '\n';
    }
//private:
    //using map instead unordered_map for 
    //ordering in dump. speed is doesn't matter 
    std::map<RegId, Word> regs_; //uninitialized
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
        size_t k = 0;
        for (size_t i = 0; i < MEM_SIZE; ++i) {
            std::cout << i << ": " << data_[i] << '\t';
            k++;
            if (k == 3) {
                std::cout << '\n';
                k = 0;
            }
        }
        if (k) std::cout << '\n';
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
}

class ICommandVisitor {
public:
    virtual void visit(cmd::End& cm)   = 0;
    virtual void visit(cmd::AddRR& cm) = 0;
    virtual void visit(cmd::AddRI& cm) = 0;
    virtual void visit(cmd::CmpRI& cm)   = 0;
    virtual void visit(cmd::Jmp& cm)   = 0;
    virtual void visit(cmd::Jz& cm)   = 0;
};


namespace cmd {
    enum Code { 
        END    = 0
      , ADD_RR = 10
      , ADD_RI = 11
      , MOV_RI = 21
      , CMP_RI = 31
      , JMP    = 50
      , JZ
    };

    class ICommand {
    protected:
        ICommand(Code code) : code_(code) {}
    public: 
        Code code_;
        virtual void accept(ICommandVisitor* cv) = 0;
        virtual ~ICommand() {}
    };

    struct End: public ICommand {
        End()
        : ICommand(END) {
        }
        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }
    };


    struct AddRR: public ICommand {
        AddRR(reg::RegId dst, reg::RegId src)
        : ICommand(ADD_RR)
        , dst_reg(dst)
        , src_reg(src) {
        }
        reg::RegId dst_reg;
        reg::RegId src_reg;
        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }
    };

    struct AddRI: public ICommand {
        AddRI(reg::RegId dst, Word val)
        : ICommand(ADD_RI)
        , dst_reg(dst)
        , val_(val) {
        }
        reg::RegId dst_reg;
        Word val_;
        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }
    };

    struct Jmp: public ICommand {
        Jmp(Address addr)
        : ICommand(JMP)
        , addr_(addr) {
        }
        Address addr_;
        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }        
    };

    struct CmpRI: public ICommand {
        CmpRI(reg::RegId reg, Word val)
        : ICommand(CMP_RI)
        , reg_(reg)
        , val_(val) {
        }
        reg::RegId reg_;
        Word val_;
        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }        
    };

    struct Jz: public ICommand {
        Jz(Address addr)
        : ICommand(JZ)
        , addr_(addr) {
        }
        Address addr_;
        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }        
    };

    using list = std::vector<cmd::ICommand*>;
}

using ICommandPtr = std::unique_ptr<cmd::ICommand>;

namespace tr {

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

using DecodingHandler = ICommandPtr (*) (Opcode opc);

class Decoder {
public:
    static ICommandPtr decode(Opcode opc) {
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

    static ICommandPtr End_dec(Opcode opc) {
        cmd::ICommand* cm = new cmd::End;
        return ICommandPtr{cm};
    }

    static ICommandPtr AddRR_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        reg::RegId src_reg = static_cast<reg::RegId>(opc[2]);
        cmd::ICommand* cm = new cmd::AddRR{dst_reg, src_reg};
        return ICommandPtr{cm};
    }

    static ICommandPtr AddRI_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        Word val = opc[2];
        cmd::ICommand* cm = new cmd::AddRI{dst_reg, val};
        return ICommandPtr{cm};
    }

    static ICommandPtr CmpRI_dec(Opcode opc) {
        reg::RegId dst_reg = static_cast<reg::RegId>(opc[1]);
        Word  val = static_cast<Word>(opc[2]);
        return ICommandPtr{new cmd::CmpRI{dst_reg, val}};
    }

    static ICommandPtr Jmp_dec(Opcode opc) {
        assert(opc[2] == 0);
        cmd::ICommand* cm = new cmd::Jmp{opc[1]};
        return ICommandPtr{cm};
    }
    static ICommandPtr Jz_dec(Opcode opc) {
        assert(opc[2] == 0);
        cmd::ICommand* cm = new cmd::Jz{opc[1]};
        return ICommandPtr{cm};
    }
private:
    static const std::unordered_map<cmd::Code, DecodingHandler> decs;
};

const std::unordered_map<cmd::Code, DecodingHandler> Decoder::decs {
      {cmd::END,          End_dec}
    , {cmd::ADD_RR,       AddRR_dec}
    , {cmd::ADD_RI,       AddRI_dec}
    , {cmd::JMP,          Jmp_dec} 
    , {cmd::JZ,           Jz_dec} 
    , {cmd::CMP_RI,       CmpRI_dec} 
};

} //namespace tr

class Processor: public ICommandVisitor {
public:
    Processor(Memory& mem, reg::Registers& regs)
    : mem_(mem)
    , regs_(regs) {
    }

    void run() {
        rip = 0;
        for (int t = 0; isActive_ && t < TIME_LIMIT; ++t) {
            Word rip_before = rip;
            Opcode opc = getNextInstruction();
            auto cm = tr::Decoder::decode(opc);
            cm->accept(this);
            //If command doesn't modifies rip, then go to the next command in memory
            if (rip_before == rip)
                rip += OPCODE_WC;
        }
    }

    Opcode getNextInstruction() {
        assert(isActive_);
        Word instrAddr = getRip();
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
        Word dst_v = regs_.getReg(dst);
        Word src_v = regs_.getReg(src);
        dst_v += src_v;
        regs_.setReg(dst, dst_v);
        if (dst_v == 0)
            regs_.setFlag(reg::ZF);
    }

    void visit(cmd::AddRI& cm) override {
        reg::RegId dst = cm.dst_reg;
        Word dst_v = regs_.getReg(dst);
        Word src_v = cm.val_;
        dst_v += src_v;
        regs_.setReg(dst, dst_v);
        if (dst_v == 0)
            regs_.setFlag(reg::ZF);
    }

    void visit(cmd::End& cm) override {
        assert(isActive_);
        isActive_ = false;
    }

    void visit(cmd::CmpRI& cm) override {
        reg::RegId reg = cm.reg_;
        Word reg_v = regs_.getReg(reg);
        Word tmp = reg_v - cm.val_;
        if (tmp == 0)
            regs_.setFlag(reg::ZF);
    }

    void visit(cmd::Jmp& cm) override {
        setRip(cm.addr_);
    }

    void visit(cmd::Jz& cm) override {
        if (regs_.getFlag(reg::ZF))
           setRip(cm.addr_);
    }
    
    Word getRip() {
        return rip;
    }

    void setRip(Address dst_address) {
        assert(dst_address < mem_.size());
        rip = dst_address;
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
    Address loopLabel = 3;
    Address doneLabel = 3 + 6*3;
    AddRI c0{reg::r0, 100};
    CmpRI c1{reg::r0, 0};
    Jz    c2{doneLabel};
    AddRI c3{reg::r3, 1};
    AddRR c4{reg::r1, reg::r3};
    AddRI c5{reg::r0, -1};
    Jmp   c6{loopLabel};
    cmd::list prog{&c0, &c1, &c2, &c3, &c4, &c5, &c6};

    tr::Encoder encoder;
    Block encodedProg = encoder.encode(prog);

    Memory mem{encodedProg};
    reg::Registers regs;


    Processor proc{mem, regs};
    proc.dump();
    proc.run();

    proc.dump();

    return 0;
}
