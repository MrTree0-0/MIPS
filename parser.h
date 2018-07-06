//
// Created by 金乐盛 on 2018/7/2.
//

#ifndef MIPS_PARSER_H
#define MIPS_PARSER_H
#include <iostream>
#include <vector>
#include <map>
#include "transfer.h"
#include "scanner.h"
#include "memory.h"
enum DataOrText{
    data, text
};
std::string leave_last(std::string str){
    std::string re;
    re = str.substr(0, str.length() - 1);
    return re;
}
std::string change_into_larger(std::string str){
    for(int i = 0; i < str.length(); i++){
        str[i] = str[i] - 'a' + 'A';
    }
    return str;
}
int change_into_int(std::string str){
    int re = 0;
    if(str[0] == '-'){
        for(int i = 1; i < str.length(); i++){
            re = re * 10 + str[i] - '0';
        }
        return -re;
    }
    else{
        for(int i = 0; i < str.length(); i++){
            re = re * 10 + str[i] - '0';
        }
    }
    return re;
}
void parser(std::istream &is, Memory &mem, std::vector<ScannerToken> &vec, std::map<std::string, int> &map, int &cnt, int &DataOrText){
    transfer reflect1;
    std::string head_word;
    is >> head_word;
    //std::cout << head_word << std::endl;
    if(head_word == ".data") {
        DataOrText = data;
        return;
    }
    if(head_word == ".align"){
        int n;
        is >> n;
        mem.align(n);
        return ;
    }
    if(head_word == ".ascii"){
        std::string str;
        std::string waste;
        getline(is, waste, ' ');
        getline(is, str, '\n');
        int len = str.length();
        char *ch = new char[len];
        int cnt = 0;
        for(int i = 1; i <= len - 2; i++){
            if(str[i] == '\\'){
                if(str[i + 1] == 'n'){
                    ch[cnt++] = '\n';
                } else
                if(str[i + 1] == 't'){
                    ch[cnt++] = '\t';
                } else
                if(str[i + 1] == '\\'){
                    ch[cnt++] = '\\';
                }else
                if(str[i + 1] == '0'){
                    ch[cnt++] = 0;
                }else
                if(str[i + 1] == '\"'){
                    ch[cnt++] = '\"';
                }else
                if(str[i + 1] == '\''){
                    ch[cnt++] = '\'';
                }
                i++;
            }
            else ch[cnt++] = str[i];
        }
        mem.ascii(ch, cnt);
        delete []ch;
        return ;
    }
    if(head_word == ".asciiz"){
        std::string str;
        std::string waste;
        getline(is, waste, ' ');
        getline(is, str, '\n');
        //std::cout << str;
        int len = str.length();
        char *ch = new char[len];
        int cnt = 0;
        for(int i = 1; i <= len - 2; i++){
            if(str[i] == '\\'){
                if(str[i + 1] == 'n'){
                    ch[cnt++] = '\n';
                } else
                if(str[i + 1] == 't'){
                    ch[cnt++] = '\t';
                } else
                if(str[i + 1] == '\\'){
                    ch[cnt++] = '\\';
                } else
                if(str[i + 1] == '0'){
                    ch[cnt++] = '\0';
                } else
                if(str[i + 1] == '\"'){
                    ch[cnt++] = '\"';
                }else
                if(str[i + 1] == '\''){
                    ch[cnt++] = '\'';
                }
                i++;
            }
            else {
                //std::cout << str[i] << std::endl;
                ch[cnt++] = str[i];
            }
        }
        mem.asciiz(ch, cnt);
        delete []ch;
        return;
    }
    if(head_word == ".byte"){
        std::string str;
        is >> str;
        while(str[2] == ','){
            mem.byte(str[1]);
            is >> str;
        }
        return ;
    }
    if(head_word == ".half"){
        std::string str;
        short sh;
        is >> str;
        while(str[2] == ','){
            std::memcpy(&sh, &str, 2);
            mem.half(sh);
            is >> str;
        }
        return ;
    }
    if(head_word == ".word"){
        std::string str;
        is >> str;
        int len = str.length();
        while(str[len - 1] == ','){
            str = leave_last(str);
            int num = change_into_int(str);
            mem.word(num);
            is >> str;
        }
        int num = change_into_int(str);
        mem.word(num);
        return;
    }
    if(head_word == ".space"){
        int n;
        is >> n;
        mem.space(n);
        return ;
    }
    if(head_word == ".text"){
        DataOrText = text;
        return ;
    }
    if(head_word == "add" || head_word == "addu" || head_word == "addiu" || head_word == "sub" || head_word == "subu"
        || head_word == "xor" || head_word == "xoru" || head_word == "rem" || head_word == "remu" || head_word == "seq"
        || head_word == "sge" || head_word == "sgt" || head_word == "sle" || head_word == "slt" || head_word == "sne"){
        head_word = change_into_larger(head_word);
        std::string Rdest, Rsrc1, Src2;
        is >> Rdest >> Rsrc1 >> Src2;
        ScannerToken st;
        st.yuan = head_word + Rdest + Rsrc1 + Src2;
        st.op = reflect1.trans(head_word);
        st.Re = reflect1.trans(leave_last(Rdest));
        st.R1 = reflect1.trans(leave_last(Rsrc1));
        if(Src2[0] == '$'){
            st.R2 = reflect1.trans(Src2);
        }
        else{
            st.Imm = change_into_int(Src2);
            st.R2 = -1;
        }
        st.line_num = cnt++;
        vec.push_back(st);
        return ;
    }
    if(head_word == "mul" || head_word == "mulu" || head_word == "div" || head_word == "divu"){
        head_word = change_into_larger(head_word);
        ScannerToken st;
        st.op = reflect1.trans(head_word);
        std::string R1, R2;
        is >> R1 >> R2;
        st.yuan = head_word + R1 + R2;
        int len2 = R2.length();
        if(R2[len2 - 1] == ','){
            std::string R3;
            is >> R3;
            st.yuan = st.yuan + R3;
            st.Re = reflect1.trans(leave_last(R1));
            st.R1 = reflect1.trans(leave_last(R2));
            if(R3[0] == '$'){
                st.R2 = reflect1.trans(R3);
            }
            else{
                st.Imm = change_into_int(R3);
                st.R2 = -1;
            }
            st.line_num = cnt++;
            vec.push_back(st);
        }
        else {
            st.R1 = reflect1.trans(leave_last(R1));
            if(R2[0] == '$'){
                st.R2 = reflect1.trans(R2);
            }
            else{
                st.Imm = change_into_int(R2);
                st.R2 = -1;
            }
            st.line_num = cnt++;
            vec.push_back(st);
        }
        return;
    }
    if(head_word == "neg" || head_word == "negu" || head_word == "move" ){
        head_word = change_into_larger(head_word);
        std::string Rdest, Rsrc;
        is >> Rdest >> Rsrc;
        ScannerToken st;
        st.yuan = head_word + Rdest + Rsrc;
        st.op = reflect1.trans(head_word);
        st.Re = reflect1.trans(leave_last(Rdest));
        st.R1 = reflect1.trans(Rsrc);
        st.line_num = cnt++;
        vec.push_back(st);
        return ;
    }
    if(head_word == "beq" || head_word == "bne" || head_word == "bge" || head_word == "ble" || head_word == "bgt" || head_word == "blt"){
        head_word = change_into_larger(head_word);
        std::string Rsrc1, Src2, label;
        is >> Rsrc1 >> Src2 >> label;
        ScannerToken st;
        st.yuan = head_word + Rsrc1 + Src2 + label;
        st.op = reflect1.trans(head_word);
        st.R1 = reflect1.trans(leave_last(Rsrc1));
        if(Src2[0] == '$'){
            st.R2 = reflect1.trans(leave_last(Src2));
        }
        else{
            st.Imm = change_into_int(leave_last(Src2));
            st.R2 = -1;
        }
        //st.R2 = reflect1.trans(leave_last(Src2));
        st.label = label;
        st.line_num = cnt++;
        vec.push_back(st);
        return;
    }
    if(head_word == "beqz" || head_word == "bnez" || head_word == "blez" || head_word == "bgez" || head_word == "bgtz" || head_word == "bltz"){
        head_word = change_into_larger(head_word);
        std::string Rsrc, label;
        ScannerToken st;
        is >> Rsrc >> label;
        st.yuan = head_word + Rsrc + label;
        st.op = reflect1.trans(head_word);
        st.R1 = reflect1.trans(leave_last(Rsrc));
        st.label = label;
        st.line_num = cnt++;
        vec.push_back(st);
        return;
    }
    if(head_word == "li"){
        head_word = change_into_larger(head_word);
        std::string Rdest, Imm;
        is >> Rdest >> Imm;
        ScannerToken st;
        st.yuan = head_word + Rdest + Imm;
        st.op = reflect1.trans(head_word);
        st.Re = reflect1.trans(leave_last(Rdest));
        st.Imm = change_into_int(Imm);
        st.line_num = cnt++;
        vec.push_back(st);
        return;
    }
    if(head_word == "b" || head_word == "j" || head_word == "jal"){
        head_word = change_into_larger(head_word);
        ScannerToken st;
        st.op = reflect1.trans(head_word);
        is >> st.label;
        st.yuan = head_word + st.label;
        st.line_num = cnt++;
        vec.push_back(st);
        return;
    }
    if(head_word == "jr" || head_word == "jalr" || head_word == "mfhi" || head_word == "mflo"){
        head_word = change_into_larger(head_word);
        std::string Rrsc;
        is >> Rrsc;
        ScannerToken st;
        st.yuan = head_word + Rrsc;
        st.op = reflect1.trans(head_word);
        st.R1 = reflect1.trans(Rrsc);
        st.line_num = cnt++;
        vec.push_back(st);
        return;
    }
    if(head_word == "la" || head_word == "lb" || head_word == "lh" || head_word == "lw" || head_word == "sb" || head_word == "sh" || head_word == "sw"){
        head_word = change_into_larger(head_word);
        std::string R, address;
        ScannerToken st;
        is >> R >> address;
        st.yuan = head_word + R + address;
        st.op = reflect1.trans(head_word);
        st.R1 = reflect1.trans(leave_last(R));
        if(address[0] == '_'){
            st.label = address;
        }
        else{
            int i;
            if(address[0] == '-'){
                for(i = 1; i < address.length(); i++){
                    if(address[i] == '(') break;
                }
                std::string offset_str = address.substr(1, i - 1);
                st.Imm = -change_into_int(offset_str);
                std::string R2_str = address.substr(i + 1, address.length() - 1 - i - 1);
                st.R2 = reflect1.trans(R2_str);
            }
            else{
                for(i = 0; i < address.length(); i++){
                    if(address[i] == '(') break;
                }
                std::string offset_str = address.substr(0, i);
                st.Imm = change_into_int(offset_str);
                std::string R2_str = address.substr(i + 1, address.length() - 1 - i - 1);
                st.R2 = reflect1.trans(R2_str);
            }
        }
        //st.address = change_into_int(address);
        st.line_num = cnt++;
        vec.push_back(st);
        return;
    }
    if(head_word == "nop" || head_word == "syscall"){
        ScannerToken st;
        st.yuan = head_word;
        head_word = change_into_larger(head_word);
        st.op = reflect1.trans(head_word);
        st.line_num = cnt++;
        vec.push_back(st);
        return;
    }
    //说明这是一个label
    if(DataOrText == data){
        std::string label = leave_last(head_word);
        map[label] = mem.get_static();
    }
    else{
        std::string label = leave_last(head_word);
        map[label] = cnt;
    }
    //std::cout << label << std::endl;
    //map.insert(std::pair<std::string, int>(label, cnt));
    //if(label == "main") return cnt;
    //return;
}

#endif //MIPS_PARSER_H
