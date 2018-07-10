//
// Created by 金乐盛 on 2018/7/3.
//

#ifndef MIPS_REFLECT_H
#define MIPS_REFLECT_H
#include <map>
#include <string>
#include <iostream>
class transfer{
  private:
    std::map<std::string, int> re;
  public:
    transfer();

    int trans(const std::string &str);
};

enum OPERTOR{
    NOP, ADD, ADDU, ADDIU, SUB, SUBU, MUL, MULU, DIV, DIVU, XOR, XORU, NEG, NEGU, REM, REMU,  LI,
 // 0     1     2     3     4    5     6    7     8     9    10    11   12   13   14     15
    SEQ, SGE, SGT, SLE, SLT, SNE,
 // 16    17   18   19  20    21
    B, BEQ, BNE, BGE, BLE, BGT, BLT, BEQZ, BNEZ, BLEZ, BGEZ, BGTZ, BLTZ, J, JR, JAL, JALR,
  //22  23   24  25   26    27  28   29   30      31     32   33   34    35  36  37  38
    LA, LB, LH, LW,
 // 39  40  41  42
    SB, SH, SW,
 // 43  44  45
    MOVE, MFHI, MFLO,
 //46     47     48
    SYSCALL
 // 49  50
};

transfer::transfer(){
    re["ADD"] = ADD; re["ADDU"] = ADDU; re["ADDIU"] =  ADDIU; re["SUB"] = SUB; re["SUBU"] = SUBU;
    re["MUL"] = MUL; re["MULU"] = MULU; re["DIV"] = DIV; re["DIVU"] = DIVU; re["XOR"] = XOR;
    re["XORU"] = XORU; re["NEG"] = NEG; re["NEGU"] = NEGU; re["REM"] = REM; re["REMU"] = REMU; re["LI"] = LI;
    re["SEQ"] = SEQ; re["SGE"] = SGE; re["SGT"] = SGT; re["SLE"] = SLE; re["SLT"] = SLT; re["SNE"] = SNE; re["B"] = B; re["BEQ"] = BEQ; re["BNE"] = BNE;
    re["BGE"] = BGE; re["BLE"] = BLE; re["BGT"] = BGT;re["BLT"] = BLT; re["BEQZ"] = BEQZ; re["BNEZ"] = BNEZ; re["BLEZ"] = BLEZ;
    re["BGEZ"] = BGEZ;re["BGTZ"] = BGTZ; re["BLTZ"] = BLTZ; re["J"] = J; re["JR"] = JR; re["JAL"] = JAL; re["JALR"] = JALR;
    re["LA"] = LA; re["LB"] = LB; re["LH"] = LH; re["LW"] = LW; re["SB"] = SB; re["SH"] = SH; re["SW"] = SW;
    re["MOVE"] = MOVE; re["MFHI"] = MFHI; re["MFLO"] = MFLO; re["NOP"] = NOP; re["SYSCALL"] = SYSCALL;
    re["$0"] = re["$zero"] = 0;
    re["$1"] = re["$at"] = 1;
    re["$2"] = re["$v0"] = 2; re["$3"] = re["$v1"] = 3;
    re["$4"] = re["$a0"] = 4; re["$5"] = re["$a1"] = 5; re["$6"] = re["$a2"] = 6; re["$7"] = re["$a3"] = 7;
    re["$8"] = re["$t0"] = 8; re["$9"] = re["$t1"] = 9; re["$10"] = re["$t2"] = 10; re["$11"] = re["$t3"] = 11;
    re["$12"] = re["$t4"] = 12; re["$13"] = re["$t5"] = 13; re["$14"] = re["$t6"] = 14; re["$15"] = re["$t7"] = 15;
    re["$16"] = re["$s0"] = 16; re["$17"] = re["$s1"] = 17; re["$18"] = re["$s2"] = 18; re["$19"] = re["$s3"] = 19;
    re["$20"] = re["$s4"] = 20; re["$21"] = re["$s5"] = 21; re["$22"] = re["$s6"] = 22; re["$23"] = re["$s7"] = 23;
    re["$24"] = re["$t8"] = 24; re["$25"] = re["$t9"] = 25; re["$28"] = re["$gp"] = 28; re["$29"] = re["$sp"] = 29;
    re["$30"] = re["$fp"] =30; re["$31"] = re["$ra"] = 31; re["$hi"] = 32;  re["$lo"] = 33; re["$pc"] = 34;
}

int transfer::trans(const std::string &str) {
    //std::cout << re[str] << std::endl;
    return re[str];
}
#endif //MIPS_REFLECT_H
