//
// Created by 金乐盛 on 2018/7/4.
//

#ifndef MIPS_PROGRAM_ONE_H
#define MIPS_PROGRAM_ONE_H
#include "CPU.h"
#include "parser.h"
#include "scanner.h"
#include "transfer.h"
#include "memory.h"
#include <string>
class program_one{
  private:
    CPU *dep;
    std::vector<ScannerToken> *text;
    std::string cmd;
    int now_line;
    int op;
    int Re, R1, R2, Imm;
    std::string label;
    int address;
    transfer tran;
    ScannerToken command;

  public:


};
#endif //MIPS_PROGRAM_ONE_H
