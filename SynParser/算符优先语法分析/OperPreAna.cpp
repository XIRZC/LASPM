#include"OperPreAna.h"

void OperPreAna::init()
{
    string temp;
    cout <<"请输入各产生式，CTRL+Z结束输入" <<endl;
    while(cin >>temp){      //产生式数组和终结符和非终结符集合
        string prod="";
        for(int i=0;i<temp.size();i++){
            if(temp[i]!=' ')        //去除产生式中可能的空格
                prod+=temp[i];
        }
        VN.insert(prod[0]);
        int jc=3; //jc从上次位置继续
        while(jc<prod.size()){
            products[prod_count].left=prod[0];
            for(;jc<prod.size();jc++){
                char chn=prod[jc];
                if(chn=='|'){
                    jc++;   //跳过|
                    break;
                }
                products[prod_count].right+=chn;
                if(isVN(chn))
                    VN.insert(chn);
                else
                    VT.insert(chn);
            }
            prod_count++;
        }
    }
    cin.clear();  //由于CRTL+Z对cin标志位进行复位，因此需要对标志位进行复位以便后续输入
    //添加Q->#S#这一产生式 用来将#扩展进来
    char left=products[0].left;     //开始符号
    string S(1,left);               //用于字符串累加
    products[prod_count].left='Q';
    products[prod_count].right="#"+S+"#";
    prod_count++;
    VN.insert('Q');
    VT.insert('#');
    set<char>::iterator it;
    for(it=VN.begin();it!=VN.end();it++){   //求每个非终结符的FirstVT和LastVT集
        getFirstVT(*it);
        getLastVT(*it);
    }
    getTable();     //获取算符优先表
}
void OperPreAna::getFirstVT(char vn)
{
    map<pair<char,char>,bool> F;   //求FirstVT和LastVT时的布尔数组：F[P,a]=true当且仅当a属于FirstVT(P)
    list<pair<char,char>> S;         //用于保存(P,a)的栈
    for(set<char>::iterator itE=VN.begin();itE!=VN.end();itE++){  //置初值false
        for(set<char>::iterator itI=VT.begin();itI!=VT.end();itI++){
            F[pair<char,char>(*itE,*itI)]=false;
        }
    }
    for(int i=0;i<prod_count;i++){      //遍历产生式  P->a……或者P->Qa…… 置真入栈
        char left=products[i].left;
        char r1=products[i].right[0];
        char r2=products[i].right[1];
        if(isVT(r1)){  //终结符
            if(!F[pair<char,char>(left,r1)]){
                F[pair<char,char>(left,r1)]=true;
                S.push_back(pair<char,char>(left,r1));
            }
        }
        else if(isVT(r2)){
            if(!F[pair<char,char>(left,r2)]){
                F[pair<char,char>(left,r2)]=true;
                S.push_back(pair<char,char>(left,r2));
            }
        }
    }
    //根据产生式置布尔数组
    while(!S.empty()){  //栈不空
        pair<char,char> p=S.back();        //(Q,a)  first=Q,second=a
        S.pop_back();
        char first=p.first;
        char second=p.second;
        for(int i=0;i<prod_count;i++){
            char left=products[i].left;
            char r1=products[i].right[0];
            if(first==r1)       //P->Q……
            {
                if(!F[pair<char,char>(left,second)]){
                    F[pair<char,char>(left,second)]=true;
                    S.push_back(pair<char,char>(left,second));
                }
            }
        }
    }
    for(set<char>::iterator itE=VN.begin();itE!=VN.end();itE++){   //遍历布尔数组加入到FirstVT集中
        for(set<char>::iterator itI=VT.begin();itI!=VT.end();itI++){
            if(F[pair<char,char>(*itE,*itI)])
                FirstVT[*itE].insert(*itI);
        }
    }
}
void OperPreAna::getLastVT(char vn)
{
    map<pair<char,char>,bool> F;   //求FirstVT和LastVT时的布尔数组：F[P,a]=true当且仅当a属于FirstVT(P)
    list<pair<char,char>> S;         //用于保存(P,a)的栈
    for(set<char>::iterator itE=VN.begin();itE!=VN.end();itE++){    //置初值false
        for(set<char>::iterator itI=VT.begin();itI!=VT.end();itI++){
            F[pair<char,char>(*itE,*itI)]=false;
        }
    }
    for(int i=0;i<prod_count;i++){      //遍历产生式  P->……a或者P->……aQ 置真入栈
        char left=products[i].left;
        int len=products[i].right.size();
        char re1=products[i].right[len-1];
        char re2=products[i].right[len-2];
        if(isVT(re1)){  //终结符
            if(!F[pair<char,char>(left,re1)]){
                F[pair<char,char>(left,re1)]=true;
                S.push_back(pair<char,char>(left,re1));
            }
        }
        else if(isVT(re2)){
            if(!F[pair<char,char>(left,re2)]){
                F[pair<char,char>(left,re2)]=true;
                S.push_back(pair<char,char>(left,re2));
            }
        }
    }

    while(!S.empty()){  //栈不空
        pair<char,char> p=S.back();        //(Q,a)  first=Q,second=a
        S.pop_back();
        char first=p.first;
        char second=p.second;
        for(int i=0;i<prod_count;i++){
            char left=products[i].left;
            int len=products[i].right.size();
            char re1=products[i].right[len-1];
            if(first==re1)       //P->……Q
            {
                if(!F[pair<char,char>(left,second)]){
                    F[pair<char,char>(left,second)]=true;
                    S.push_back(pair<char,char>(left,second));
                }
            }
        }
    }
    for(set<char>::iterator itE=VN.begin();itE!=VN.end();itE++){  //遍历布尔数组加入到LastVT集中
        for(set<char>::iterator itI=VT.begin();itI!=VT.end();itI++){
            if(F[pair<char,char>(*itE,*itI)])
                LastVT[*itE].insert(*itI);
        }
    }
}

