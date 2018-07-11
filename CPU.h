//
// Created by 金乐盛 on 2018/7/3.
//

#ifndef MIPS_CPU_H
#define MIPS_CPU_H
#include "parser.h"
#include "transfer.h"
class CPU{
    friend class program;
  private:
    int cpu[35] = {0};
    int used[35] = {0};
    transfer trans;

  public:
    void lock_reg(int n){
        used[n]++;
    }

    void unlock_reg(int n){
        used[n]--;
    }

    bool lock_or_not(int n){
        return used[n];
    }

    inline void ADD(int Re, int R1, int R2, int Imm);

    inline void ADDU(int Re, int R1, int R2, int Imm);

    inline void ADDIU(int Re, int R1, int R2, int Imm);

    inline void SUB(int Re, int R1, int R2, int Imm);

    inline void SUBU(int Re, int R1, int R2, int Imm);

    inline void MUL(int Re, int R1, int R2, int Imm);

    inline void MULU(int Re, int R1, int R2, int Imm);

    inline void MUL(int R1, int R2, int Imm);

    inline void MULU(int R1, int R2, int Imm);

    inline void DIV(int Re, int R1, int R2, int Imm);

    inline void DIVU(int Re, int R1, int R2, int Imm);

    inline void DIV(int R1, int R2, int Imm);

    inline void DIVU(int R1, int R2, int Imm);

    inline void XOR(int Re, int R1, int R2, int Imm);

    inline void XORU(int Re, int R1, int R2, int Imm);

    inline void NEG(int Re, int R1);

    inline void NEGU(int Re, int R1);

    inline void REM(int Re, int R1, int R2, int Imm);

    inline void REMU(int Re,int R1, int R2, int Imm);

    inline  void LI(int Re, int Imm);

    inline void SEQ(int Re, int R1, int R2, int Imm);

    inline void SGE(int Re, int R1, int R2, int Imm);

    inline void SGT(int Re, int R1, int R2, int Imm);

    inline void SLE(int Re, int R1, int R2, int Imm);

    inline void SLT(int Re, int R1, int R2, int Imm);

    inline void SNE(int Re, int R1, int R2, int Imm);

    inline void B(int Re);

    inline void BEQ(int R1, int R2, int Imm, int Re);

    inline void BNE(int R1, int R2, int Imm, int Re);

    inline void BGE(int R1, int R2, int Imm, int Re);

    inline void BLE(int R1, int R2, int Imm, int Re);

    inline void BGT(int R1, int R2, int Imm, int Re);

    inline void BLT(int R1, int R2, int Imm, int Re);

    inline void BEQZ(int R1, int Re);

    inline void BNEZ(int R1, int Re);

    inline void BLEZ(int R1, int Re);

    inline void BGEZ(int R1, int Re);

    inline void BGTZ(int R1, int Re);

    inline void BLTZ(int R1, int Re);

    inline void J(int Re);

    inline void Jr(int R1);

    inline void JAL(int Re);

    inline void JALR(int R1);

    inline void LA(int Re, int address);

    /*void LB(int R1, int R2, int Imm, int Re);

    void LH(int R1, int R2, int Imm, int Re);

    void LW(int R1, int R2, int Imm, int Re);

    void SB(int R1, int R2, int Imm, int Re);

    void SH(int R1, int R2, int Imm, int Re);

    void SW(int R1, int R2, int Imm, int Re);*/

    inline void MOVE(int Re, int R1);

    inline void MFHI(int R1);

    inline void MFLO(int R1);

    void NOP();

    int SYSCALL();

    int getcommand();

    inline void get_start(int sp, int mainnum);

    inline void Load_char(int Re, int num);

    inline void Load_halfword(int Re, int num);

    inline void Load_word(int Re, int num);

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
    return cpu[34]++;
}

void CPU::get_start(int sp, int mainnum){
    cpu[29] = sp;
    cpu[34] = mainnum;
}

void CPU::ADD(int Re, int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[Re] = cpu[R1] + cpu[R2];
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
    cpu[33] = an & 0xffffffff;
    cpu[32] = (an >> 8) & 0xffffffff;
}

