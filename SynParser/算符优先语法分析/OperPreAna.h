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
    node products[PRODUCTS_NUM];    //����ʽ����
    int prod_count=0;  //����ʽ����
    set<char> VT;   //�ս������
    set<char> VN;   //���ս������
    map<char,set<char>> FirstVT;  //FirstVT����
    map<char,set<char>> LastVT;   //LastVT����
    map<pair<char,char>,string> table;//������ȱ���Ϊ�����ս����ֵΪ������ȹ�ϵ,=Ϊ���,>Ϊ����,-1Ϊ��,<ΪС��
public:
    OperPreAna() { init(); dispInfo(); }
    void analyze();
    void init();       //���������ķ������ս�����ͷ��ս�������г�ʼ��,�����FirstVT��LastVT���������������ȱ�
    void getFirstVT(char vn); //��ȡFirstVT��
    void getLastVT(char vn);  //��ȡLastVT��
    void getTable();   //��ȡ������ȱ�
    void dispInfo();   //���FirstVT��lastVT����������ȱ�
    bool isVT(char ch) { return !isupper(ch); }
    bool isVN(char ch) { return isupper(ch); }
    string getsub(int ids,int ide,char* str) { string temp; for(int i=ids;i<=ide;i++) temp+=str[i];  return temp; }
};
#endif // OperPreAna_H
