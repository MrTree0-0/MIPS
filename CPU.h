//
// Created by 金乐盛 on 2018/7/3.
//

#ifndef MIPS_CPU_H
#define MIPS_CPU_H
#include "parser.h"
#include "transfer.h"
class CPU{
  private:
    int cpu[35];
    transfer trans;

  public:
    void ADD(int Re, int R1, int R2, int Imm);

    void ADDU(int Re, int R1, int R2, int Imm);

    void ADDIU(int Re, int R1, int R2, int Imm);

    void SUB(int Re, int R1, int R2, int Imm);

    void SUBU(int Re, int R1, int R2, int Imm);

    void MUL(int Re, int R1, int R2, int Imm);

    void MULU(int Re, int R1, int R2, int Imm);

    void MUL(int R1, int R2, int Imm);

    void MULU(int R1, int R2, int Imm);

    void DIV(int Re, int R1, int R2, int Imm);

    void DIVU(int Re, int R1, int R2, int Imm);

    void DIV(int R1, int R2, int Imm);

    void DIVU(int R1, int R2, int Imm);

    void XOR(int Re, int R1, int R2, int Imm);

    void XORU(int Re, int R1, int R2, int Imm);

    void NEG(int Re, int R1);

    void NEGU(int Re, int R1);

    void REM(int Re, int R1, int R2, int Imm);

    void REMU(int Re,int R1, int R2, int Imm);

    void LI(int Re, int Imm);

    void SEQ(int Re, int R1, int R2, int Imm);

    void SGE(int Re, int R1, int R2, int Imm);

    void SGT(int Re, int R1, int R2, int Imm);

    void SLE(int Re, int R1, int R2, int Imm);

    void SLT(int Re, int R1, int R2, int Imm);

    void SNE(int Re, int R1, int R2, int Imm);

    void B(int Re);

    void BEQ(int R1, int R2, int Imm, int Re);

    void BNE(int R1, int R2, int Imm, int Re);

    void BGE(int R1, int R2, int Imm, int Re);

    void BLE(int R1, int R2, int Imm, int Re);

    void BGT(int R1, int R2, int Imm, int Re);

    void BLT(int R1, int R2, int Imm, int Re);

    void BEQZ(int R1, int Re);

    void BNEZ(int R1, int Re);

    void BLEZ(int R1, int Re);

    void BGEZ(int R1, int Re);

    void BGTZ(int R1, int Re);

    void BLTZ(int R1, int Re);

    void J(int Re);

    void Jr(int R1);

    void JAL(int Re);

    void JALR(int R1);

    void LA(int Re, int address);

    /*void LB(int R1, int R2, int Imm, int Re);

    void LH(int R1, int R2, int Imm, int Re);

    void LW(int R1, int R2, int Imm, int Re);

    void SB(int R1, int R2, int Imm, int Re);

    void SH(int R1, int R2, int Imm, int Re);

    void SW(int R1, int R2, int Imm, int Re);*/

    void MOVE(int Re, int R1);

    void MFHI(int R1);

    void MFLO(int R1);

    void NOP();

    int SYSCALL();

    int getcommand();

    void get_start(int sp, int mainnum);

    void Load_char(int Re, int num);

    void Load_halfword(int Re, int num);

    void Load_word(int Re, int num);

    int get_value(int Re);

    int get_value(std::string str);

    void print(){
        for(int i = 0; i <= 31; i++){
            std::cout << "R" << i << '\t' << cpu[i] << '\t';
            if(i % 5 == 0) std::cout << std::endl;
        }
        std::cout << "hi" << '\t' << cpu[32] << std::endl;
        std::cout << "lo" << '\t' << cpu[33] << std::endl;
    }
};

int CPU::getcommand(){
    return cpu[trans.trans("$pc")]++;
}

void CPU::get_start(int sp, int mainnum){
    cpu[trans.trans("$sp")] = sp;
    cpu[trans.trans("$pc")] = mainnum;
}

void CPU::ADD(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = cpu[R1] + cpu[R2];
        //std::cout << cpu[Re] << std::endl;
    }
    else{
        cpu[Re] = cpu[R1] + Imm;
    }
}

void CPU::ADDU(int Re, int R1, int R2, int Imm){
    ADD(Re, R1, R2, Imm);
}

void CPU::ADDIU(int Re, int R1, int R2, int Imm) {
    ADD(Re, R1, R2, Imm);
}

void CPU::SUB(int Re, int R1, int R2, int Imm) {
    if(R2 >= 0){
        cpu[Re] = cpu[R1] - cpu[R2];
    }
    else{
        cpu[Re] = cpu[R1] - Imm;
    }
}

void CPU::SUBU(int Re, int R1, int R2, int Imm) {
    SUB(Re, R1, R2, Imm);
}