void CPU::MULU(int R1, int R2, int Imm) {
    unsigned long long an;
    if(R2 >= 0){
        an = (unsigned long long)cpu[R1] * (unsigned long long)cpu[R2];
    }
    else{
        an = (unsigned long long)cpu[R1] * (unsigned long long)Imm;
    }
    cpu[33] = an & 0xffffffff;
    cpu[32] = (an >> 8) & 0xffffffff;
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
        std::cout << "fuck " << cpu[R1] << " " << cpu[R2] << std::endl;
        cpu[32] = cpu[R1] % cpu[R2];
        cpu[33] = cpu[R1] / cpu[R2];
        std::cout << "shit" << cpu[32] << " " << cpu[33] << std::endl;
    }
    else{
        cpu[32] = cpu[R1] % Imm;
        cpu[33] = cpu[R1] / Imm;
    }
};

void CPU::DIVU(int R1, int R2, int Imm){
    if(R2 >= 0){
        cpu[32] = (unsigned int)cpu[R1] % (unsigned int)cpu[R2];
        cpu[33] = (unsigned int)cpu[R1] / (unsigned int)cpu[R2];
    }
    else{
        cpu[32] = (unsigned int)cpu[R1] % (unsigned int)Imm;
        cpu[33] = (unsigned int)cpu[R1] / (unsigned int)Imm;
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
    cpu[34] = Re;//b和j毛区别？
};

void CPU::BEQ(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] == cpu[R2]){
            cpu[34] = Re;
        }
    }
    else{
        if(cpu[R1] == Imm){
            cpu[34] = Re;
        }
    }
};

void CPU::BNE(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] != cpu[R2]){
            cpu[34] = Re;
        }
    }
    else{
        if(cpu[R1] != Imm){
            cpu[34] = Re;
        }
    }
};

void CPU::BGE(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] >= cpu[R2]){
            cpu[34] = Re;
        }
    }
    else{
        if(cpu[R1] >= Imm){
            cpu[34] = Re;
        }
    }
};

void CPU::BLE(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] <= cpu[R2]){
            cpu[34] = Re;
        }
    }
    else{
        if(cpu[R1] <= Imm){
            cpu[34] = Re;
        }
    }
};

void CPU::BGT(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] > cpu[R2]){
            cpu[34] = Re;
        }
    }
    else{
        if(cpu[R1] > Imm){
            cpu[34] = Re;
        }
    }
};

void CPU::BLT(int R1, int R2, int Imm, int Re){
    if(R2 >= 0){
        if(cpu[R1] < cpu[R2]){
            cpu[34] = Re;
        }
    }
    else{
        if(cpu[R1] < Imm){
            cpu[34] = Re;
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
    cpu[34] = cpu[R1];
};

void CPU::JAL(int Re){
    cpu[31] = cpu[34];
    cpu[34] = Re;
};

void CPU::JALR(int R1){
    cpu[31] = cpu[34];
    cpu[34] = cpu[R1];
};

void CPU::LA(int Re, int address){
    cpu[Re] = address;
};

void CPU::MOVE(int Re, int R1){
    cpu[Re] = cpu[R1];
};

void CPU::MFHI(int R1){
    cpu[R1] = cpu[32];
};

void CPU::MFLO(int R1){
    cpu[R1] = cpu[33];
};

void CPU::NOP(){

};

int CPU::SYSCALL(){
    int command = cpu[2];
    switch (command){
        case 1:{int i; i = cpu[4];std::cout << i; break;}
        case 4: return 4;
        case 5: {int i; std::cin >> i; cpu[2] = i;break;}
        case 8:return 8;
        case 9:return 9;
        case 10:exit(0);
        case 17:exit(cpu[4]);

    }
    return -1;
};

void CPU::Load_char(int Re, int num) {
    cpu[Re] = num;
}

void CPU::Load_halfword(int Re, int num) {
    cpu[Re] = num;
}

void CPU::Load_word(int Re, int num) {
    cpu[Re] = num;
}

int CPU::get_value(int Re) {
    return cpu[Re];
}

int CPU::get_value(std::string str) {
    return cpu[trans.trans(str)];
}


#endif //MIPS_CPU_H
