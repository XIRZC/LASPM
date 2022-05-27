#ifndef OperPreAna_H
#define OperPreAna_H
#include<iostream>
#include<map>
#include<set>
#include<list>
#include<iomanip>
#include<algorithm>
#include<string.h>
#include<ctype.h>
#define PRODUCTS_NUM 20

using namespace std;

struct node{
    char left;
    string right;
};

class OperPreAna
{
protected:
    node products[PRODUCTS_NUM];    //产生式数组
    int prod_count=0;  //产生式个数
    set<char> VT;   //终结符集合
    set<char> VN;   //非终结符集合
    map<char,set<char>> FirstVT;  //FirstVT集合
    map<char,set<char>> LastVT;   //LastVT集合
    map<pair<char,char>,string> table;//算符优先表，键为两个终结符，值为算符优先关系,=为相等,>为大于,-1为空,<为小于
public:
    OperPreAna() { init(); dispInfo(); }
    void analyze();
    void init();       //用于输入文法并对终结符集和非终结符集进行初始化,并求出FirstVT和LastVT集，并求出算符优先表
    void getFirstVT(char vn); //获取FirstVT集
    void getLastVT(char vn);  //获取LastVT集
    void getTable();   //获取算符优先表
    void dispInfo();   //输出FirstVT和lastVT集、算符优先表
    bool isVT(char ch) { return !isupper(ch); }
    bool isVN(char ch) { return isupper(ch); }
    string getsub(int ids,int ide,char* str) { string temp; for(int i=ids;i<=ide;i++) temp+=str[i];  return temp; }
};
#endif // OperPreAna_H
