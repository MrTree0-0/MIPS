//
// Created by 金乐盛 on 2018/7/4.
//

#ifndef MIPS_PROGRAM_ONE_H
#define MIPS_PROGRAM_ONE_H
#define CAL
#include "CPU.h"
#include "parser.h"
#include "scanner.h"
#include "transfer.h"
#include "retranfer.h"
#include "memory.h"
#include "autobranch.h"
#include <string>
class program{
    friend class CPU;
    enum STATE{
        SNT, WNT, WT, ST
    };
    struct IFID{
        int stop_or_not;
        ScannerToken command;
        int Next_command_line;
    };
    struct IDEX{
        int stop_or_not;
        int Next_command_line;
        int valA, valB;
        int imm;
        int op;
        int Rd;
        int Possible_Next_Line;
    };
    struct EXMEM{
        int stop_or_not;
        int Next_command_line;
        int result;
        int valB;
        int dest;
        int op;
    };
    struct MEMWB{
        int stop_or_not;
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
    //int stop_read;
    IFID IFID_line;
    IDEX IDEX_line;
    EXMEM EXMEM_line;
    MEMWB MEMWB_line;
    //int start;
    //int statement;
    retransfer retran;
    autobranch statement;
#ifndef CAL
    int correct, incorrect;
#endif


  public:
    program(CPU *c, Memory *m, std::vector<ScannerToken> *vec);

    program(program &p);

    int IF();

    int ID();

    int EX();

    int MA();

    void WB();

    void run();

