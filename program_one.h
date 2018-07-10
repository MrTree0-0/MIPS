//
// Created by 金乐盛 on 2018/7/4.
//

#ifndef MIPS_PROGRAM_ONE_H
#define MIPS_PROGRAM_ONE_H
#include "CPU.h"
#include "parser.h"
#include "scanner.h"
#include "transfer.h"
//#include "retranfer.h"
#include "memory.h"
#include <string>
class program{
    struct IFID{
        ScannerToken command;
        int Next_command_line;
    };
    struct IDEX{
        int Next_command_line;
        int valA, valB;
        int imm;
        int op;
        int Rd;
        int Possible_Next_Line;
    };
    struct EXMEM{
        int Next_command_line;
        int result;
        int valB;
        int dest;
        int op;
    };
    struct MEMWB{
        int result;
        int mdata;
        int dest;
        int op;
    };
  private:
    CPU *dep;
    Memory *mem;
    std::vector<ScannerToken> *text;
    int brunch_cnt;
    /*int now_line;
    int op;
    int Re, R1, R2, Imm;
    std::string label;
    int address;
    transfer tran;
    ScannerToken command;*/
    IFID IFID_line;
    IDEX IDEX_line;
    EXMEM EXMEM_line;
    MEMWB MEMWB_line;
    int start;
    //retransfer retran;



  public:
    program(CPU *c, Memory *m, std::vector<ScannerToken> *vec);

    program(program &p);

    void IF();

    void ID();

    void EX();

    void MA();

    void WB();

    void run();
};

program::program(CPU *c, Memory *m, std::vector<ScannerToken> *vec):dep(c), mem(m), text(vec)/*, op(0), Re(0), R1(0), R2(0), address(-1)*/{
    brunch_cnt = 0;
    start = 0;
    IFID_line.Next_command_line = 0;
    IDEX_line.op = EXMEM_line.op = MEMWB_line.op = 0;
    IDEX_line.Next_command_line = IDEX_line.imm = IDEX_line.Rd = IDEX_line.Possible_Next_Line = IDEX_line.valA = IDEX_line.valB = 0;
    EXMEM_line.Next_command_line = EXMEM_line.dest = EXMEM_line.result = EXMEM_line.valB = 0;
    MEMWB_line.result = MEMWB_line.dest = MEMWB_line.mdata = 0;

}

program::program(program &p):dep(p.dep),mem(p.mem), text(p.text)/*, op(p.op), Re(p.Re), R1(p.R1), R2(p.R2)*/{
    brunch_cnt = p.brunch_cnt;
    IFID_line = p.IFID_line;
    IDEX_line = p.IDEX_line;
    EXMEM_line = p.EXMEM_line;
    MEMWB_line = p.MEMWB_line;
}

void program::IF(){
    if(start){
        if(brunch_cnt){
            dep->cpu[34] = EXMEM_line.Next_command_line;
            brunch_cnt = 0;
        }
        else{
            dep->cpu[34]++;
        }
    }
    else{
        start = 1;
    }
    int now_command_line;
    now_command_line = dep->cpu[34];
    IFID_line.command = (*text)[now_command_line];
    IFID_line.Next_command_line = now_command_line + 1;
}

