#include"ll1Ana.h"


void ll1Ana::analyze()
{
    cout <<"请输入一个输入串:" <<endl;
    cin >>input_str;
    cout <<setw(15) <<"符号栈" <<setw(15) <<"输入串" <<setw(15) <<"所用产生式" <<endl;
    analyze_stack.push_back('#');
    analyze_stack.push_back(products[0].left);  //开始符号
    //栈内剩余字符串
    while(!analyze_stack.empty())
    {
        char X=analyze_stack.back();
        char a=input_str[0];
        int num=pa_table[pair<char,char>(X,a)];
        //栈内剩余字符串
        string stack_left;
        for(list<char>::iterator it=analyze_stack.begin();it!=analyze_stack.end();it++)
            stack_left+=(*it);
        cout <<setw(15) <<stack_left;
        //当前剩余输入串
        cout <<setw(15) <<input_str;
        //匹配过程
        if(X==a&&X=='#')  //接受
        {
            cout <<setw(15) <<"Accepted!!";
            return;
        }
        else if(X==a)  //匹配
        {
            analyze_stack.pop_back();        //出栈
            input_str.erase(input_str.begin());  //输入字符去除首字符
            cout <<setw(15) <<X <<" 匹配";
        }
        else if(num!=-1)        //非终结符扩展
        {
             analyze_stack.pop_back();        //出栈
             string right=products[num].right;
             if(right[0]!=epsilon)
             {
                 for(int i=right.size()-1;i>=0;i--)  //反序入栈
                    analyze_stack.push_back(right[i]);
             }
             cout <<setw(15) <<products[num].left <<"->" <<right;
        }
        else //出错
        {
            cout <<setw(15) <<"ERR";
            return;
        }
        cout <<endl;
    }
}

void ll1Ana::get_pa_table()
{
    cout <<"各产生式右部FIRST(alpha)为:" <<endl;
    for(int i=0;i<products_num;i++)
    {
        char left=products[i].left;
        string right=products[i].right;
        int flag=0;
        set<char> first_alpha;    //用于求产生式右部alpha的整个FIRST集而不仅仅是右部第一个非终结符的FIRST集
        int j;
        for(j=0;j<right.size();j++)
        {
            if(!isupper(right[j]))
            {
                first_alpha.insert(right[j]);
                break;
            }
            else{
                set<char>::iterator it;
                for(it=FIRST[right[j]].begin();it!=FIRST[right[j]].end();it++)
                {
                    if(*it==epsilon)
                        flag=1;
                    else
                        first_alpha.insert(*it);
                }
                if(!flag)
                    break;
                flag=0;
            }
        }
        if(j==right.size())
            first_alpha.insert(epsilon);

        //输出每个first_alpha
        cout <<"FIRST(" <<right <<") = { ";
        set<char>::iterator it;
        for(it=first_alpha.begin();it!=first_alpha.end();it++)
        {
            cout <<*it <<" , ";
            if(*it==epsilon){  //若epsilon属于FIRST(alpha)，则对任何b属于FOLLOW(A)，把A->alpha加到M[A,b]中
                set<char>::iterator itn;
                for(itn=FOLLOW[left].begin();itn!=FOLLOW[left].end();itn++)
                    pa_table[pair<char,char>(left,*itn)]=i;
            }
            else       //对每个终结符a属于FIRST(alpha)，把A->alpha加到M[A,a]中
                pa_table[pair<char,char>(left,*it)]=i;
        }
        cout <<"};" <<endl;
    }
    //输出预测分析表
    cout <<"预测分析表为:"  <<endl;
    for(set<char>::iterator it=VT.begin();it!=VT.end();it++)
    {
        if(*it==epsilon) continue;
        cout <<setw(15) <<*it;
    }
    cout <<endl;
    string temp="t";    //字符转字符串
    for(set<char>::iterator itn=VN.begin();itn!=VN.end();itn++)
    {
        cout  <<*itn <<":";
        for(set<char>::iterator itt=VT.begin();itt!=VT.end();itt++)
        {
            if(*itt==epsilon) continue;
            int num=pa_table[pair<char,char>(*itn,*itt)];
            temp[0]=products[num].left;
            if(num==-1)
                cout <<setw(15) <<"ERR";
            else
                cout <<setw(15) <<temp+"->"+products[num].right;
        }
        cout <<endl;
    }
}

void ll1Ana::get_first(char vn)
{
    int flag=0;
    for(int i=0;i<products_num;i++)
    {
        if(products[i].left==vn)  //遍历产生式使得左部与该非终结符对应，根据右部得到FIRST集
        {
            string temp(products[i].right);
            int j;
            for(j=0;j<temp.size();j++)
            {                           //X->a……或者X->epsilon
                if(!isupper(temp[j]))   //或者从后续第一个非终结符开始，若遇到一个终结符则直接结束
                {
                    FIRST[vn].insert(temp[j]);
                    break;
                }
                else{                   //X->Y……
                    if(FIRST[temp[j]].empty())      //避免重复求FIRST集
                        get_first(temp[j]);
                    set<char>::iterator it;
                    for(it=FIRST[temp[j]].begin();it!=FIRST[temp[j]].end();it++)
                    {
                        if(*it==epsilon)  //置标志位，若上一个非终结符可推出epsilon，则后一个非终结符的FIRST集也可加到X中
                            flag=1;       //X->Y1Y2Y3……且Y1->epsilon……
                        else
                            FIRST[vn].insert(*it);
                    }
                }
                if(!flag)  //若上一非终结符不能推出epsilon则退出循环
                    break;
                flag=0;
            }
            if(j==temp.size())  //Y1,Y2……->epsilon 则X->epsilon
                FIRST[vn].insert(epsilon);
        }
    }
}

