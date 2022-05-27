#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include"list.h"  //自定义链表头文件
#define KEYSLENGTH 40

//预处理函数-对源程序的换行、tab、回车、空格等删掉并用一个空格分隔以及去除注释部分
void pre_treatment(char destStr[],char srcStr[],int count);
//词法分析函数-返回若干个二元组
int lex_parse(char srcStr[],List* L);
//查关键字表返回种别码的函数
int keys2num(char id[10]);
//判断是否是算符的第一个字符
int isOperandsFirst(char ch);
//返回算符种别码的函数
int operands2num(char srcStr[],int &i);
//返回界符种别码的函数
int bounds2num(char ch);
//清空字符串的内容
void clearStr(char str[]);
void showAll(List* l,FILE *out);

//关键字、算符界符表
char idsTable[90][10]={"#",
    //关键字1-32
    "auto","break","case","char","const","continue","default","do",
    "double","else","enum","extern","float","for","goto","if",
    "int","long","register","return","short","signed","sizeof","static",
    "struct","switch","typedef","union","unsigned","void","volatile","while",
    //界符  33-40
    "(",")","{","}",",",";","\"","\'",
    //算符和界符  41-77
    "++","--","->","[","]",
    "*","/","%","+","-",
    "<<",">>","<","<=",">",
    ">=","==","!=","&","^",
    "|","&&","||","?:","=",
    "+=","-=","*=","/=","%=",
    "&=","^=","|=","<<=",">>=",
    ".","!",
    //标识符，常整数，字符串常量，字符常量，小数常数，含指数的常数，十六进制常数，八进制常数  78,79,80,81,82,83,84,85
    "ID","NUM","STRING","CHAR","DECIMAL","EXPONET","HEX","OCTAL",":"
    //:为界符，用86来表示,case语句中会用到
};
int main()
{
    List *l=(List*)malloc(sizeof(List));
    char srcStr[100000],destStr[100000];
    int count=0;
    list_init(l);
    FILE *fp=fopen("main.cpp","r");
    FILE *out=fopen("output.txt","w");
    while((srcStr[count]=fgetc(fp))!=EOF)
        count++;
    srcStr[count]='\0';
    pre_treatment(destStr,srcStr,count);
    printf("源文件字符串：\n%s",srcStr);
    printf("预处理后字符串：\n%s\n",destStr);
    fprintf(out,"预处理后字符串：\n%s\n",destStr);
    if(!lex_parse(destStr,l)){
        printf("程序存在错误！！！\n");
        return 0;
    }
    else
        showAll(l,out);
    return 0;
}

