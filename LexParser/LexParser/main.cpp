#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include"list.h"  //�Զ�������ͷ�ļ�
#define KEYSLENGTH 40

//Ԥ������-��Դ����Ļ��С�tab���س����ո��ɾ������һ���ո�ָ��Լ�ȥ��ע�Ͳ���
void pre_treatment(char destStr[],char srcStr[],int count);
//�ʷ���������-�������ɸ���Ԫ��
int lex_parse(char srcStr[],List* L);
//��ؼ��ֱ����ֱ���ĺ���
int keys2num(char id[10]);
//�ж��Ƿ�������ĵ�һ���ַ�
int isOperandsFirst(char ch);
//��������ֱ���ĺ���
int operands2num(char srcStr[],int &i);
//���ؽ���ֱ���ĺ���
int bounds2num(char ch);
//����ַ���������
void clearStr(char str[]);
void showAll(List* l,FILE *out);

//�ؼ��֡���������
char idsTable[90][10]={"#",
    //�ؼ���1-32
    "auto","break","case","char","const","continue","default","do",
    "double","else","enum","extern","float","for","goto","if",
    "int","long","register","return","short","signed","sizeof","static",
    "struct","switch","typedef","union","unsigned","void","volatile","while",
    //���  33-40
    "(",")","{","}",",",";","\"","\'",
    //����ͽ��  41-77
    "++","--","->","[","]",
    "*","/","%","+","-",
    "<<",">>","<","<=",">",
    ">=","==","!=","&","^",
    "|","&&","||","?:","=",
    "+=","-=","*=","/=","%=",
    "&=","^=","|=","<<=",">>=",
    ".","!",
    //��ʶ�������������ַ����������ַ�������С����������ָ���ĳ�����ʮ�����Ƴ������˽��Ƴ���  78,79,80,81,82,83,84,85
    "ID","NUM","STRING","CHAR","DECIMAL","EXPONET","HEX","OCTAL",":"
    //:Ϊ�������86����ʾ,case����л��õ�
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
    printf("Դ�ļ��ַ�����\n%s",srcStr);
    printf("Ԥ������ַ�����\n%s\n",destStr);
    fprintf(out,"Ԥ������ַ�����\n%s\n",destStr);
    if(!lex_parse(destStr,l)){
        printf("������ڴ��󣡣���\n");
        return 0;
    }
    else
        showAll(l,out);
    return 0;
}

