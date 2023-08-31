//Emulation of processor with assembly language
#include <iostream>

#include "Commands.h"
#include "Registers.h"
#include "Memory.h"
#include "Emulator.h"

int main() {
    using namespace cmd;
    using namespace reg;
    //Fibonacci number n=19. r0 = f(r1)
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

    Emulator proc{mem};
    //proc.dump();
    proc.run();
    proc.dump();


    return 0;
}