void CPU::MUL(int Re, int R1, int R2, int Imm) {
    if(R2 >= 0){
        cpu[Re] = cpu[R1] * cpu[R2];
    }
    else{
        cpu[Re] = cpu[R1] * Imm;
    }
}

void CPU::MULU(int Re, int R1, int R2, int Imm) {
    if(R2 >= 0){
        cpu[Re] = (unsigned int)cpu[R1] * (unsigned int)cpu[R2];
    }
    else{
        cpu[Re] = (unsigned int)cpu[R1] * (unsigned int)Imm;
    }
}

void CPU::MUL(int R1, int R2, int Imm) {
    long long an;
    if(R2 >= 0){
        an = (long long)cpu[R1] * (long long)cpu[R2];
    }
    else{
        an = (long long)cpu[R1] * (long long)Imm;
    }
    cpu[trans.trans("$lo")] = an & 0xffffffff;
    cpu[trans.trans("$hi")] = (an >> 8) & 0xffffffff;
}

void CPU::MULU(int R1, int R2, int Imm) {
    unsigned long long an;
    if(R2 >= 0){
        an = (unsigned long long)cpu[R1] * (unsigned long long)cpu[R2];
    }
    else{
        an = (unsigned long long)cpu[R1] * (unsigned long long)Imm;
    }
    cpu[trans.trans("$lo")] = an & 0xffffffff;
    cpu[trans.trans("$hi")] = (an >> 8) & 0xffffffff;
}

void CPU::DIV(int Re, int R1, int R2, int Imm) {
    if(R2 >= 0){
        cpu[Re] = cpu[R1] / cpu[R2];
    }
    else{
        cpu[Re] = cpu[R1] / Imm;
    }
}

void CPU::DIVU(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = (unsigned int)cpu[R1] / (unsigned int)cpu[R2];
    }
    else{
        cpu[Re] = (unsigned int)cpu[R1] / (unsigned int)Imm;
    }
};

void CPU::DIV(int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[trans.trans("$hi")] = cpu[R1] % cpu[R2];
        cpu[trans.trans("$lo")] = cpu[R1] / cpu[R2];
    }
    else{
        cpu[trans.trans("$hi")] = cpu[R1] % Imm;
        cpu[trans.trans("$lo")] = cpu[R1] / Imm;
    }
};

void CPU::DIVU(int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[trans.trans("$hi")] = (unsigned int)cpu[R1] % (unsigned int)cpu[R2];
        cpu[trans.trans("$lo")] = (unsigned int)cpu[R1] / (unsigned int)cpu[R2];
    }
    else{
        cpu[trans.trans("$hi")] = (unsigned int)cpu[R1] % (unsigned int)Imm;
        cpu[trans.trans("$lo")] = (unsigned int)cpu[R1] / (unsigned int)Imm;
    }
};

void CPU::XOR(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = cpu[R1] ^ cpu[R2];
    }
    else{
        cpu[Re] = cpu[R1] ^ Imm;
    }
};

void CPU::XORU(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = (unsigned int)cpu[R1] ^ (unsigned int)cpu[R2];
    }
    else{
        cpu[Re] = (unsigned int)cpu[R1] ^ (unsigned int)Imm;
    }
};

void CPU::NEG(int Re, int R1){
    cpu[Re] = -cpu[R1];
};

void CPU::NEGU(int Re, int R1){
    cpu[Re] = ~cpu[R1];
};

void CPU::REM(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = cpu[R1] % cpu[R2];
    }
    else{
        cpu[Re] = cpu[R1] % Imm;
    }
};

void CPU::REMU(int Re,int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = (unsigned int)cpu[R1] % (unsigned int)cpu[R2];
    }
    else{
        cpu[Re] = (unsigned int)cpu[R1] % (unsigned int)Imm;
    }
};

void CPU::LI(int Re, int Imm){
    cpu[Re] = Imm;
};

void CPU::SEQ(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = (cpu[R1] == cpu[R2]);
    }
    else{
        cpu[Re] = (cpu[R1] == Imm);
    }
};

void CPU::SGE(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = (cpu[R1] >= cpu[R2]);
    }
    else{
        cpu[Re] = (cpu[R1] >= Imm);
    }
};

void CPU::SGT(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = (cpu[R1] > cpu[R2]);
    }
    else{
        cpu[Re] = (cpu[R1] > Imm);
    }
};

void CPU::SLE(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = (cpu[R1] <= cpu[R2]);
    }
    else{
        cpu[Re] = (cpu[R1] <= Imm);
    }
};

void CPU::SLT(int Re, int R1, int R2, int Imm) {
    if(R2 >= 0){
        cpu[Re] = (cpu[R1] < cpu[R2]);
    }
    else{
        cpu[Re] = (cpu[R1] < Imm);
    }
}

