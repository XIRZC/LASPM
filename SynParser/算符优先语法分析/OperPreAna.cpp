#include"OperPreAna.h"

void OperPreAna::init()
{
    string temp;
    cout <<"�����������ʽ��CTRL+Z��������" <<endl;
    while(cin >>temp){      //����ʽ������ս���ͷ��ս������
        string prod="";
        for(int i=0;i<temp.size();i++){
            if(temp[i]!=' ')        //ȥ������ʽ�п��ܵĿո�
                prod+=temp[i];
        }
        VN.insert(prod[0]);
        int jc=3; //jc���ϴ�λ�ü���
        while(jc<prod.size()){
            products[prod_count].left=prod[0];
            for(;jc<prod.size();jc++){
                char chn=prod[jc];
                if(chn=='|'){
                    jc++;   //����|
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
    cin.clear();  //����CRTL+Z��cin��־λ���и�λ�������Ҫ�Ա�־λ���и�λ�Ա��������
    //���Q->#S#��һ����ʽ ������#��չ����
    char left=products[0].left;     //��ʼ����
    string S(1,left);               //�����ַ����ۼ�
    products[prod_count].left='Q';
    products[prod_count].right="#"+S+"#";
    prod_count++;
    VN.insert('Q');
    VT.insert('#');
    set<char>::iterator it;
    for(it=VN.begin();it!=VN.end();it++){   //��ÿ�����ս����FirstVT��LastVT��
        getFirstVT(*it);
        getLastVT(*it);
    }
    getTable();     //��ȡ������ȱ�
}
void OperPreAna::getFirstVT(char vn)
{
    map<pair<char,char>,bool> F;   //��FirstVT��LastVTʱ�Ĳ������飺F[P,a]=true���ҽ���a����FirstVT(P)
    list<pair<char,char>> S;         //���ڱ���(P,a)��ջ
    for(set<char>::iterator itE=VN.begin();itE!=VN.end();itE++){  //�ó�ֵfalse
        for(set<char>::iterator itI=VT.begin();itI!=VT.end();itI++){
            F[pair<char,char>(*itE,*itI)]=false;
        }
    }
    for(int i=0;i<prod_count;i++){      //��������ʽ  P->a��������P->Qa���� ������ջ
        char left=products[i].left;
        char r1=products[i].right[0];
        char r2=products[i].right[1];
        if(isVT(r1)){  //�ս��
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
    //���ݲ���ʽ�ò�������
    while(!S.empty()){  //ջ����
        pair<char,char> p=S.back();        //(Q,a)  first=Q,second=a
        S.pop_back();
        char first=p.first;
        char second=p.second;
        for(int i=0;i<prod_count;i++){
            char left=products[i].left;
            char r1=products[i].right[0];
            if(first==r1)       //P->Q����
            {
                if(!F[pair<char,char>(left,second)]){
                    F[pair<char,char>(left,second)]=true;
                    S.push_back(pair<char,char>(left,second));
                }
            }
        }
    }
    for(set<char>::iterator itE=VN.begin();itE!=VN.end();itE++){   //��������������뵽FirstVT����
        for(set<char>::iterator itI=VT.begin();itI!=VT.end();itI++){
            if(F[pair<char,char>(*itE,*itI)])
                FirstVT[*itE].insert(*itI);
        }
    }
}
void OperPreAna::getLastVT(char vn)
{
    map<pair<char,char>,bool> F;   //��FirstVT��LastVTʱ�Ĳ������飺F[P,a]=true���ҽ���a����FirstVT(P)
    list<pair<char,char>> S;         //���ڱ���(P,a)��ջ
    for(set<char>::iterator itE=VN.begin();itE!=VN.end();itE++){    //�ó�ֵfalse
        for(set<char>::iterator itI=VT.begin();itI!=VT.end();itI++){
            F[pair<char,char>(*itE,*itI)]=false;
        }
    }
    for(int i=0;i<prod_count;i++){      //��������ʽ  P->����a����P->����aQ ������ջ
        char left=products[i].left;
        int len=products[i].right.size();
        char re1=products[i].right[len-1];
        char re2=products[i].right[len-2];
        if(isVT(re1)){  //�ս��
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

    while(!S.empty()){  //ջ����
        pair<char,char> p=S.back();        //(Q,a)  first=Q,second=a
        S.pop_back();
        char first=p.first;
        char second=p.second;
        for(int i=0;i<prod_count;i++){
            char left=products[i].left;
            int len=products[i].right.size();
            char re1=products[i].right[len-1];
            if(first==re1)       //P->����Q
            {
                if(!F[pair<char,char>(left,second)]){
                    F[pair<char,char>(left,second)]=true;
                    S.push_back(pair<char,char>(left,second));
                }
            }
        }
    }
    for(set<char>::iterator itE=VN.begin();itE!=VN.end();itE++){  //��������������뵽LastVT����
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
    //�������ʽ
    cout <<"����ʽ:" <<endl;
    for(int i=0;i<prod_count;i++){
        cout <<products[i].left <<"->" <<products[i].right <<endl;
    }
    //����ս����
    set<char>::iterator it,itI;
    cout <<"�ս����:";
    for(it=VT.begin();it!=VT.end();it++){
        cout <<*it <<" ";
    }
    cout <<endl;
    //������ս����
    cout <<"���ս����:";
    for(it=VN.begin();it!=VN.end();it++){
        cout <<*it <<" ";
    }
    cout <<endl;
    //���FirstVT��
    cout <<"FirstVT��:" <<endl;
    for(it=VN.begin();it!=VN.end();it++){
        cout <<"FirstVT(" <<*it <<")={";
        for(itI=FirstVT[*it].begin();itI!=FirstVT[*it].end();itI++)
            cout <<*itI <<" , ";
        cout <<"};" <<endl;
    }
    //���LastVT��
    cout <<"LastVT��:" <<endl;
    for(it=VN.begin();it!=VN.end();it++){
        cout <<"LastVT(" <<*it <<")={";
        for(itI=LastVT[*it].begin();itI!=LastVT[*it].end();itI++)
            cout <<*itI <<" , ";
        cout <<"};" <<endl;
    }
    //���������ȱ�
    cout <<"������ȱ�" <<endl;
    cout <<setw(3) <<"VT";
    for(set<char>::iterator itE=VT.begin();itE!=VT.end();itE++){    //�����ͷ
        cout <<setw(3) <<*itE;
    }
    cout <<endl;
    for(set<char>::iterator itE=VT.begin();itE!=VT.end();itE++){  //���������
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
    cout <<"������������ľ��ӣ�exit��������:";
    while(cin >>str&&strcmp(str,"exit")!=0){
        int k=1,id=0,j;
        bool success=false;   //��־λ�������Ƿ�����Ƿ�ɹ�
        bool exit=false;     //����Ϊ������벻��ֱ��return����˼����־λ��ֱ���˳�ѭ��
        S[k]='#';
        int len=0;
        char a;
        while(str[len++]!='\0');    //�����ַ��������ַ�����
        cout <<setw(20) <<"����ջ" <<setw(20) <<"���봮" <<setw(20) <<"����" <<endl;
        do{
            a=str[id];  //��һ���ַ���a��
            if(isVT(S[k]))
                j=k;
            else
                j=k-1;
            while(table[pair<char,char>(S[j],a)]==">"){    //S[j]>a   ׼����Լ����
                cout <<setw(20) <<getsub(1,k,S);  //�����ǰջ������
                cout <<setw(20) <<getsub(id,len-1,str);    //�����ǰʣ�����봮
                cout <<setw(20) <<"��Լ" <<endl;
                char Q;
                do{
                    Q=S[j];
                    if(isVT(S[j-1]))  //��ջ�ڵ��ս��
                        j=j-1;
                    else
                        j=j-2;
                }while(table[pair<char,char>(S[j],Q)]!="<");  //S[j]<Qʱ�˳�ѭ��
                //��S[j+1]��S[k]��ԼΪĳ��N
                k=j+1;
                S[k]='N';       //��Լ
                if(k==2&&a=='#')     //ջ�ڳ��� #N# ���Ƴ���ȷ
                {
                    cout <<setw(20) <<getsub(1,k,S);  //�����ǰջ������
                    cout <<setw(20) <<getsub(id,len-1,str);    //�����ǰʣ�����봮
                    cout <<setw(20) <<"�ɹ�!!!" <<endl;
                    success=true;
                    exit=true;
                    break;
                }
            }
            if(exit)    //�ɹ������ʱ�˳�ѭ��
                break;
            if(table[pair<char,char>(S[j],a)]=="<"||table[pair<char,char>(S[j],a)]=="=")  //S[j]<a����S[j]=a  ׼���ƽ�����
            {
                cout <<setw(20) <<getsub(1,k,S);  //�����ǰջ������
                cout <<setw(20) <<getsub(id,len-1,str);    //�����ǰʣ�����봮
                cout <<setw(20) <<"�ƽ�" <<endl;
                k=k+1;
                S[k]=a;     //ջ�������ƽ�
                id++;       //��ȡ��һ�����ַ�
            }
            else{        //����
                success=false;
                exit=true;
                break;
            }
        }while(a!='#'&!exit);
         if(success)
            cout <<"���봮�����ķ�����!!!" <<endl;
         else
            cout <<"���봮�������ķ�����!!!" <<endl;
    }
}