    /*int lower_statement(){
        if(statement <=3 && statement > 0){
            statement--;
        }
        if(statement == 1 || statement == 2) {
#ifndef CAL
            incorrect++;
#endif
            return 1;
        }
#ifndef CAL
        correct++;
#endif
        return 0;
    }

    int upper_statement(){
        if(statement <3 && statement >= 0){
            statement++;
        }
        if(statement == 2 || statement == 1) {
#ifndef CAL
            incorrect++;
#endif
            return 1;
        }
#ifndef CAL
        correct++;
#endif
        return 0;
    }*/
};

program::program(CPU *c, Memory *m, std::vector<ScannerToken> *vec):dep(c), mem(m), text(vec)/*, op(0), Re(0), R1(0), R2(0), address(-1)*/{
    //statement = WT;
    brunch_cnt = 0;
    //stop_read = 0;
    IFID_line.Next_command_line = 0;
    IDEX_line.op = EXMEM_line.op = MEMWB_line.op = 0;
    IDEX_line.Next_command_line = IDEX_line.imm = IDEX_line.Rd = IDEX_line.Possible_Next_Line = IDEX_line.valA = IDEX_line.valB = 0;
    EXMEM_line.Next_command_line = EXMEM_line.dest = EXMEM_line.result = EXMEM_line.valB = 0;
    MEMWB_line.result = MEMWB_line.dest = MEMWB_line.mdata = 0;
    IFID_line.stop_or_not = IDEX_line.stop_or_not = EXMEM_line.stop_or_not = MEMWB_line.stop_or_not = 1;
}

program::program(program &p):dep(p.dep),mem(p.mem), text(p.text)/*, op(p.op), Re(p.Re), R1(p.R1), R2(p.R2)*/{
    brunch_cnt = p.brunch_cnt;
    IFID_line = p.IFID_line;
    IDEX_line = p.IDEX_line;
    EXMEM_line = p.EXMEM_line;
    MEMWB_line = p.MEMWB_line;
}

int program::IF(){
    int now_command_line;
    if(dep->lock_or_not(34)) return 1;
    now_command_line = dep->cpu[34]++;
              //std::cout << now_command_line << " shit" << std::endl;
    IFID_line.command = (*text)[now_command_line];
    //std::cout << IFID_line.command.line_num << "IF: " << retran.retrans(IFID_line.command.op) << std::endl;
    IFID_line.Next_command_line = now_command_line + 1;
    return 0;
}

int program::ID() {
    IDEX_line.Next_command_line = IFID_line.Next_command_line;
    IDEX_line.op = IFID_line.command.op;
    //std::cout << "ID: " << retran.retrans(IDEX_line.op) << std::endl;
    //std::cout << retran.retrans(IDEX_line.op) << std::endl;
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
            if(dep->lock_or_not(IFID_line.command.R1) || ((IFID_line.command.R2 >= 0) && dep->lock_or_not(IFID_line.command.R2))) return 1;
            IDEX_line.valA = dep->cpu[IFID_line.command.R1];
            IDEX_line.Rd = IFID_line.command.Re;
            dep->lock_reg(IDEX_line.Rd);
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
            if(dep->lock_or_not(IFID_line.command.R1) || ((IFID_line.command.R2 >= 0) && dep->lock_or_not(IFID_line.command.R2))) return 1;
            IDEX_line.Rd = IFID_line.command.Re;//如果目标寄存器是0，那么说明我们要注入hi和lo
            if(IDEX_line.Rd == 0){
                dep->lock_reg(32);
                dep->lock_reg(33);
            }
            else{
                dep->lock_reg(IDEX_line.Rd);
            }
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
            if(dep->lock_or_not(IFID_line.command.R1) || ((IFID_line.command.R2 >= 0) && dep->lock_or_not(IFID_line.command.R2))) return 1;
            IDEX_line.Rd = IFID_line.command.Re;
            dep->lock_reg(IDEX_line.Rd);
            IDEX_line.valA = dep->cpu[IFID_line.command.R1];
            break;
        case LI:
            IDEX_line.Rd = IFID_line.command.Re;
            dep->lock_reg(IDEX_line.Rd);
            IDEX_line.imm = IFID_line.command.Imm;
            break;
        case B:
        case J:
            IDEX_line.Next_command_line = IFID_line.command.address;
            //dep->lock_reg(34);
            break;
        case BEQ:
        case BNE:
        case BGE:
        case BLE:
        case BGT:
        case BLT:
            if(dep->lock_or_not(IFID_line.command.R1) || ((IFID_line.command.R2 >= 0) && dep->lock_or_not(IFID_line.command.R2))) return 1;
            IDEX_line.valA = dep->cpu[IFID_line.command.R1];
            if (IFID_line.command.R2 >= 0) {
                IDEX_line.valB = dep->cpu[IFID_line.command.R2];
            } else {
                IDEX_line.valB = IFID_line.command.Imm;
            }
            IDEX_line.Possible_Next_Line = IFID_line.command.address;
            if(statement.get_predict() == WT || statement.get_predict() == ST)dep->lock_reg(34);
            break;
        case BEQZ:
        case BNEZ:
        case BLEZ:
        case BGEZ:
        case BLTZ:
        case BGTZ:
            if(dep->lock_or_not(IFID_line.command.R1)) return 1;
            IDEX_line.valA = dep->cpu[IFID_line.command.R1];
            IDEX_line.Possible_Next_Line = IFID_line.command.address;
            if(statement.get_predict() == WT || statement.get_predict() == ST)dep->lock_reg(34);
            break;
        case JR:
            if(dep->lock_or_not(IFID_line.command.R1)) return 1;
            IDEX_line.Next_command_line = dep->cpu[IFID_line.command.R1];
            //dep->lock_reg(34);
            break;
        case JAL:
            IDEX_line.valB = IFID_line.Next_command_line;
            IDEX_line.Next_command_line = IFID_line.command.address;
            dep->lock_reg(31);
            //dep->lock_reg(34);
            break;
        case JALR:
            if(dep->lock_or_not(IFID_line.command.R1)) return 1;
            IDEX_line.valB = IFID_line.Next_command_line;
            IDEX_line.Next_command_line = dep->cpu[IFID_line.command.R1];
            dep->lock_reg(31);
            //dep->lock_reg(34);
            break;
        case MOVE:
            if(dep->lock_or_not(IFID_line.command.R1)) return 1;
            IDEX_line.Rd = IFID_line.command.Re;
            IDEX_line.valB = dep->cpu[IFID_line.command.R1];
            dep->lock_reg(IDEX_line.Rd);
            break;
        case MFHI:
            if(dep->lock_or_not(32)) return 1;
            IDEX_line.Rd = IFID_line.command.R1;
            IDEX_line.valB = dep->cpu[32];
            dep->lock_reg(IDEX_line.Rd);
            break;
        case MFLO:
            if(dep->lock_or_not(33)) return 1;
            IDEX_line.Rd = IFID_line.command.R1;
            IDEX_line.valB = dep->cpu[33];
            dep->lock_reg(IDEX_line.Rd);
            break;
        case NOP:
            break;
        case SYSCALL: {
            if(dep->lock_or_not(2)) {
                //std::cout << "holy shit" << std::endl;
                return 1;
            }
            IDEX_line.imm = dep->cpu[2];
            switch (IDEX_line.imm) {
                case 1:
                    if(dep->lock_or_not(4)) return 1;
                    IDEX_line.valA = dep->cpu[4];
                    break;
                case 4:
                    if(dep->lock_or_not(4)) return 1;
                    IDEX_line.valA = dep->cpu[4];
                    break;
                case 5:dep->lock_reg(2);break;
                case 8:
                    if(dep->lock_or_not(4) || dep->lock_or_not(5)) return 1;
                    IDEX_line.valA = dep->cpu[4];
                    IDEX_line.valB = dep->cpu[5];
                    break;
                case 9:
                    if(dep->lock_or_not(4)) return 1;
                    IDEX_line.valA = dep->cpu[4];
                    dep->lock_reg(2);
                    break;
                case 10:
                    //std::cout << std::endl;
                    //std::cout << correct << " " << incorrect << std::endl;
                    //c1 = correct;
                    //c2 = incorrect;
                {
#ifndef CAL
                    std::ofstream of("reslut.out", std::ios::app);
                    of << correct << " " << incorrect << std::endl;
#endif
                    exit(0);}
                case 17:
                    //std::cout << std::endl;
                    //std::cout << correct << " " << incorrect << std::endl;
                    //c1 = correct;
                    //c2 = incorrect;
                {
                    if(dep->lock_or_not(4)) return 1;
#ifndef CAL
                    std::ofstream of("reslut.out", std::ios::app);
                    of << correct << " " << incorrect << std::endl;
#endif
                    exit(dep->cpu[4]);
                }

            }
            break;
        }
        case LB:
        case LH:
        case LW:
        case LA:
            IDEX_line.Rd = IFID_line.command.R1;
            if (IFID_line.command.address == -1) {
                if(dep->lock_or_not(IFID_line.command.R2)) return 1;
                IDEX_line.valB = dep->cpu[IFID_line.command.R2];
                IDEX_line.imm = IFID_line.command.Imm;
            } else {
                IDEX_line.valB = 0;
                IDEX_line.imm = IFID_line.command.address;
            }
            dep->lock_reg(IDEX_line.Rd);
            break;
        case SB:
        case SH:
        case SW:
            if(dep->lock_or_not(IFID_line.command.R1)) return 1;
            IDEX_line.Rd = dep->cpu[IFID_line.command.R1];
            if (IFID_line.command.address == -1) {
                if(dep->lock_or_not(IFID_line.command.R2)) return 1;
                IDEX_line.valB = dep->cpu[IFID_line.command.R2];
                IDEX_line.imm = IFID_line.command.Imm;
            } else {
                IDEX_line.valB = 0;
                IDEX_line.imm = IFID_line.command.address;
            }
            break;
    }
    return 0;
}

