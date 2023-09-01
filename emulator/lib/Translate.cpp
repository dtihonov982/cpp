#include "Translate.h"

namespace tr {
const std::unordered_map<cmd::Code, DecodingHandler> Decoder::decs {
      {cmd::END,          End_dec}
    , {cmd::ADD_RR,       AddRR_dec}
    , {cmd::ADD_RI,       AddRI_dec}
    , {cmd::MOV_RR,       MovRR_dec}
    , {cmd::MOV_RRm,      MovRRm_dec}
    , {cmd::MOV_RI,       MovRI_dec}
    , {cmd::MOV_RM,       MovRM_dec}
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

} // namespace tr