void OperPreAna::getTable()
{
    for(int i=0;i<prod_count;i++){
        int len=products[i].right.size();
        string r=products[i].right;
        for(int j=0;j<=len-2;j++){
            if(isVT(r[j])&&isVT(r[j+1]))
                table[pair<char,char>(r[j],r[j+1])]="=";
            if(j<=len-3&&isVT(r[j])&&isVT(r[j+2])&&isVN(r[j+1]))
                table[pair<char,char>(r[j],r[j+2])]="=";
            if(isVT(r[j])&&isVN(r[j+1]))
            {
                set<char> sc=FirstVT[r[j+1]];
                for(set<char>::iterator it=sc.begin();it!=sc.end();it++)
                    table[pair<char,char>(r[j],*it)]="<";
            }
            if(isVN(r[j])&&isVT(r[j+1]))
            {
                set<char> sc=LastVT[r[j]];
                for(set<char>::iterator it=sc.begin();it!=sc.end();it++)
                    table[pair<char,char>(*it,r[j+1])]=">";
            }
        }
    }
}

void OperPreAna::dispInfo()
{
    //输出产生式
    cout <<"产生式:" <<endl;
    for(int i=0;i<prod_count;i++){
        cout <<products[i].left <<"->" <<products[i].right <<endl;
    }
    //输出终结符集
    set<char>::iterator it,itI;
    cout <<"终结符集:";
    for(it=VT.begin();it!=VT.end();it++){
        cout <<*it <<" ";
    }
    cout <<endl;
    //输出非终结符集
    cout <<"非终结符集:";
    for(it=VN.begin();it!=VN.end();it++){
        cout <<*it <<" ";
    }
    cout <<endl;
    //输出FirstVT集
    cout <<"FirstVT集:" <<endl;
    for(it=VN.begin();it!=VN.end();it++){
        cout <<"FirstVT(" <<*it <<")={";
        for(itI=FirstVT[*it].begin();itI!=FirstVT[*it].end();itI++)
            cout <<*itI <<" , ";
        cout <<"};" <<endl;
    }
    //输出LastVT集
    cout <<"LastVT集:" <<endl;
    for(it=VN.begin();it!=VN.end();it++){
        cout <<"LastVT(" <<*it <<")={";
        for(itI=LastVT[*it].begin();itI!=LastVT[*it].end();itI++)
            cout <<*itI <<" , ";
        cout <<"};" <<endl;
    }
    //输出算符优先表
    cout <<"算符优先表" <<endl;
    cout <<setw(3) <<"VT";
    for(set<char>::iterator itE=VT.begin();itE!=VT.end();itE++){    //输出表头
        cout <<setw(3) <<*itE;
    }
    cout <<endl;
    for(set<char>::iterator itE=VT.begin();itE!=VT.end();itE++){  //输出表内容
        cout <<setw(3) <<*itE;
        for(set<char>::iterator itI=VT.begin();itI!=VT.end();itI++){
            cout <<setw(3) <<table[pair<char,char>(*itE,*itI)];
        }
        cout <<endl;
    }
}