int program::EX() {
    EXMEM_line.op = IDEX_line.op;
    EXMEM_line.valB = IDEX_line.valB;
    EXMEM_line.dest = IDEX_line.Rd;
    EXMEM_line.Next_command_line = IDEX_line.Next_command_line;
    //std::cout << "EX: " << " " << retran.retrans(EXMEM_line.op) << std::endl;
    int re;
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
                if (IDEX_line.valA == IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;dep->cpu[34] = EXMEM_line.Next_command_line;
                    re = statement.change_st(1);
                }else{
                    re = statement.change_st(0);
                }
            if(re) brunch_cnt = 1;
            break;
        case BNE:
                if (IDEX_line.valA != IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;dep->cpu[34] = EXMEM_line.Next_command_line;
                    re = statement.change_st(1);
                }else{
                    re = statement.change_st(0);
                }
            if(re) brunch_cnt = 1;
            break;
        case BGE:
                if (IDEX_line.valA >= IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;dep->cpu[34] = EXMEM_line.Next_command_line;
                    re = statement.change_st(1);
                } else{
                    re = statement.change_st(0);
                }
            if(re) brunch_cnt = 1;
            break;
        case BLE:
                if (IDEX_line.valA <= IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;dep->cpu[34] = EXMEM_line.Next_command_line;
                    re = statement.change_st(1);
                } else{
                    re = statement.change_st(0);
                }
            if(re) brunch_cnt = 1;
            break;
        case BGT:
                if (IDEX_line.valA > IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;dep->cpu[34] = EXMEM_line.Next_command_line;
                    re = statement.change_st(1);
                } else{
                    re = statement.change_st(0);
                }
            if(re) brunch_cnt = 1;
            break;
        case BLT:
                if (IDEX_line.valA < IDEX_line.valB) {
                    EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                    dep->cpu[34] = EXMEM_line.Next_command_line;
                    re = statement.change_st(1);
                } else{
                    re = statement.change_st(0);
                }
            if(re) brunch_cnt = 1;
            break;
        case BEQZ:
            if (IDEX_line.valA == 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                dep->cpu[34] = EXMEM_line.Next_command_line;
                //std::cout << "holy shit" << std::endl;
                re = statement.change_st(1);
            } else{
                re = statement.change_st(0);
            }
            if(re) brunch_cnt = 1;
            break;
        case BNEZ:
            if (IDEX_line.valA != 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                dep->cpu[34] = EXMEM_line.Next_command_line;
                re = statement.change_st(1);
            } else{
                re = statement.change_st(0);
            }
            if(re) brunch_cnt = 1;
            break;
        case BLEZ:
            if (IDEX_line.valA <= 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                dep->cpu[34] = EXMEM_line.Next_command_line;
                re = statement.change_st(1);
            } else{
                re = statement.change_st(0);
            }
            if(re) brunch_cnt = 1;
            break;
        case BGEZ:
            if (IDEX_line.valA >= 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                dep->cpu[34] = EXMEM_line.Next_command_line;
                re = statement.change_st(1);
            } else{
                re = statement.change_st(0);
            }
            if(re) brunch_cnt = 1;
            break;
        case BLTZ:
            if (IDEX_line.valA < 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                dep->cpu[34] = EXMEM_line.Next_command_line;
                re = statement.change_st(1);
            } else{
                re = statement.change_st(0);
            }
            if(re) brunch_cnt = 1;
            break;
        case BGTZ:
            if (IDEX_line.valA > 0) {
                EXMEM_line.Next_command_line = IDEX_line.Possible_Next_Line;
                dep->cpu[34] = EXMEM_line.Next_command_line;
                re = statement.change_st(1);
            } else{
                re = statement.change_st(0);
            }
            if(re) brunch_cnt = 1;
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
    if(brunch_cnt == 1){
        dep->cpu[34] = EXMEM_line.Next_command_line;
        IFID_line.stop_or_not = 1;
        //dep->unlock_reg(34);
        brunch_cnt = 0;
    }
    if(dep->lock_or_not(34)){
        dep->unlock_reg(34);
    }
    return 0;
}


