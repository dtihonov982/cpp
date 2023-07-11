//Emulation of processor with assembly language
#include <iostream>
#include <memory>
#include <cassert>

#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <bitset>
#include <utility>

#define MEM_SIZE 128
#define OPCODE_WC 3     //Opcode words count
#define TIME_LIMIT 1'000'000
#define FLAGS_COUNT 8

using Address   = int;
using Word      = int;
using Block     = std::vector<Word>;
using Opcode    = std::array<Word, OPCODE_WC>;

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
        static const std::vector<std::pair<RegId, std::string>> regsRepr {
                    {r0, "r0"}, {r1, "r1"}, {r2, "r2"}, {r3, "r3"}
                  , {rip, "rip"}, {rsp, "rsp"}, {rbp, "rbp"}
                };
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

template <typename It>
void truncatePrint(It begin, It end, std::ostream& os, int width) {
    size_t i = 0;
    for (auto it = begin; it != end; ) {
        os << i << ":\t";
        for (size_t k = 0; it != end && k != width; ++k) {
            os << *it << '\t';
            ++it;
        }
        i += width;
        std::cout << '\n';
    }
}



class Memory {
public:
    Memory(const Block& data) {
        data_.fill(0);
        readStat_.fill(0);
        writeStat_.fill(0);
        assert(data.size() < MEM_SIZE);
        std::copy(data.begin(), data.end(), data_.begin());
    }

    Word get(Address address) {
        ++readStat_[address];
        return data_[address];
    }

    void set(Address address, Word word) {
        ++writeStat_[address];
        data_[address] = word;
    }

    size_t size() const {
        return data_.size();
    }

    void dump() {
        auto& os = std::cout;
        os << "Memory: \n";
        truncatePrint(data_.begin(), data_.end(), std::cout, OPCODE_WC);
    }

    void dumpReadings() {
        auto& os = std::cout;
        os << "Readings of memory: \n";
        truncatePrint(readStat_.begin(), readStat_.end(), std::cout, OPCODE_WC);        
    }

    void dumpWritings() {
        auto& os = std::cout;
        os << "Writing to memory: \n";
        truncatePrint(writeStat_.begin(), writeStat_.end(), std::cout, OPCODE_WC);        
    }
private:
    std::array<Word,   MEM_SIZE> data_;
    std::array<size_t, MEM_SIZE> readStat_;
    std::array<size_t, MEM_SIZE> writeStat_;
};

namespace cmd {
    struct AddRR;
    struct AddRI;
    struct CmpRI;
    struct End;
    struct JmpR;
    struct JmpI;
    struct Jz;
    struct JlessI;
    struct CallI;
    struct MovRI;
    struct MovRR;
    struct PushR;
    struct PopR;
    struct Leave;
    struct Ret;

    enum Code { 
        END    = 0
      , ADD_RR = 10
      , ADD_RI = 11
      , MOV_RR = 20
      , MOV_RI = 21
      , CMP_RI = 31
      , JMP_R  = 50
      , JMP_I
      , JZ
      , JL_I
      , CALL_I
      , PUSH_R = 70
      , POP_R  
      , RET
      , LEAVE
    };
} //namespace cmd 

class ICommandVisitor {
public:
    virtual void visit(cmd::End& cm)    = 0;
    virtual void visit(cmd::AddRR& cm)  = 0;
    virtual void visit(cmd::AddRI& cm)  = 0;
    virtual void visit(cmd::MovRI& cm)  = 0;
    virtual void visit(cmd::MovRR& cm)  = 0;
    virtual void visit(cmd::CmpRI& cm)  = 0;
    virtual void visit(cmd::JmpR& cm)   = 0;
    virtual void visit(cmd::JmpI& cm)   = 0;
    virtual void visit(cmd::Jz& cm)     = 0;
    virtual void visit(cmd::JlessI& cm) = 0;
    virtual void visit(cmd::CallI& cm)  = 0;
    virtual void visit(cmd::PushR& cm)  = 0;
    virtual void visit(cmd::PopR& cm)   = 0;
    virtual void visit(cmd::Leave& cm)  = 0;
    virtual void visit(cmd::Ret& cm)    = 0;

    virtual ~ICommandVisitor() {}
};

namespace cmd {

    class ICommand {
    protected:
        ICommand(Code code) : code_(code) {}
    public: 
        Code code_;
        virtual void accept(ICommandVisitor* cv) = 0;
        virtual ~ICommand() {}
    };

    template <typename Derived>
    class ICommandHelper: public ICommand {
    public:
        ICommandHelper(Code code): ICommand(code) {}
        void accept(ICommandVisitor* cv) override {
            cv->visit(static_cast<Derived&>(*this));
        }
        virtual ~ICommandHelper() {}
    };

    using ICommandPtr = std::unique_ptr<cmd::ICommand>;

    struct End: public ICommandHelper<End> {
        End()
        : ICommandHelper<End>(END) {
        }
    };