int lex_parse(char srcStr[],List *l){
    char token[TOKENLEGNTH];  //识别到的单词暂时存储的区域
    int syn=-1;  //种别码
    int i=0;  //当前字符串的下标
    int count=0;  //当前单词的下标
    int flag=0;  //左右引号的标志,奇数为1，偶数为0
    clearStr(token);
    while(srcStr[i]!='\0'){
        //字符串合成的if语句优先级要比其他要高，否则会导致先行处理为标识符或常数等其他
        if(syn==39&&(flag%2==1)){  //上一个"则到下一个"结束均为字符串常量
            token[count++]='\"';
            while(srcStr[i]!='\"'){
                if(srcStr[i]=='\\'){  // "\""和"\'"的处理
                    if(srcStr[i+1]=='\''||srcStr[i+1]=='\"'||srcStr[i+1]=='\\')
                        i++;  //跳过 '\'
                    token[count++]=srcStr[i++];
                }
                else
                    token[count++]=srcStr[i++];
            }
            token[count++]='\"';
            token[count]='\0';
            syn=80;
        }
        else if(syn==40&&(flag%2==1)){  //从上一个'到下一个'结束为字符常量
            token[count++]='\'';
            while(srcStr[i]!='\''){
                if(srcStr[i]=='\\'){  // "\""和"\'"的处理
                    if(srcStr[i+1]=='\''||srcStr[i+1]=='\"'||srcStr[i+1]=='\\')
                        i++;  //跳过 '\'
                    token[count++]=srcStr[i++];
                }
                else
                    token[count++]=srcStr[i++];
            }
            token[count++]='\'';
            token[count]='\0';
            syn=81;
        }
        else if(srcStr[i]=='#'){  //以#为界符开头的宏定义指令  0
            syn=0;
            i++;
        }
        else if(isalpha(srcStr[i])||srcStr[i]=='_'){  //前为_或字母，后跟字符数字串或_则为标识符
            while(isalnum(srcStr[i])||srcStr[i]=='_'){
                token[count++]=srcStr[i++];
            }
            token[count]='\0';
            syn=keys2num(token);  //关键字  1-32
            if(syn==0)  //标识符  78
                syn=78;
        }
        else if(isdigit(srcStr[i])){  //数字后跟数字串
            if(srcStr[i]=='0'&&isdigit(srcStr[i+1])){   //八进制常数  85
                while(isdigit(srcStr[i])){
                    token[count++]=srcStr[i++];
               }
               syn=85;
            }
            else if(srcStr[i]=='0'&&(srcStr[i+1]=='x'||srcStr[i+1]=='X')){  //十六进制常数 84
                while(isdigit(srcStr[i])||srcStr[i]=='x'||srcStr[i]=='X'){
                    token[count++]=srcStr[i++];
                }
                syn=84;
            }
            else{   //十进制常数
                syn=79;  //正常情况为常整数   79
                while(isdigit(srcStr[i])||srcStr[i]=='.'||srcStr[i]=='e'||srcStr[i]=='E'||srcStr[i]=='+'||srcStr[i]=='-'){
                    if(srcStr[i]=='.')
                        syn=82;   //普通浮点数  82
                    if(srcStr[i]=='e'||srcStr[i]=='E')
                        syn=83;   //带指数的浮点数  83
                    token[count++]=srcStr[i++];
                }
            }
            token[count]='\0';
        }
        else if(isOperandsFirst(srcStr[i])){   //算符  41-77
            syn=operands2num(srcStr,i);
        }
        else if(bounds2num(srcStr[i])){  //界符  32-40
            syn=bounds2num(srcStr[i]);
            if(syn==39||syn==40)
            {
                flag++;
                i++;
                continue;
            }
            i++;
        }
        else if(isspace(srcStr[i])){
            i++;
            continue;
        }
        /*
        else{
            printf("%c %d",srcStr[i],i);
            return 0;
        }
        */
        Node* node=(Node *)malloc(sizeof(Node));
        node->syn=syn;
        if(token[0])
            strcpy(node->token,token);
        clearStr(token);  //清空token
        count=0;
        push_back(node,l);
    }
    return 1;
}

void showAll(List *l,FILE *out){
    Node* node=l->head;
    while(node!=NULL){
        if(node->syn>=78&&node->syn<=85){  //标识符、常整数、字符串常量、字符常量
            printf("< %s , %s >\n",idsTable[node->syn],node->token);
            fprintf(out,"< %s , %s >\n",idsTable[node->syn],node->token);
        }
        else{  //算符、界符、关键字 一字一符
            printf("< %s , - >\n",idsTable[node->syn]);
            fprintf(out,"< %s , - >\n",idsTable[node->syn]);
        }
        node=node->next;
    }
}

void clearStr(char str[]){
    memset(str,'\0',sizeof(str));
}

int bounds2num(char ch){
    int syn=0;
    if(ch=='(')
        syn=33;
    else if(ch==')')
        syn=34;
    else if(ch=='{')
        syn=35;
    else if(ch=='}')
        syn=36;
    else if(ch==',')
        syn=37;
    else if(ch==';')
        syn=38;
    else if(ch=='\"')
        syn=39;
    else if(ch=='\'')
        syn=40;
    else if(ch==':')
        syn=86;
    return syn;
}

