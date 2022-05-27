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

#define epsilon '$'     //ʹ�ú궨����ų�����ʾ����epsilon
#define PRODUCTS_MAX_LENGTH 20  //����ʽ������
#define VT_MAX              50  //�ս��������
#define VN_MAX              20  //���ս��������

using namespace std;    //ʹ�ñ�׼�����ռ�

//����ʽ��㶨��
typedef struct Node{
    char left;      //����ʽ��
    string right;   //����ʽ�Ҳ�
}Node;

class ll1Ana
{
private:
    int products_num=0;   //����ʽ����
    Node products[PRODUCTS_MAX_LENGTH]; //����ʽ����
    set<char> VT;   //�ս������
    set<char> VN;   //���ս������
    map<char,set<char>> FIRST;  //���з��ս����FIRST����
    map<char,set<char>> FOLLOW; //���з��ս����FOLLOW����
    map<pair<char,char>,int> pa_table;    //Ԥ���������Ϊ�ս���ͷ��ս������ϣ�ֵΪʹ�õĲ���ʽ���
    list<char> analyze_stack;   //LL(1)����ջ
    string input_str;       //���봮
public:
    ll1Ana() { init(); }
    void analyze();             //����Ԥ�����������봮���з���
private:
    void init();    //����LL(1)�ķ����ʼ������ʽ���顢�ս�����ϡ����ս�����ϣ��������������з��ս����FIRST��FOLLOW��
    void get_first(char vn);   //�ݹ����һ�����ս����FIRST��
    void get_follow(char vn);  //�ݹ����һ�����ս����FOLLOW��
    void get_pa_table();        //����FIRST����FOLLOW�����㲢���Ԥ�������

};


#endif // LL1Aan_H