void OperPreAna::analyze()
{
    char str[20];
    char S[25];
    cout <<"请输入待分析的句子，exit结束输入:";
    while(cin >>str&&strcmp(str,"exit")!=0){
        int k=1,id=0,j;
        bool success=false;   //标志位，代表是否分析是否成功
        bool exit=false;     //由于为多次输入不可直接return，因此假如标志位来直接退出循环
        S[k]='#';
        int len=0;
        char a;
        while(str[len++]!='\0');    //计算字符数组存的字符个数
        cout <<setw(20) <<"分析栈" <<setw(20) <<"输入串" <<setw(20) <<"动作" <<endl;
        do{
            a=str[id];  //读一个字符到a中
            if(isVT(S[k]))
                j=k;
            else
                j=k-1;
            while(table[pair<char,char>(S[j],a)]==">"){    //S[j]>a   准备规约动作
                cout <<setw(20) <<getsub(1,k,S);  //输出当前栈内数据
                cout <<setw(20) <<getsub(id,len-1,str);    //输出当前剩余输入串
                cout <<setw(20) <<"规约" <<endl;
                char Q;
                do{
                    Q=S[j];
                    if(isVT(S[j-1]))  //找栈内的终结符
                        j=j-1;
                    else
                        j=j-2;
                }while(table[pair<char,char>(S[j],Q)]!="<");  //S[j]<Q时退出循环
                //把S[j+1]…S[k]规约为某个N
                k=j+1;
                S[k]='N';       //规约
                if(k==2&&a=='#')     //栈内呈现 #N# 则推出正确
                {
                    cout <<setw(20) <<getsub(1,k,S);  //输出当前栈内数据
                    cout <<setw(20) <<getsub(id,len-1,str);    //输出当前剩余输入串
                    cout <<setw(20) <<"成功!!!" <<endl;
                    success=true;
                    exit=true;
                    break;
                }
            }
            if(exit)    //成功或错误时退出循环
                break;
            if(table[pair<char,char>(S[j],a)]=="<"||table[pair<char,char>(S[j],a)]=="=")  //S[j]<a或者S[j]=a  准备移进动作
            {
                cout <<setw(20) <<getsub(1,k,S);  //输出当前栈内数据
                cout <<setw(20) <<getsub(id,len-1,str);    //输出当前剩余输入串
                cout <<setw(20) <<"移进" <<endl;
                k=k+1;
                S[k]=a;     //栈顶数据移进
                id++;       //获取下一输入字符
            }
            else{        //出错
                success=false;
                exit=true;
                break;
            }
        }while(a!='#'&!exit);
         if(success)
            cout <<"输入串符合文法定义!!!" <<endl;
         else
            cout <<"输入串不符合文法定义!!!" <<endl;
    }
}