int lex_parse(char srcStr[],List *l){
    char token[TOKENLEGNTH];  //ʶ�𵽵ĵ�����ʱ�洢������
    int syn=-1;  //�ֱ���
    int i=0;  //��ǰ�ַ������±�
    int count=0;  //��ǰ���ʵ��±�
    int flag=0;  //�������ŵı�־,����Ϊ1��ż��Ϊ0
    clearStr(token);
    while(srcStr[i]!='\0'){
        //�ַ����ϳɵ�if������ȼ�Ҫ������Ҫ�ߣ�����ᵼ�����д���Ϊ��ʶ������������
        if(syn==39&&(flag%2==1)){  //��һ��"����һ��"������Ϊ�ַ�������
            token[count++]='\"';
            while(srcStr[i]!='\"'){
                if(srcStr[i]=='\\'){  // "\""��"\'"�Ĵ���
                    if(srcStr[i+1]=='\''||srcStr[i+1]=='\"'||srcStr[i+1]=='\\')
                        i++;  //���� '\'
                    token[count++]=srcStr[i++];
                }
                else
                    token[count++]=srcStr[i++];
            }
            token[count++]='\"';
            token[count]='\0';
            syn=80;
        }
        else if(syn==40&&(flag%2==1)){  //����һ��'����һ��'����Ϊ�ַ�����
            token[count++]='\'';
            while(srcStr[i]!='\''){
                if(srcStr[i]=='\\'){  // "\""��"\'"�Ĵ���
                    if(srcStr[i+1]=='\''||srcStr[i+1]=='\"'||srcStr[i+1]=='\\')
                        i++;  //���� '\'
                    token[count++]=srcStr[i++];
                }
                else
                    token[count++]=srcStr[i++];
            }
            token[count++]='\'';
            token[count]='\0';
            syn=81;
        }
        else if(srcStr[i]=='#'){  //��#Ϊ�����ͷ�ĺ궨��ָ��  0
            syn=0;
            i++;
        }
        else if(isalpha(srcStr[i])||srcStr[i]=='_'){  //ǰΪ_����ĸ������ַ����ִ���_��Ϊ��ʶ��
            while(isalnum(srcStr[i])||srcStr[i]=='_'){
                token[count++]=srcStr[i++];
            }
            token[count]='\0';
            syn=keys2num(token);  //�ؼ���  1-32
            if(syn==0)  //��ʶ��  78
                syn=78;
        }
        else if(isdigit(srcStr[i])){  //���ֺ�����ִ�
            if(srcStr[i]=='0'&&isdigit(srcStr[i+1])){   //�˽��Ƴ���  85
                while(isdigit(srcStr[i])){
                    token[count++]=srcStr[i++];
               }
               syn=85;
            }
            else if(srcStr[i]=='0'&&(srcStr[i+1]=='x'||srcStr[i+1]=='X')){  //ʮ�����Ƴ��� 84
                while(isdigit(srcStr[i])||srcStr[i]=='x'||srcStr[i]=='X'){
                    token[count++]=srcStr[i++];
                }
                syn=84;
            }
            else{   //ʮ���Ƴ���
                syn=79;  //�������Ϊ������   79
                while(isdigit(srcStr[i])||srcStr[i]=='.'||srcStr[i]=='e'||srcStr[i]=='E'||srcStr[i]=='+'||srcStr[i]=='-'){
                    if(srcStr[i]=='.')
                        syn=82;   //��ͨ������  82
                    if(srcStr[i]=='e'||srcStr[i]=='E')
                        syn=83;   //��ָ���ĸ�����  83
                    token[count++]=srcStr[i++];
                }
            }
            token[count]='\0';
        }
        else if(isOperandsFirst(srcStr[i])){   //���  41-77
            syn=operands2num(srcStr,i);
        }
        else if(bounds2num(srcStr[i])){  //���  32-40
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
        clearStr(token);  //���token
        count=0;
        push_back(node,l);
    }
    return 1;
}

void showAll(List *l,FILE *out){
    Node* node=l->head;
    while(node!=NULL){
        if(node->syn>=78&&node->syn<=85){  //��ʶ�������������ַ����������ַ�����
            printf("< %s , %s >\n",idsTable[node->syn],node->token);
            fprintf(out,"< %s , %s >\n",idsTable[node->syn],node->token);
        }
        else{  //�����������ؼ��� һ��һ��
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
            //���賬ǰ�������ַ�
            case '[': syn=44;i++;break;
            case ']': syn=45;i++;break;
            case '?': syn=46;i++;break;
            case '.': syn=76;i++;break;
            //�賬ǰ�������ַ�
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
            return i;  //�ҵ������ֱ���  1-32
        }
    }
    return 0;  //�Ҳ�������0
}

void pre_treatment(char destStr[],char srcStr[],int count){
    int j=0;
    int i=0;
    while(i<count){
        if(isspace(srcStr[i])){
            while(isspace(srcStr[i])){  //����ո񡢻��з����Ʊ����ת��Ϊһ���ո�
                i++;
            }
            destStr[j++]=' ';
        }
        if(srcStr[i]=='/'&&srcStr[i+1]=='/'){  //����ע�͵�����
                i+=2;  //����//
            while(srcStr[i]!='\n')
                i++;
            i++;  //�������з�
            continue;  //��ȥ���������ո�
        }
        if(srcStr[i]=='/'&&srcStr[i+1]=='*'){   //��������ע��
            i+=2;  //����/*
            while(srcStr[i]!='*'||srcStr[i+1]!='/'){
                i++;
            }
            i+=2;  //����*/
            continue;  //��ȥ���������ո�
        }
        //�ַ����ַ�����#�Ų��ܹ��˵���ֻ�к�ָ��ǰ��#���Թ��˵�
        if(srcStr[i]=='#'&&(srcStr[i-1]!='\''&&srcStr[i-1]!='\"')){   //��ָ��Ԥ����׶ν��д����˴������������ȥ����ָ��
            while(srcStr[i]!='\n')
                i++;
            i++;
            continue;
        }
        destStr[j++]=srcStr[i++];
    }
    destStr[j]='\0';
}

