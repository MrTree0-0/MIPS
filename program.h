//
// Created by 金乐盛 on 2018/7/3.
//

#ifndef MIPS_PROGRAM_H
#define MIPS_PROGRAM_H
#include "CPU.h"
#include "parser.h"
#include "scanner.h"
#include "transfer.h"
#include "memory.h"
#include <string>
#include "retranfer.h"
class program{
  private:
    CPU *dep;
    Memory *mem;
    std::vector<ScannerToken> *text;
    int now_line;
    int op;
    int Re, R1, R2, Imm;
    std::string label;
    int address;
    transfer tran;
    ScannerToken command;
    //retransfer retran;


  public:
    program(CPU *c, Memory *m, std::vector<ScannerToken> *vec);

    program(program &p);

    int get_address(){
        if(address == -1){
            //std::cout << "yes";
            return dep->get_value(R2) + Imm;
        }
        else return address;
    }

    void IF();

    void ID();

    void EX();

    void MA();

    void WB();

    void run();
};

program::program(CPU *c, Memory *m, std::vector<ScannerToken> *vec):dep(c), mem(m), text(vec), op(0), Re(0), R1(0), R2(0), address(-1){}

program::program(program &p):dep(p.dep),mem(p.mem), text(p.text), op(p.op), Re(p.Re), R1(p.R1), R2(p.R2){
    command = p.command;
    label = p.label;
    now_line = p.now_line;
    address = p.address;
}

void program::IF(){
    int command_line;
    command_line = dep->getcommand();
    //std::cout << command_line << std::endl;
    command = (*text)[command_line];
    //std::cout << command.op << std::endl;
}

void program::ID(){
    op = command.op;
    //std::cout << command.op << std::endl;
    now_line = command.line_num;
    Re = command.Re;
    R1 = command.R1;
    R2 = command.R2;
    Imm = command.Imm;
    label = command.label;
    address = command.address;
}

void program::EX(){
    switch(op){
        case ADD: dep->ADD(Re, R1, R2, Imm); break;
        case ADDU: dep->ADDU(Re, R1, R2, Imm); break;
        case ADDIU: dep->ADDIU(Re, R1, R2, Imm); break;
        case SUB: dep->SUB(Re, R1, R2, Imm); break;
        case SUBU: dep->SUBU(Re, R1, R2, Imm); break;
        case MUL: {
            if(Re == 0){
                dep->MUL(R1, R2, Imm);
            }
            else{
                dep->MUL(Re, R1, R2, Imm);
            }
            break;
        }
        case MULU: {
            if(Re == 0){
                dep->MULU(R1, R2, Imm);
            }
            else{
                dep->MULU(Re, R1, R2, Imm);
            }
            break;
        }
        case DIV:{
            if(Re == 0){
                dep->DIV(R1, R2, Imm);
            }
            else{
                dep->DIV(Re, R1, R2, Imm);
            }
            break;
        }
        case DIVU: {
            if(Re == 0){
                dep->DIVU(R1, R2, Imm);
            }
            else{
                dep->DIVU(Re, R1, R2, Imm);
            }
            break;
        }
        case XOR: dep->XOR(Re, R1, R2, Imm);break;
        case XORU: dep->XORU(Re, R1, R2, Imm); break;
        case NEG: dep->NEG(Re, R1); break;
        case NEGU: dep->NEGU(Re, R1); break;
        case REM: dep->REM(Re, R1, R2, Imm); break;
        case REMU: dep->REMU(Re, R1, R2, Imm); break;
        case LI: dep->LI(Re, Imm); break;
        case SEQ: dep->SEQ(Re, R1, R2, Imm); break;
        case SGE: dep->SGE(Re, R1, R2, Imm); break;
        case SGT: dep->SGT(Re, R1, R2, Imm); break;
        case SLE: dep->SLE(Re, R1, R2, Imm); break;
        case SLT: dep->SLT(Re, R1, R2, Imm); break;
        case SNE: dep->SNE(Re, R1, R2, Imm); break;
        case B: dep->B(address); break;
        case BEQ: dep->BEQ(R1, R2, Imm, address); break;
        case BNE: dep->BNE(R1, R2, Imm, address); break;
        case BGE: dep->BGE(R1, R2, Imm, address); break;
        case BLE: dep->BLE(R1, R2, Imm, address); break;
        case BGT: dep->BGT(R1, R2, Imm, address); break;
        case BLT: dep->BLT(R1, R2, Imm, address); break;
        case BEQZ: dep->BEQZ(R1, address); break;
        case BNEZ: dep->BNEZ(R1, address); break;
        case BLEZ: dep->BLEZ(R1, address); break;
        case BGEZ: dep->BGEZ(R1, address); break;
        case BLTZ: dep->BLTZ(R1, address); break;
        case BGTZ: dep->BGTZ(R1, address); break;
        case J: dep->J(address); break;
        case JR: dep->Jr(R1); break;
        case JAL: dep->JAL(address); break;
        case JALR: dep->JALR(R1); break;
        case LA: {int num = get_address(); dep->LA(R1, num); break;}
        case LB: {int num = get_address(); dep->Load_char(R1, mem->Load(num, 1)); break;}
        case LH: {int num = get_address(); dep->Load_halfword(R1, mem->Load(num, 2)); break;}
        case LW: {int num = get_address(); dep->Load_word(R1, mem->Load(num, 4)); break;}
        case SB: {int num = get_address(); mem->Store_char(num, dep->get_value(R1)); break;}
        case SH: {int num = get_address(); mem->Store_halfword(num, dep->get_value(R1)); break;}
        case SW: {int num = get_address(); mem->Store_word(num, dep->get_value(R1)); break;}
        case MOVE: dep->MOVE(Re, R1); break;
        case MFHI: dep->MFHI(R1); break;
        case MFLO: dep->MFLO(R1); break;
        case NOP: dep->NOP(); break;
        case SYSCALL:
        {int judge = dep->SYSCALL();
            if(judge == 4){
                int address = dep->get_value(tran.trans("$a0"));
                while(mem->mem[address] != '\0'){
                    std::cout << (char)mem->mem[address++];
                }
                //std::cout << '\0';
            } else
            if(judge == 9){
                int len = dep->get_value(tran.trans("$a0"));
                dep->LI(tran.trans("$v0"), mem->data_point);
                mem->data_point += len;
            }else
            if(judge == 8){
                int len = dep->get_value(tran.trans("$a1"));
                int address = dep->get_value(tran.trans("$a0"));
                std::string str;
                std::cin >> str;
                for(int i = 0; i < len; i++){
                    mem->mem[address + i] = str[i];
                }
                mem->mem[address + len] = '\0';
            }
            break;
        }
        //default: throw "bitch";
    }
    //std::cout << "line_num" << '\t' << now_line << '\t' << "opt" << '\t' << retran.retrans(op) << std::endl;

}

void program::MA(){}

void program::WB(){}

void program::run(){
    int cnt = 0;
    while(true){
        /*WB();
        MA();
        EX();
        ID();
        IF();*/
        //if(cnt > 50) break;
        cnt++;
        IF();
        ID();
        //std::cout << now_line << std::endl;
        EX();
        //dep->print();
        MA();
        WB();
    }
}
#endif //MIPS_PROGRAM_H