int program::MA(){
    MEMWB_line.result = EXMEM_line.result;
    MEMWB_line.dest = EXMEM_line.dest;
    MEMWB_line.op = EXMEM_line.op;
    //std::cout << "MA: " << retran.retrans(MEMWB_line.op) << std::endl;
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
    return 0;
}

void program::WB(){
    //std::cout << "WB: " << retran.retrans(MEMWB_line.op) << std::endl;
    switch(MEMWB_line.op){
        case ADD:
        case SUB:
        case ADDIU:
        case SUBU:
        case ADDU:
            dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
            dep->unlock_reg(MEMWB_line.dest);
            break;
        case MUL:
        case MULU:
            if(MEMWB_line.dest == 0){
                dep->cpu[33] = MEMWB_line.result;
                dep->cpu[32] = MEMWB_line.mdata;
                dep->unlock_reg(33);
                dep->unlock_reg(32);
            }
            else{
                dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
                dep->unlock_reg(MEMWB_line.dest);
            }
            break;
        case DIV:
        case DIVU:
            if(MEMWB_line.dest == 0){
                dep->cpu[33] = MEMWB_line.result;
                dep->cpu[32] = MEMWB_line.mdata;
                dep->unlock_reg(33);
                dep->unlock_reg(32);
            }
            else{
                dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
                dep->unlock_reg(MEMWB_line.dest);
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
            dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
            dep->unlock_reg(MEMWB_line.dest);
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
            dep->unlock_reg(31);
            break;
        case JALR:
            dep->cpu[31] = MEMWB_line.result;
            dep->unlock_reg(31);
            break;
        case LB:
        case LH:
        case LW:
            dep->cpu[MEMWB_line.dest] = MEMWB_line.mdata;
            dep->unlock_reg(MEMWB_line.dest);
            break;
        case SB:
        case SH:
        case SW:
            break;
        case MOVE:
        case MFHI:
        case MFLO:
            dep->cpu[MEMWB_line.dest] = MEMWB_line.result;
            dep->unlock_reg(MEMWB_line.dest);
            break;
        case NOP:
            break;
        case SYSCALL:
            switch (MEMWB_line.dest) {
                case 5:{
                    int i;
                    std::cin >> i;
                    dep->cpu[2] = i;
                    dep->unlock_reg(2);
                    break;
                }
                case 9:
                    dep->cpu[2] = MEMWB_line.mdata;
                    dep->unlock_reg(2);
                    break;
            }
            break;
    }
}

void program::run(){
    //int cnt = 0;
    while(true){
        //cnt++;
        if(MEMWB_line.stop_or_not){
            MEMWB_line.stop_or_not = 0;
        }
        else WB();
        if(EXMEM_line.stop_or_not){
            EXMEM_line.stop_or_not = 0;
            MEMWB_line.stop_or_not = 1;
        }
        else MA();
        if(IDEX_line.stop_or_not){
            IDEX_line.stop_or_not = 0;
            EXMEM_line.stop_or_not = 1;
        }
        else EX();
        if(IFID_line.stop_or_not){
            IFID_line.stop_or_not = 0;
            IDEX_line.stop_or_not = 1;
        }
        else {
            if(ID()){
                IDEX_line.stop_or_not = 1;
                continue;
            }
        }
        if(IF()){
            IFID_line.stop_or_not = 1;
        }
        //if(cnt > 10)break;
        /*IF();
        ID();
        EX();
        MA();
        WB();*/
    }
}

#endif //MIPS_PROGRAM_ONE_H
