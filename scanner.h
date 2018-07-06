//
// Created by 金乐盛 on 2018/7/2.
//

#ifndef MIPS_SCANNER_H
#define MIPS_SCANNER_H
#include <iostream>
#include <cstring>
#include <string>
enum OP{

};

class ScannerToken{
  //private:
  public:
    //std::string st;
    int op;
    int line_num;
    int Re, R1, R2, Imm;
    std::string label;
    std::string yuan;
    int address;

  public:
    ScannerToken(){
        op = line_num = Re = R1 = R2 = Imm =  0;
        address = -1;
        label = "";
    }

    ScannerToken(const ScannerToken &st):op(st.op), line_num(st.line_num), Re(st.Re), R1(st.R1), R2(st.R2), Imm(st.Imm), address(st.address){
        label = st.label;
        yuan = st.yuan;
    }

    ScannerToken &operator=(const ScannerToken &st){
        ScannerToken re;
        op = st.op;
        line_num = st.line_num;
        Re = st.Re;
        R1 = st.R1;
        R2 = st.R2;
        Imm = st.Imm;
        yuan = st.yuan;
        label = st.label;
        address = st.address;
        return *this;
    }
};


#endif //MIPS_SCANNER_H