void program::ID() {
    /*op = command.op;
    now_line = command.line_num;
    Re = command.Re;
    R1 = command.R1;
    R2 = command.R2;
    Imm = command.Imm;
    label = command.label;
    address = command.address;*/
    IDEX_line.Next_command_line = IFID_line.Next_command_line;
    IDEX_line.op = IFID_line.command.op;
    //std::cout << IFID_line.Next_command_line - 1 << retran.retrans(IDEX_line.op) << std::endl;
    switch (IDEX_line.op) {
        case ADD:
        case ADDU:
        case ADDIU:
        case SUB:
        case SUBU:
        case XOR:
        case XORU:
        case REM:
        case REMU:
        case SEQ:
        case SGE:
        case SGT:
        case SLE:
        case SLT:
        case SNE:
            IDEX_line.valA = dep->cpu[IFID_line.command.R1];
            IDEX_line.Rd = IFID_line.command.Re;
            if (IFID_line.command.R2 >= 0) {
                IDEX_line.valB = dep->cpu[IFID_line.command.R2];
                IDEX_line.imm = 0;
            } else {
                IDEX_line.imm = IFID_line.command.Imm;
                IDEX_line.valB = 0;
            }
            break;
        case MUL:
        case MULU:
        case DIV:
        case DIVU:
            IDEX_line.Rd = IFID_line.command.Re;//如果目标寄存器是0，那么说明我们要注入hi和lo
            IDEX_line.valA = dep->cpu[IFID_line.command.R1];
            if (IFID_line.command.R2 >= 0) {
                IDEX_line.valB = dep->cpu[IFID_line.command.R2];
                IDEX_line.imm = 0;
            } else {
                IDEX_line.valB = 0;
                IDEX_line.imm = IFID_line.command.Imm;
            }
            break;
        case NEG:
        case NEGU:
            IDEX_line.Rd = IFID_line.command.Re;
            IDEX_line.valA = dep->cpu[IFID_line.command.R1];
            break;
        case LI:
            IDEX_line.Rd = IFID_line.command.Re;
            IDEX_line.imm = IFID_line.command.Imm;
            break;
        case B:
        case J:
            IDEX_line.Next_command_line = IFID_line.command.address;
            break;
        case BEQ:
        case BNE:
        case BGE:
        case BLE:
        case BGT:
        case BLT:
            IDEX_line.valA = dep->cpu[IFID_line.command.R1];
            if (IFID_line.command.R2 >= 0) {
                IDEX_line.valB = dep->cpu[IFID_line.command.R2];
            } else {
                IDEX_line.valB = IFID_line.command.Imm;
            }
            IDEX_line.Possible_Next_Line = IFID_line.command.address;
            break;
        case BEQZ:
        case BNEZ:
        case BLEZ:
        case BGEZ:
        case BLTZ:
        case BGTZ:
            IDEX_line.valA = dep->cpu[IFID_line.command.R1];
            IDEX_line.Possible_Next_Line = IFID_line.command.address;
            break;
        case JR:
            IDEX_line.Next_command_line = dep->cpu[IFID_line.command.R1];
            break;
        case JAL:
            IDEX_line.valB = IFID_line.Next_command_line;
            IDEX_line.Next_command_line = IFID_line.command.address;
            break;
        case JALR:IDEX_line.valB = IFID_line.Next_command_line;
            IDEX_line.Next_command_line = dep->cpu[IFID_line.command.R1];
            break;
        case MOVE:
            IDEX_line.Rd = IFID_line.command.Re;
            IDEX_line.valB = dep->cpu[IFID_line.command.R1];
        //std::cout << IDEX_line.Rd << " " << IDEX_line.valB << std::endl;
            break;
        case MFHI:
            IDEX_line.Rd = IFID_line.command.R1;
            IDEX_line.valB = dep->cpu[32];
            break;
        case MFLO:
            IDEX_line.Rd = IFID_line.command.R1;
            IDEX_line.valB = dep->cpu[33];
            break;
        case NOP:
            break;
        case SYSCALL: {
            IDEX_line.imm = dep->cpu[2];
            switch (IDEX_line.imm) {
                case 1:IDEX_line.valA = dep->cpu[4];
                    break;
                case 4:IDEX_line.valA = dep->cpu[4];
                    break;
                case 5:break;
                case 8:IDEX_line.valA = dep->cpu[4];
                    IDEX_line.valB = dep->cpu[5];
                    break;
                case 9:IDEX_line.valA = dep->cpu[4];
                    break;
                case 10:exit(0);
                case 17:exit(dep->cpu[4]);
            }
            break;
        }
        case LB:
        case LH:
        case LW:
        case LA:
            IDEX_line.Rd = IFID_line.command.R1;
            if (IFID_line.command.address == -1) {
                IDEX_line.valB = dep->cpu[IFID_line.command.R2];
                IDEX_line.imm = IFID_line.command.Imm;
            } else {
                IDEX_line.valB = 0;
                IDEX_line.imm = IFID_line.command.address;
            }
            break;
        case SB:
        case SH:
        case SW:
            IDEX_line.Rd = dep->cpu[IFID_line.command.R1];
            if (IFID_line.command.address == -1) {
                IDEX_line.valB = dep->cpu[IFID_line.command.R2];
                IDEX_line.imm = IFID_line.command.Imm;
            } else {
                IDEX_line.valB = 0;
                IDEX_line.imm = IFID_line.command.address;
            }
            break;
    }

}

