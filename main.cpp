#include <iostream>
#include <stdio.h>
#include "scanner.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "memory.h"
#include <string>
#include <map>
#include "parser.h"
#include "program.h"
int main(int agrc, char* agrv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::ifstream in(agrv[1]);
    transfer transfer1;
    CPU cpu;
    Memory mem;
    //std::string filename = agrv[1]; /*"11.s"*/;
    std::vector<ScannerToken> text;//每一句话和其在vec中的位置
    std::map<std::string, int> label;//每一个label的对应vector的位置
    int cnt = 0;//第几句push到vector中了
    int main_num;
    int DataOrText;
    while(in.peek() != EOF){
        parser(in, mem, text, label, cnt, DataOrText, transfer1);
    }

    main_num = label["main"];
    cpu.get_start(4 * 1024 * 1024 - 1, main_num);

    std::vector<ScannerToken>::iterator vec_iter;

    /*for(vec_iter = text.begin(); vec_iter != text.end(); vec_iter++){
        std::cout << vec_iter->line_num << '\t' << vec_iter->op << '\t'<< vec_iter->Re << '\t' << vec_iter->R1 << '\t' << vec_iter->R2 << '\t' << vec_iter->Imm << '\t'  << vec_iter->address<< '\t' <<  vec_iter->label << std::endl ;
    }*/



    for(vec_iter = text.begin(); vec_iter != text.end(); vec_iter++){
        if(vec_iter->label != ""){
            vec_iter->address = label[vec_iter->label];
        }
        /*else{
            if(vec_iter->op == LA || vec_iter->op == LB || vec_iter->op == LH || vec_iter->op == LW || vec_iter->op == SB || vec_iter->op == SH || vec_iter->op == SW){
                vec_iter->address = cpu.get_value(vec_iter->R2) + vec_iter->Imm;
                std::cout <<vec_iter->op << '\t' << vec_iter->address << std::endl;
            }
        }*/
    }


    //std::cout << "here" << std::endl;
    //std::cout << main_num << std::endl;
    /*for(vec_iter = text.begin(); vec_iter != text.end(); vec_iter++){
        std::cout << vec_iter->line_num << '\t' << vec_iter->yuan << std::endl;
        //std::cout << vec_iter->line_num << '\t' << vec_iter->op << '\t'<< vec_iter->Re << '\t' << vec_iter->R1 << '\t' << vec_iter->R2 << '\t' << vec_iter->Imm << '\t'  << vec_iter->address<< '\t' <<  vec_iter->label << std::endl ;
    }
    std::map<std::string, int>::iterator map_iter;
    for(map_iter = label.begin(); map_iter != label.end(); map_iter++){
        std::cout << map_iter->first << ' ' << map_iter->second << std::endl;
    }
    return 0;*/

    program pro(&cpu, &mem, &text);

    //int a[10];

    /*for(int i = 0; i < 10; i++){
        memcpy(a + i, mem.mem + 4 * i, sizeof(int));
        std::cout << a[i] << std::endl;
    }*/
    //std::cout << std::endl;

    pro.run();

    in.close();
    return 0;
}