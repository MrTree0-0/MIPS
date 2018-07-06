//
// Created by 金乐盛 on 2018/7/3.
//

#ifndef MIPS_MEMORY_H
#define MIPS_MEMORY_H
#include <cmath>
#include <string>
#include <cstring>
class Memory{
  private:
  public:

    Memory(){
        mem = new unsigned char[1024 * 1024 * 4];
        std::memset(mem, 0, 1024 * 1024 * 4 * sizeof(unsigned char));
        data_point = 0;
    }

    ~Memory(){
        delete mem;
    }

    void align(int n){
        //int end_point = static_cast<int>(pow(2, n));
        int num = pow(2, n);
        /*for(int i = 0; i < num; i++) mem[i] = 0;//此处或许可以优化
        data_point += num;*/
        int left = num - (data_point % num);
        if(left % num == 0) return;
        //for(int i = 1; i <= num; i++) mem[data_point + i] = 0;
        std::memset(mem + data_point + 1, 0, num * sizeof(unsigned char));
        data_point += left;
    }

    int get_static(){
        return data_point;
    }

    void ascii(const char* ch, int len){
        std::memcpy(mem + data_point, ch, len * sizeof(unsigned char));
        /*for(int i = 0; i < len; i++){
            mem[data_point + i] = ch[i];
        }*/
        data_point += len;
    }

    void asciiz(const char* ch, int len){
        std::memcpy(mem + data_point, ch, len * sizeof(unsigned char));
        /*for(int i = 0; i < len; i++){
            mem[data_point + i] = ch[i];
        }*/
        data_point += len;
        mem[data_point++] = '\0';
    }

    void byte(char ch){
        mem[data_point++] = ch;
    }

    void half(short ch){
        std::memcpy(mem + data_point, &ch, 2 * sizeof(unsigned char));
        data_point += 2;
    }

    void word(int ch){
        std::memcpy(mem + data_point, &ch, 4 * sizeof(unsigned char));
        data_point += 4;
    }

    void space(int n){
        int end_point = data_point + n;
        for(int i = data_point; i < end_point; i++) mem[i] = 0;//此处或许可以优化
        data_point = end_point;
    }

    /*void init(){
        fp = data_point;
        sp = 4 * 1024 * 1024 - 1;
    }*/

    /*unsigned char get_char(int address){
        unsigned char ch;
        if(address <= 0) throw "shit";
        ch = mem[address];
        return ch;
    }

    unsigned short get_halfword(int address){
        unsigned short sh;
        sh = mem[address] & 0xffff;
        sh = sh << 8;
        sh = mem[address + 1] | sh;
        return sh;
        if(address <= 0) throw "shit";
        memcpy(&sh, mem + address, 2 * sizeof(unsigned char));
        return sh;
    }

    unsigned int get_word(int address){
        unsigned int word;
        for(int i = 1; i <= 4; i++){
            word = mem[address + i - 1] & 0xff;
            word = word << 8;
        }
        if(address <= 0) throw "shit";
        memcpy(&word, mem + address, 4 * sizeof(unsigned char));
        return word;
    }*/


    void Store(int address, int value, int len){
        for(int i = 0; i < len; i++){
            mem[address + i] = (value >> (8 * i)) & 0xff;
        }
    }

    int Load(int address, int len){
        int ans = 0;
        for(int i = 0; i < len; i++){
            ans |= (mem[address + i] & 0xff) << (8 * i);
        }
        return ans;
    }

    void Store_char(int address, int num){
        /*num = num >> 24;
        mem[address] = num & 0xff;*/
        if(address <= 0) throw "shit";
        Store(address, num, 1);

    }

    void Store_halfword(int address, int num){
        /*num = num >> 16;
        for(int i = 2; i >= 1; i--){
            mem[address + i - 1] = num & 0xff;
            num = num >> 8;
        }*/
        if(address <= 0) throw "shit";
        Store(address, num, 2);

    }

    void Store_word(int address, int num){
        /*for(int i = 4; i >= 1; i-- ){
            mem[address + i - 1] = num & 0xff;
            num = num >> 8;
        }*/
        if(address <= 0) throw "shit";
        Store(address, num, 4);

    }

    unsigned char *mem;
    //int fp, sp;
    int data_point;
};
#endif //MIPS_MEMORY_H
