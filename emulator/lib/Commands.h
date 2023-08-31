#ifndef COMMANDS_H
#define COMMANDS_H

#include <memory>
#include "Definitions.h"
#include "Registers.h"

//Forward declaration command for ICommandVisitor declaration 
namespace cmd {
    struct End;

    //Addressation modes:
    //R - register
    //I - immediate

    struct AddRR;
    struct AddRI;

    struct CmpRI;

    struct JmpR;
    struct JmpI;
    struct Jz;

    //It is hard to read JlI so JlessI
    struct JlessI;

    struct CallI;

    struct MovRI;
    struct MovRR;
    struct MovRM;

    struct PushR;
    struct PopR;

    struct Leave;
    struct Ret;

    //Codes uses for encode and decode commands from binary code
    //TODO: move to translate namespace
    enum Code { 
        END    = 0

      , ADD_RR = 10
      , ADD_RI

      , MOV_RR = 20
      , MOV_RI
      , MOV_RM

      , CMP_RI = 41

      , JMP_R  = 50
      , JMP_I
      , JZ
      , JL_I

      , CALL_I = 60

      , PUSH_R = 70
      , POP_R  

      , LEAVE  = 100
      , RET 
    };
} //namespace cmd 

//Use visitor pattern to avoid switch-case in command handlers
class ICommandVisitor {
public:
    virtual void visit(cmd::End& cm)    = 0;
    virtual void visit(cmd::AddRR& cm)  = 0;
    virtual void visit(cmd::AddRI& cm)  = 0;
    virtual void visit(cmd::MovRI& cm)  = 0;
    virtual void visit(cmd::MovRR& cm)  = 0;
    virtual void visit(cmd::MovRM& cm)  = 0;
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

    //This helper class allows to not wirte accept method in each command
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

    //Stop programm execution
    struct End: public ICommandHelper<End> {
        End()
        : ICommandHelper<End>(END) {
        }
    };

    //Add values of two registers and store result in the first register
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

    struct MovRM: public ICommandHelper<MovRM> {
        reg::RegId dst_reg;
        Address src_addr;

        MovRM(reg::RegId dst, Address src)
        : ICommandHelper<MovRM>(MOV_RM)
        , dst_reg(dst)
        , src_addr(src) {
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
#endif