    struct AddRR: public ICommandHelper<AddRR> {
        reg::RegId dst_reg;
        reg::RegId src_reg;

        AddRR(reg::RegId dst, reg::RegId src)
        : ICommandHelper<AddRR> (ADD_RR)
        , dst_reg(dst)
        , src_reg(src) {
        }
    };

    struct AddRI: public ICommandHelper<AddRI>  {
        reg::RegId dst_reg;
        Word val_;

        AddRI(reg::RegId dst, Word val)
        : ICommandHelper<AddRI>(ADD_RI)
        , dst_reg(dst)
        , val_(val) {
        }

    };

    struct MovRI: public ICommandHelper<MovRI> {
        reg::RegId dst_reg;
        Word val_;

        MovRI(reg::RegId dst, Word val)
        : ICommandHelper<MovRI>(MOV_RI)
        , dst_reg(dst)
        , val_(val) {
        }

    };

    struct MovRR: public ICommandHelper<MovRR> {
        reg::RegId dst_reg;
        reg::RegId src_reg;

        MovRR(reg::RegId dst, reg::RegId src)
        : ICommandHelper<MovRR>(MOV_RR)
        , dst_reg(dst)
        , src_reg(src) {
        }
    };

    struct JmpR: public ICommandHelper<JmpR> {
        reg::RegId reg;

        JmpR(reg::RegId reg_)
        : ICommandHelper<JmpR>(JMP_R)
        , reg(reg_) {
        }
 
    };

    struct JmpI: public ICommand {
        Address addr_;

        JmpI(Address addr)
        : ICommand(JMP_I)
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

    struct JlessI: public ICommand {
        Address addr_;

        JlessI(Address addr)
        : ICommand(JL_I)
        , addr_(addr) {
        }

        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }        
    };

    struct CallI: public ICommand {
        Address addr_;

        CallI(Address addr)
        : ICommand(CALL_I)
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
    struct Ret: public ICommand {
        Ret()
        : ICommand(RET) {
        }
        void accept(ICommandVisitor* cv) override {
            cv->visit(*this);
        }
    };
    struct Leave: public ICommand {
        Leave()
        : ICommand(LEAVE) {
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

class Processor: public ICommandVisitor {
public:
    Processor(Memory& mem)
    : mem_(mem)
    , stack_(mem_, regs_) {
    }

    void run() {
        initFrame();
        int t;
        for (t = 0; isActive_ && t < TIME_LIMIT; ++t) {
            Word rip_before = regs_.get(reg::rip);
            Opcode opc = getNextInstruction();
            auto cm = tr::Decoder::decode(opc);

            //Command execution
            cm->accept(this);

            Word rip_after = regs_.get(reg::rip);
            //If command doesn't modifies rip, then go to the next command in memory
            if (rip_before == rip_after)
                regs_.set(reg::rip, rip_before += OPCODE_WC);
        }
        if (t >= TIME_LIMIT) std::cout << "Time limit exceeded! ";
        std::cout << "Clock: " << t << '\n';
    }

    void dump() {
        std::cout << "--------------Processor dump ---------------\n";
        regs_.dump();
        mem_.dump();
        mem_.dumpReadings();
        mem_.dumpWritings();
    }
private:
   void initFrame() {
        regs_.set(reg::rbp, mem_.size());
    }

    void setFlagsFromResult(Word result) {
        regs_.set(reg::ZF, result == 0);
        regs_.set(reg::SF, result <  0);
    }

    void jump(Address addr) {
        regs_.set(reg::rip, addr);
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

int main() {
    using namespace cmd;
    using namespace reg;
    //Fibonacci number n=19
    Block encodedProg = {
         MOV_RI,	r1,	19   //int n = ...
        ,CALL_I,	9,	0   // fib(n)
        ,END,	    0,	0

/* 9*/  ,PUSH_R,	rbp,	0
        ,MOV_RR,	rbp,	 rsp
        ,CMP_RI,	r1,	2   //if (n < 2)
        ,JL_I,	    54,	0   //  goto .done

        ,ADD_RI,	r1,	-1  
        ,PUSH_R,	r1,	0   
/*27*/  ,CALL_I,	9,	0   //fib(n-1)

        ,POP_R,	    r1,	0   
        ,PUSH_R ,	r0,	0
        ,ADD_RI,	r1,	-1
        ,CALL_I,	9,	0   //fib(n-2)

/*42*/  ,POP_R,	    r2,	0
        ,ADD_RR,	r0,	 r2 

        ,LEAVE,	    0,	0
        ,RET,	    0,	0

//.done
/*54*/  ,MOV_RR,    r0, r1  //int result = n;
        ,LEAVE,	    0,	0   
        ,RET,	    0,	0   //return result
    };

    Memory mem{encodedProg};

    Processor proc{mem};
    //proc.dump();
    proc.run();
    proc.dump();


    return 0;
}