void ll1Ana::get_follow(char vn)
{
    int flag=0;
    string temp;
    for(int i=0;i<products_num;i++)  //先在产生式右部找到该非终结符的位置，然后根据右部情况确定其FOLLOW集
    {
        int loc=-1;   //该非终结符在每一个产生式右部的位置即数组下标，
        temp.assign(products[i].right);
        for(int j=0;j<temp.size();j++)
        {
            if(temp[j]==vn)
                loc=j;
        }
        if(loc!=-1)  //找到的前提下
        {
            char left=products[i].left;  //得到产生式左部非终结符
            int j;
            for(j=loc+1;j<temp.size();j++)
            {                               //S *-> alpha Ba……
                if(!isupper(temp[j]))       //或者从后续第一个非终结符开始，若遇到一个终结符则直接结束
                {
                    FOLLOW[vn].insert(temp[j]);
                    break;
                }
                else{
                    if(FIRST[temp[j]].empty())  //避免重复求FIRST集
                        get_first(temp[j]);
                    set<char>::iterator it;
                    for(it=FIRST[temp[j]].begin();it!=FIRST[temp[j]].end();it++)
                    {
                        if(*it==epsilon)
                            flag=1;
                        else                //A->alpha B belta 将FIRST(belta)/epsilon加到FOLLOW(B)中
                            FOLLOW[vn].insert(*it);
                    }
                }
                if(!flag)
                    break;
                flag=0;
            }
            //注意此时要保证左部left和原字符vn不能相等，否则会导致递归无限循环
            if(j==temp.size()&&left!=vn)  //A->alpha B
            {                   //或者是A->alpha B belta且belta->epsilon则FOLLOW(A)加到FOLLOW(B)中
                if(FOLLOW[left].empty())       //避免重复求FOLLOW集
                    get_follow(left);
                set<char>::iterator it;
                for(it=FOLLOW[left].begin();it!=FOLLOW[left].end();it++)
                {
                    FOLLOW[vn].insert(*it);
                }
            }
        }
    }
}

void ll1Ana::init()
{
    string temp;
    cout <<"请输入LL(1)文法，CTRL+Z结束" <<endl;
    while(cin >>temp)
    {
        string str;
        for(int i=0;i<temp.size();i++)     //去除输入中可能存在的空格
        {
            if(temp[i]!=' ')
                str+=temp[i];
        }
        products[products_num].left=str[0];
        VN.insert(str[0]);       //非终结符
        for(int i=3;i<str.size();i++)  //跳过-和>
        {
            products[products_num].right+=str[i];
            if(isupper(str[i]))     //非终结符
                VN.insert(str[i]);
            else  //终结符
                VT.insert(str[i]);
        }
        products_num++;
    }
    cin.clear();  //由于CRTL+Z对cin标志位进行置位，因此需要对标志位进行复位以便后续输入
    set<char>::iterator its,itm;
//    //输出产生式数组
//    cout <<"产生式数组" <<endl;
//    for(int i=0;i<products_num;i++)
//    {
//        cout <<products[i].left <<"->" <<products[i].right <<endl;
//    }
//    //输出非终结符集合
//    cout <<"非终结符集合"  <<endl;
//    for(it=VN.begin();it!=VN.end();it++)
//        cout <<*it <<" ";
//    cout <<endl;
//    //输出终结符集合
//    cout <<"终结符集合" <<endl;
//    for(it=VT.begin();it!=VT.end();it++)
//        cout <<*it <<" ";
//    cout <<endl;
    //FIRST集输出
    cout <<"各非终结符FIRST集如下:" <<endl;
    for(its=VN.begin();its!=VN.end();its++)
    {
        get_first(*its);
        cout <<"FIRST(" <<*its <<")={ ";
        for(itm=FIRST[*its].begin();itm!=FIRST[*its].end();itm++)
            cout <<*itm <<" , ";
        cout <<" };" <<endl;
    }
    char S=products[0].left;  //第一个产生式的左部为开始符号
    FOLLOW[S].insert('#');    //开始符号的FOLLOW集要加入#
    VT.insert('#');         //#作为终结符加入终结符集合
    //FOLLOW集输出
    cout <<"各非终结符FOLLOW集如下:" <<endl;
    for(its=VN.begin();its!=VN.end();its++)
    {
        get_follow(*its);
        cout <<"FOLLOW(" <<*its <<")={ ";
        for(itm=FOLLOW[*its].begin();itm!=FOLLOW[*its].end();itm++)
            cout <<*itm <<" , ";
        cout <<" };" <<endl;
    }
    //赋初值-1便于判断是否为错误
    for(set<char>::iterator itn=VN.begin();itn!=VN.end();itn++)
    {
        for(set<char>::iterator itt=VT.begin();itt!=VT.end();itt++)
        {
            pa_table[pair<char,char>(*itn,*itt)]=-1;
        }
    }
    //获取预测分析表
    get_pa_table();
}
