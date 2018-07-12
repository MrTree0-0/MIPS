//
// Created by 金乐盛 on 2018/7/12.
//

#ifndef MIPS_AUTOBRANCH_H
#define MIPS_AUTOBRANCH_H
#include <iostream>
class autobranch{
    friend class program;
    enum STATE{
        SNT, WNT, WT, ST
    };
  private:
    int *st;
    unsigned int history;
    int n;

  public:
    autobranch(int num = 25){
        n = num;
        st = new int[1<<n];
        std::memset(st, 0, (1<<n) * sizeof(int));
    }

    ~autobranch(){
        delete st;
        //delete history;
    }

    unsigned int get_place(){
        unsigned int a = 0xffffffff;
        a = a>>(32 - n);
        return history & a;
    }

    void update_history(int i){
        history = (history<<1) | i;
    }

    int get_predict(){
        return st[get_place()];
    }

    bool upper_statment(int &i){
        if(i < 3) i++;
        if(i == 2 || i == 1) return true;
        else return false;
    }

    bool lower_statement(int &i){
        if(i >= 1) i--;
        if(i == 1 || i == 2) return true;
        return false;
    }
    int change_st(int i){
        int re;
        if(i) re = upper_statment(st[get_place()]);
        else re = lower_statement(st[get_place()]);
        update_history(i);
        return re;
    }

};
#endif //MIPS_AUTOBRANCH_H