void CPU::SNE(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = (cpu[R1] != cpu[R2]);
    }
    else{
        cpu[Re] = (cpu[R1] != Imm);
    }
};

void CPU::B(int Re){
    cpu[trans.trans("$pc")] = Re;//b和j毛区别？
};

void CPU::BEQ(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] == cpu[R2]){
            cpu[trans.trans("$pc")] = Re;
        }
    }
    else{
        if(cpu[R1] == Imm){
            cpu[trans.trans("$pc")] = Re;
        }
    }
};

void CPU::BNE(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] != cpu[R2]){
            cpu[trans.trans("$pc")] = Re;
        }
    }
    else{
        if(cpu[R1] != Imm){
            cpu[trans.trans("$pc")] = Re;
        }
    }
};

void CPU::BGE(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] >= cpu[R2]){
            cpu[trans.trans("$pc")] = Re;
        }
    }
    else{
        if(cpu[R1] >= Imm){
            cpu[trans.trans("$pc")] = Re;
        }
    }
};

void CPU::BLE(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] <= cpu[R2]){
            cpu[trans.trans("$pc")] = Re;
        }
    }
    else{
        if(cpu[R1] <= Imm){
            cpu[trans.trans("$pc")] = Re;
        }
    }
};

void CPU::BGT(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] > cpu[R2]){
            cpu[trans.trans("$pc")] = Re;
        }
    }
    else{
        if(cpu[R1] > Imm){
            cpu[trans.trans("$pc")] = Re;
        }
    }
};

void CPU::BLT(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] < cpu[R2]){
            cpu[trans.trans("$pc")] = Re;
        }
    }
    else{
        if(cpu[R1] < Imm){
            cpu[trans.trans("$pc")] = Re;
        }
    }
};

void CPU::BEQZ(int R1, int Re){
    BEQ(R1, -1, 0, Re);
};

void CPU::BNEZ(int R1, int Re){
    BNE(R1, -1, 0, Re);
};

void CPU::BLEZ(int R1, int Re){
    BLE(R1, -1, 0, Re);
};

void CPU::BGEZ(int R1, int Re){
    BGE(R1, -1, 0, Re);
};

void CPU::BGTZ(int R1, int Re){
    BGT(R1, -1, 0, Re);
};

void CPU::BLTZ(int R1, int Re){
    BLT(R1, -1, 0, Re);
};

void CPU::J(int Re){
    B(Re);
};

void CPU::Jr(int R1){
    cpu[trans.trans("$pc")] = cpu[R1];
};

void CPU::JAL(int Re){
    cpu[trans.trans("$31")] = cpu[trans.trans("$pc")];
    cpu[trans.trans("$pc")] = Re;
};

void CPU::JALR(int R1){
    cpu[trans.trans("$31")] = cpu[trans.trans("$pc")];
    cpu[trans.trans("$pc")] = cpu[R1];
};

void CPU::LA(int Re, int address){
    cpu[Re] = address;
};

/*void CPU::LB(int R1, int R2, int Imm, int Re);

void CPU::LH(int R1, int R2, int Imm, int Re);

void CPU::LW(int R1, int R2, int Imm, int Re);

void CPU::SB(int R1, int R2, int Imm, int Re);

void CPU::SH(int R1, int R2, int Imm, int Re);

void CPU::SW(int R1, int R2, int Imm, int Re);*/

void CPU::MOVE(int Re, int R1){
    cpu[Re] = cpu[R1];
};

void CPU::MFHI(int R1){
    cpu[R1] = cpu[trans.trans("$hi")];
};

void CPU::MFLO(int R1){
    cpu[R1] = cpu[trans.trans("$lo")];
};

void CPU::NOP(){

};

int CPU::SYSCALL(){
    int command = cpu[trans.trans("$v0")];
    switch (command){
        case 1:{int i; i = cpu[trans.trans("$a0")];std::cout << i; break;}
        case 4: return 4;
        case 5: {int i; std::cin >> i; cpu[trans.trans("$v0")] = i;break;}
        case 8:return 8;
        case 9:return 9;
        case 10:exit(0);
        case 17:exit(cpu[trans.trans("$a0")]);

    }
    return -1;
};

void CPU::Load_char(int Re, int num) {
    //cpu[Re] = (int)ch;
    cpu[Re] = num;
}

void CPU::Load_halfword(int Re, int num) {
    //cpu[Re] = (int)sh;
    //memcpy(cpu + Re, &sh, sizeof(unsigned short));
    cpu[Re] = num;
}

void CPU::Load_word(int Re, int num) {
    //cpu[Re] = (int)word;
    //memcpy(cpu + Re, &word, sizeof(unsigned int));
    cpu[Re] = num;
}

int CPU::get_value(int Re) {
    return cpu[Re];
}

int CPU::get_value(std::string str) {
    return cpu[trans.trans(str)];
}


#endif //MIPS_CPU_H