void program::EX() {
    EXMEM_line.op = IDEX_line.op;
    EXMEM_line.valB = IDEX_line.valB;
    EXMEM_line.dest = IDEX_line.Rd;
    EXMEM_line.Next_command_line = IDEX_line.Next_command_line;
    switch (EXMEM_line.op) {
        case ADD:EXMEM_line.result = IDEX_line.valA + IDEX_line.valB + IDEX_line.imm;
            break;
        case ADDIU:
        case ADDU:
            EXMEM_line.result =
                (unsigned int) IDEX_line.valA + (unsigned int) IDEX_line.valB + (unsigned int) IDEX_line.imm;
            break;
        case SUB:EXMEM_line.result = IDEX_line.valA - (IDEX_line.valB + IDEX_line.imm);
            break;
        case SUBU:
            EXMEM_line.result =
                (unsigned int) IDEX_line.valA - ((unsigned int) IDEX_line.valB + (unsigned int) IDEX_line.imm);
            break;
        case MUL:
            if (EXMEM_line.dest == 0) {
                long long an;
                if(IDEX_line.valB != 0){
                    an = IDEX_line.valA * IDEX_line.valB;
                    EXMEM_line.result = an & 0xffffffff;
                    EXMEM_line.valB = (an >> 8) & 0xffffffff;
                }
                else{
                    an = IDEX_line.valA * IDEX_line.imm;
                    EXMEM_line.result = an & 0xffffffff;
                    EXMEM_line.valB = (an >> 8) & 0xffffffff;
                }
            } else {
                if(IDEX_line.valB != 0){
                    EXMEM_line.result = IDEX_line.valA * IDEX_line.valB;
                }
                else{
                    EXMEM_line.result = IDEX_line.valA * IDEX_line.imm;
                }

            }
            break;
        case MULU:
            if (EXMEM_line.dest == 0) {
                unsigned long long an;
                if(IDEX_line.valB != 0){
                    an = (unsigned long long) IDEX_line.valA * (unsigned long long) IDEX_line.valB;
                    EXMEM_line.result = an & 0xffffffff;
                    EXMEM_line.valB = (an >> 8) & 0xffffffff;
                }
                else{
                    an = (unsigned long long) IDEX_line.valA * (unsigned long long) IDEX_line.imm;
                    EXMEM_line.result = an & 0xffffffff;
                    EXMEM_line.valB = (an >> 8) & 0xffffffff;
                }
            } else {
                if(IDEX_line.valB != 0){
                    EXMEM_line.result = (unsigned int) IDEX_line.valA * (unsigned int) IDEX_line.valB;
                }
                else{
                    EXMEM_line.result = (unsigned int) IDEX_line.valA * (unsigned int) IDEX_line.imm;
                }

            }
            break;
        case DIV:
        //std::cout << "fuck " <<  IDEX_line.valA << " " << IDEX_line.valB << std::endl;
            EXMEM_line.result = IDEX_line.valA / (IDEX_line.valB + IDEX_line.imm);
            EXMEM_line.valB = IDEX_line.valA % (IDEX_line.valB + IDEX_line.imm);
            break;
        case DIVU:
            EXMEM_line.result = (unsigned int) IDEX_line.valA / (unsigned int) (IDEX_line.valB + IDEX_line.imm);
            EXMEM_line.valB = (unsigned int) IDEX_line.valA % (unsigned int) (IDEX_line.valB + IDEX_line.imm);
            break;
        case XOR:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = IDEX_line.valA ^ IDEX_line.valB;
            } else {
                EXMEM_line.result = IDEX_line.valA ^ IDEX_line.imm;
            }
            break;
        case XORU:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = (unsigned int) IDEX_line.valA ^ (unsigned int) IDEX_line.valB;
            } else {
                EXMEM_line.result = (unsigned int) IDEX_line.valA ^ (unsigned int) IDEX_line.imm;
            }
            break;
        case NEG:EXMEM_line.result = -IDEX_line.valA;
            break;
        case NEGU:EXMEM_line.result = ~IDEX_line.valA;
            break;
        case REM:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = IDEX_line.valA % IDEX_line.valB;
            } else {
                EXMEM_line.result = IDEX_line.valA % IDEX_line.imm;
            }
            break;
        case REMU:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = (unsigned int) IDEX_line.valA % (unsigned int) IDEX_line.valB;
            } else {
                EXMEM_line.result = (unsigned int) IDEX_line.valA % (unsigned int) IDEX_line.imm;
            }
            break;
        case LI:
            EXMEM_line.result = IDEX_line.imm;
            break;
        case SEQ:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = (IDEX_line.valA == IDEX_line.valB);
            } else {
                EXMEM_line.result = (IDEX_line.valA == IDEX_line.imm);
            }
            break;
        case SGE:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = (IDEX_line.valA >= IDEX_line.valB);
            } else {
                EXMEM_line.result = (IDEX_line.valA >= IDEX_line.imm);
            }
            break;
        case SGT:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = (IDEX_line.valA > IDEX_line.valB);
            } else {
                EXMEM_line.result = (IDEX_line.valA > IDEX_line.imm);
            }
            break;
        case SLE:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = (IDEX_line.valA <= IDEX_line.valB);
            } else {
                EXMEM_line.result = (IDEX_line.valA <= IDEX_line.imm);
            }
            break;
        case SLT:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = (IDEX_line.valA < IDEX_line.valB);
            } else {
                EXMEM_line.result = (IDEX_line.valA < IDEX_line.imm);
            }
            break;
        case SNE:
            if (IDEX_line.valB != 0) {
                EXMEM_line.result = (IDEX_line.valA != IDEX_line.valB);
            } else {
                EXMEM_line.result = (IDEX_line.valA != IDEX_line.imm);
            }
            break;
        case B:
        case J:
            brunch_cnt = 1;
            break;
        case BEQ:
            if (IDEX_line.valB != 0) {
                if (IDEX_line.valA == IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            } else {
                if (IDEX_line.valA == IDEX_line.imm) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            }
            break;
        case BNE:
            if (IDEX_line.valB != 0) {
                if (IDEX_line.valA != IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            } else {
                if (IDEX_line.valA != IDEX_line.imm) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            }
            break;
        case BGE:
            if (IDEX_line.valB != 0) {
                if (IDEX_line.valA >= IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            } else {
                if (IDEX_line.valA >= IDEX_line.imm) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            }
            break;
        case BLE: //dep->BLE(R1, R2, Imm, address); break;
            if (IDEX_line.valB != 0) {
                if (IDEX_line.valA <= IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            } else {
                if (IDEX_line.valA <= IDEX_line.imm) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            }
            break;
        case BGT: //dep->BGT(R1, R2, Imm, address); break;
            if (IDEX_line.valB != 0) {
                if (IDEX_line.valA > IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            } else {
                if (IDEX_line.valA > IDEX_line.imm) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            }
            break;
        case BLT: //dep->BLT(R1, R2, Imm, address); break;
            if (IDEX_line.valB != 0) {
                if (IDEX_line.valA < IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            } else {
                if (IDEX_line.valA < IDEX_line.imm) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    brunch_cnt = 1;
                }
            }
            break;
        case BEQZ:
            if (IDEX_line.valA == 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                brunch_cnt = 1;
            }
            break;
        case BNEZ:
            if (IDEX_line.valA != 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                brunch_cnt = 1;
            }
            break;
        case BLEZ:
            if (IDEX_line.valA <= 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                brunch_cnt = 1;
            }
            break;
        case BGEZ:
            if (IDEX_line.valA >= 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                brunch_cnt = 1;
            }
            break;
        case BLTZ:
            if (IDEX_line.valA < 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                brunch_cnt = 1;
            }
            break;
        case BGTZ:
            if (IDEX_line.valA > 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                brunch_cnt = 1;
            }
            break;
        case JR:
            brunch_cnt = 1;
            break;
        case JAL:
        case JALR:
            EXMEM_line.result = IDEX_line.valB;
            brunch_cnt = 1;
            break;
        case LA:
        case LB:
        case LH:
        case LW:
        case SB:
        case SH:
        case SW:
            EXMEM_line.result = IDEX_line.valB + IDEX_line.imm;
            EXMEM_line.dest = IDEX_line.Rd;
            break;
        case MOVE:
        case MFHI:
        case MFLO:
            EXMEM_line.result = IDEX_line.valB;
            break;
        case NOP:break;
        case SYSCALL:
            EXMEM_line.dest = IDEX_line.imm;
            switch (EXMEM_line.dest) {
                case 1:std::cout << IDEX_line.valA;
                    break;
                case 4:EXMEM_line.valB = IDEX_line.valA;
                    break;
                case 5:break;
                case 8:EXMEM_line.result = IDEX_line.valA;//存放的区域在reslut里面
                    break;
                case 9:EXMEM_line.result = IDEX_line.valA;
                    break;
            }
            break;
    }
}


void program::MA(){
    MEMWB_line.result = EXMEM_line.result;
    MEMWB_line.dest = EXMEM_line.dest;
    MEMWB_line.op = EXMEM_line.op;
    switch(MEMWB_line.op){
        case MUL:
        case MULU:
        case DIV:
        case DIVU:
            MEMWB_line.mdata = EXMEM_line.valB;
            break;
        case LB:
            MEMWB_line.mdata = mem->Load(MEMWB_line.result, 1);
            break;
        case LH:
            MEMWB_line.mdata = mem->Load(MEMWB_line.result, 2);
            break;
        case LW:
            MEMWB_line.mdata = mem->Load(MEMWB_line.result, 4);
            break;
        case SB:
            mem->Store_char(MEMWB_line.result, MEMWB_line.dest);
            break;
        case SH:
            mem->Store_halfword(MEMWB_line.result, MEMWB_line.dest);
            break;
        case SW:
            mem->Store_word(MEMWB_line.result, MEMWB_line.dest);
            break;
        case SYSCALL:
            switch(MEMWB_line.dest){
                case 4:
                {
                    int address = EXMEM_line.valB;
                    //std::cout << address << std::endl;
                    while(mem->mem[address] != '\0'){
                        std::cout << (char)mem->mem[address++];
                    }
                    break;
                }
                case 8:
                {
                    int len = EXMEM_line.valB;
                    int address = EXMEM_line.result;
                    std::string str;
                    std::cin >> str;
                    for(int i = 0; i < len; i++){
                        mem->mem[address + i] = str[i];
                    }
                    mem->mem[address + len] = '\0';
                    break;
                }
                case 9:{
                    int len = EXMEM_line.result;
                    MEMWB_line.mdata = mem->data_point;
                    mem->data_point += len;
                    break;
                }
            }
            break;

    }
}

void program::WB(){
    switch(MEMWB_line.op){
        case ADD:
        case SUB:
        case ADDIU:
        case SUBU:
        case ADDU:
            dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
            break;
        case MUL:
        case MULU:
            if(MEMWB_line.dest == 0){
                dep->cpu[33] = MEMWB_line.result;
                dep->cpu[32] = MEMWB_line.mdata;
            }
            else{
                dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
            }
            break;
        case DIV:
        case DIVU:
            if(MEMWB_line.dest == 0){
                //std::cout << "shit" << " " << MEMWB_line.mdata << " " << MEMWB_line.result << std::endl;
                dep->cpu[33] = MEMWB_line.result;
                dep->cpu[32] = MEMWB_line.mdata;
            }
            else{
                dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
            }
            break;
        case XOR:
        case XORU:
        case NEG:
        case NEGU:
        case REM:
        case REMU:
        case SEQ:
        case SGE:
        case SGT:
        case SLE:
        case SLT:
        case SNE:
        case LI:
        case LA:
        //std::cout << MEMWB_line.result << std::endl;
            dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
            break;
        case J:
        case B:
        case BEQ:
        case BNE:
        case BGE:
        case BLE:
        case BGT:
        case BLT:
        case BEQZ:
        case BNEZ:
        case BLEZ:
        case BGEZ:
        case BLTZ:
        case BGTZ:
        case JR:
            break;
        case JAL:
            dep->cpu[31] = MEMWB_line.result;
            break;
        case JALR:
            dep->cpu[31] = MEMWB_line.result;
            break;
        case LB:
        case LH:
        case LW:
        //std::cout << MEMWB_line.mdata << std::endl;
            dep->cpu[MEMWB_line.dest] = MEMWB_line.mdata;
            break;
        case SB:
        case SH:
        case SW:
            break;
        case MOVE:
        case MFHI:
        case MFLO:
            dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
            break;
        case NOP:
            break;
        case SYSCALL:
            EXMEM_line.dest = IDEX_line.imm;
            switch (IDEX_line.imm) {
                case 5:{
                    int i;
                    std::cin >> i;
                    dep->cpu[2] = i;
                    break;
                }
                case 9:
                    dep->cpu[2] = MEMWB_line.mdata;
                    break;
            }
            break;
    }
}

void program::run(){
    int cnt = 0;
    while(true){
        cnt++;
        /*WB();
        MA();
        EX();
        ID();
        IF();*/
        //if(cnt > 500)break;
        IF();
        ID();
        EX();
        MA();
        WB();
    }
}

#endif //MIPS_PROGRAM_ONE_H