int operands2num(char srcStr[],int &i){
    int syn;
        switch(srcStr[i]){
            //无需超前搜索的字符
            case '[': syn=44;i++;break;
            case ']': syn=45;i++;break;
            case '?': syn=46;i++;break;
            case '.': syn=76;i++;break;
            //需超前搜索的字符
            case '+':
                if(srcStr[i+1]=='+'){
                    syn=41;
                    i+=2;
                }
                else if(srcStr[i+1]=='='){
                    syn=66;
                    i+=2;
                }
                else{
                    syn=49;
                    i++;
                }
                break;
            case '-':
                if(srcStr[i+1]=='-'){
                    syn=42;
                    i+=2;
                }
                else if(srcStr[i+1]=='='){
                    syn=67;
                    i+=2;
                }
                else if(srcStr[i+1]=='>'){
                    syn=43;
                    i+=2;
                }
                else{
                    syn=50;
                    i++;
                }
                break;
            case '*':
                if(srcStr[i+1]=='='){
                    syn=68;
                    i+=2;
                }
                else{
                    syn=46;
                    i++;
                }
                break;
            case '/':
                if(srcStr[i+1]=='='){
                    syn=69;
                    i+=2;
                }
                else{
                    syn=47;
                    i++;
                }
                break;
            case '%':
                if(srcStr[i+1]=='='){
                    syn=70;
                    i+=2;
                }
                else{
                    syn=48;
                    i++;
                }
                break;
            case '&':
                if(srcStr[i+1]=='&'){
                    syn=62;
                    i+=2;
                }
                else if(srcStr[i+1]=='='){
                    syn=71;
                    i+=2;
                }
                else{
                    syn=59;
                    i++;
                }
                break;
            case '|':
                if(srcStr[i+1]=='|'){
                    syn=63;
                    i+=2;
                }
                else if(srcStr[i+1]=='='){
                    syn=73;
                    i+=2;
                }
                else{
                    syn=61;
                    i++;
                }
                break;
            case '!':
                if(srcStr[i+1]=='='){
                    syn=58;
                    i+=2;
                }
                else{
                    syn=77;
                    i++;
                }
                break;
            case '^':
                if(srcStr[i+1]=='='){
                    syn=72;
                    i+=2;
                }
                else{
                    syn=60;
                    i++;
                }
                break;
            case '>':
                if(srcStr[i+1]=='>'){
                    if(srcStr[i+2]=='='){
                        syn=75;
                        i+=3;
                    }
                    else{
                        syn=52;
                        i+=2;
                    }
                }
                else if(srcStr[i+1]=='='){
                    syn=56;
                    i+=2;
                }
                else{
                    syn=55;
                    i++;
                }
                break;
            case '<':
                if(srcStr[i+1]=='<'){
                    if(srcStr[i+2]=='='){
                        syn=74;
                        i+=3;
                    }
                    else{
                        syn=51;
                        i+=2;
                    }
                }
                else if(srcStr[i+1]=='='){
                    syn=54;
                    i+=2;
                }
                else{
                    syn=53;
                    i++;
                }
                break;
            case '=':
                if(srcStr[i+1]=='='){
                    syn=57;
                    i+=2;
                }
                else{
                    syn=65;
                    i++;
                }
                break;
            default:
                break;
        }
    return syn;
}

int isOperandsFirst(char ch){
    if(ch=='+'||ch=='-'||ch=='['||ch==']'||ch=='*'||ch=='/'||
       ch=='%'||ch=='<'||ch=='>'||ch=='&'||ch=='!'||ch=='^'||
       ch=='|'||ch=='?' ||ch=='.'||ch=='=')
        return 1;
    else
        return 0;
}


int keys2num(char id[10]){
    for(int i=0;i<KEYSLENGTH;i++){
        if(strcmp(id,idsTable[i])==0){
            return i;  //找到返回种别码  1-32
        }
    }
    return 0;  //找不到返回0
}

void pre_treatment(char destStr[],char srcStr[],int count){
    int j=0;
    int i=0;
    while(i<count){
        if(isspace(srcStr[i])){
            while(isspace(srcStr[i])){  //多个空格、换行符、制表符均转换为一个空格
                i++;
            }
            destStr[j++]=' ';
        }
        if(srcStr[i]=='/'&&srcStr[i+1]=='/'){  //单行注释的消除
                i+=2;  //跳过//
            while(srcStr[i]!='\n')
                i++;
            i++;  //跳过换行符
            continue;  //回去继续消除空格
        }
        if(srcStr[i]=='/'&&srcStr[i+1]=='*'){   //消除单行注释
            i+=2;  //跳过/*
            while(srcStr[i]!='*'||srcStr[i+1]!='/'){
                i++;
            }
            i+=2;  //跳过*/
            continue;  //回去继续消除空格
        }
        //字符和字符串的#号不能过滤掉，只有宏指令前的#可以过滤掉
        if(srcStr[i]=='#'&&(srcStr[i-1]!='\''&&srcStr[i-1]!='\"')){   //宏指令预处理阶段进行处理，此处不做处理，因此去除宏指令
            while(srcStr[i]!='\n')
                i++;
            i++;
            continue;
        }
        destStr[j++]=srcStr[i++];
    }
    destStr[j]='\0';
}

