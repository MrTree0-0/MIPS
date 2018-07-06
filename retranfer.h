//
// Created by 金乐盛 on 2018/7/6.
//

#ifndef MIPS_RETRANFER_H
#define MIPS_RETRANFER_H
#include <map>
#include <string>
#include <iostream>
#include "transfer.h"
class retransfer{
  private:
    std::map<int, std::string> re;
  public:
    retransfer();

    std::string retrans(int i);
};


retransfer::retransfer(){
    re[ADD] = "ADD"; re[ADDU] = "ADDU"; re[ADDIU] =  "ADDIU"; re[SUB] = "SUB"; re[SUBU] = "SUBU";
    re[MUL] = "MUL"; re[MULU] = "MULU"; re[DIV] = "DIV"; re[DIVU] = "DIVU"; re[XOR] = "XOR";
    re[XORU] = "XORU"; re[NEG] = "NEG"; re[NEGU] = "NEGU"; re[REM] = "REM"; re[REMU] = "REMU"; re[LI] = "LI";
    re[SEQ] = "SEQ"; re[SGE] = "SGE"; re[SGT] = "SGT"; re[SLE] = "SLE"; re[SLT] = "SLT"; re[SNE] = "SNE"; re[B] = "B"; re[BEQ] = "BEQ"; re[BNE] = "BNE";
    re[BGE] = "BGE"; re[BLE] = "BLE"; re[BGT] = "BGT";re[BLT] = "BLT"; re[BEQZ] = "BEQZ"; re[BNEZ] = "BNEZ"; re[BLEZ] = "BLEZ";
    re[BGEZ] = "BGEZ";re[BGTZ] = "BGTZ"; re[BLTZ] = "BLTZ"; re[J] = "J"; re[JR] = "JR"; re[JAL] = "JAL"; re[JALR] = "JALR";
    re[LA] = "LA"; re[LB] = "LB"; re[LH] = "LH"; re[LW] = "LW"; re[SB] = "SB"; re[SH] = "SH"; re[SW] = "SW";
    re[MOVE] = "MOVE"; re[MFHI] = "MFHI"; re[MFLO] = "MFLO"; re[NOP] = "NOP"; re[SYSCALL] = "SYSCALL";
}

std::string retransfer::retrans(int i) {
    //std::cout << re[str] << std::endl;
    return re[i];
}
#endif //MIPS_RETRANFER_H
