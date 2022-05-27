#ifndef LL1Aan_H
#define LL1Ana_H

#include<iostream>
#include <iomanip>
#include<vector>
#include<set>
#include<map>
#include<list>
#include<string>
#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define epsilon '$'     //使用宏定义符号常量表示空字epsilon
#define PRODUCTS_MAX_LENGTH 20  //产生式最大个数
#define VT_MAX              50  //终结符最大个数
#define VN_MAX              20  //非终结符最大个数

using namespace std;    //使用标准命名空间

//产生式结点定义
typedef struct Node{
    char left;      //产生式左部
    string right;   //产生式右部
}Node;

class ll1Ana
{
private:
    int products_num=0;   //产生式个数
    Node products[PRODUCTS_MAX_LENGTH]; //产生式数组
    set<char> VT;   //终结符集合
    set<char> VN;   //非终结符集合
    map<char,set<char>> FIRST;  //所有非终结符的FIRST集合
    map<char,set<char>> FOLLOW; //所有非终结符的FOLLOW集合
    map<pair<char,char>,int> pa_table;    //预测分析表，键为终结符和非终结符的组合，值为使用的产生式序号
    list<char> analyze_stack;   //LL(1)分析栈
    string input_str;       //输入串
public:
    ll1Ana() { init(); }
    void analyze();             //利用预测分析表对输入串进行分析
private:
    void init();    //输入LL(1)文法后初始化产生式数组、终结符集合、非终结符集合，并计算和输出所有非终结符的FIRST和FOLLOW集
    void get_first(char vn);   //递归计算一个非终结符的FIRST集
    void get_follow(char vn);  //递归计算一个非终结符的FOLLOW集
    void get_pa_table();        //根据FIRST集和FOLLOW集计算并输出预测分析表

};


#endif // LL1Aan_H
