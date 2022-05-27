#include"ll1Ana.h"


void ll1Ana::analyze()
{
    cout <<"������һ�����봮:" <<endl;
    cin >>input_str;
    cout <<setw(15) <<"����ջ" <<setw(15) <<"���봮" <<setw(15) <<"���ò���ʽ" <<endl;
    analyze_stack.push_back('#');
    analyze_stack.push_back(products[0].left);  //��ʼ����
    //ջ��ʣ���ַ���
    while(!analyze_stack.empty())
    {
        char X=analyze_stack.back();
        char a=input_str[0];
        int num=pa_table[pair<char,char>(X,a)];
        //ջ��ʣ���ַ���
        string stack_left;
        for(list<char>::iterator it=analyze_stack.begin();it!=analyze_stack.end();it++)
            stack_left+=(*it);
        cout <<setw(15) <<stack_left;
        //��ǰʣ�����봮
        cout <<setw(15) <<input_str;
        //ƥ�����
        if(X==a&&X=='#')  //����
        {
            cout <<setw(15) <<"Accepted!!";
            return;
        }
        else if(X==a)  //ƥ��
        {
            analyze_stack.pop_back();        //��ջ
            input_str.erase(input_str.begin());  //�����ַ�ȥ�����ַ�
            cout <<setw(15) <<X <<" ƥ��";
        }
        else if(num!=-1)        //���ս����չ
        {
             analyze_stack.pop_back();        //��ջ
             string right=products[num].right;
             if(right[0]!=epsilon)
             {
                 for(int i=right.size()-1;i>=0;i--)  //������ջ
                    analyze_stack.push_back(right[i]);
             }
             cout <<setw(15) <<products[num].left <<"->" <<right;
        }
        else //����
        {
            cout <<setw(15) <<"ERR";
            return;
        }
        cout <<endl;
    }
}

void ll1Ana::get_pa_table()
{
    cout <<"������ʽ�Ҳ�FIRST(alpha)Ϊ:" <<endl;
    for(int i=0;i<products_num;i++)
    {
        char left=products[i].left;
        string right=products[i].right;
        int flag=0;
        set<char> first_alpha;    //���������ʽ�Ҳ�alpha������FIRST�������������Ҳ���һ�����ս����FIRST��
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

        //���ÿ��first_alpha
        cout <<"FIRST(" <<right <<") = { ";
        set<char>::iterator it;
        for(it=first_alpha.begin();it!=first_alpha.end();it++)
        {
            cout <<*it <<" , ";
            if(*it==epsilon){  //��epsilon����FIRST(alpha)������κ�b����FOLLOW(A)����A->alpha�ӵ�M[A,b]��
                set<char>::iterator itn;
                for(itn=FOLLOW[left].begin();itn!=FOLLOW[left].end();itn++)
                    pa_table[pair<char,char>(left,*itn)]=i;
            }
            else       //��ÿ���ս��a����FIRST(alpha)����A->alpha�ӵ�M[A,a]��
                pa_table[pair<char,char>(left,*it)]=i;
        }
        cout <<"};" <<endl;
    }
    //���Ԥ�������
    cout <<"Ԥ�������Ϊ:"  <<endl;
    for(set<char>::iterator it=VT.begin();it!=VT.end();it++)
    {
        if(*it==epsilon) continue;
        cout <<setw(15) <<*it;
    }
    cout <<endl;
    string temp="t";    //�ַ�ת�ַ���
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
        if(products[i].left==vn)  //��������ʽʹ������÷��ս����Ӧ�������Ҳ��õ�FIRST��
        {
            string temp(products[i].right);
            int j;
            for(j=0;j<temp.size();j++)
            {                           //X->a��������X->epsilon
                if(!isupper(temp[j]))   //���ߴӺ�����һ�����ս����ʼ��������һ���ս����ֱ�ӽ���
                {
                    FIRST[vn].insert(temp[j]);
                    break;
                }
                else{                   //X->Y����
                    if(FIRST[temp[j]].empty())      //�����ظ���FIRST��
                        get_first(temp[j]);
                    set<char>::iterator it;
                    for(it=FIRST[temp[j]].begin();it!=FIRST[temp[j]].end();it++)
                    {
                        if(*it==epsilon)  //�ñ�־λ������һ�����ս�����Ƴ�epsilon�����һ�����ս����FIRST��Ҳ�ɼӵ�X��
                            flag=1;       //X->Y1Y2Y3������Y1->epsilon����
                        else
                            FIRST[vn].insert(*it);
                    }
                }
                if(!flag)  //����һ���ս�������Ƴ�epsilon���˳�ѭ��
                    break;
                flag=0;
            }
            if(j==temp.size())  //Y1,Y2����->epsilon ��X->epsilon
                FIRST[vn].insert(epsilon);
        }
    }
}

void ll1Ana::get_follow(char vn)
{
    int flag=0;
    string temp;
    for(int i=0;i<products_num;i++)  //���ڲ���ʽ�Ҳ��ҵ��÷��ս����λ�ã�Ȼ������Ҳ����ȷ����FOLLOW��
    {
        int loc=-1;   //�÷��ս����ÿһ������ʽ�Ҳ���λ�ü������±꣬
        temp.assign(products[i].right);
        for(int j=0;j<temp.size();j++)
        {
            if(temp[j]==vn)
                loc=j;
        }
        if(loc!=-1)  //�ҵ���ǰ����
        {
            char left=products[i].left;  //�õ�����ʽ�󲿷��ս��
            int j;
            for(j=loc+1;j<temp.size();j++)
            {                               //S *-> alpha Ba����
                if(!isupper(temp[j]))       //���ߴӺ�����һ�����ս����ʼ��������һ���ս����ֱ�ӽ���
                {
                    FOLLOW[vn].insert(temp[j]);
                    break;
                }
                else{
                    if(FIRST[temp[j]].empty())  //�����ظ���FIRST��
                        get_first(temp[j]);
                    set<char>::iterator it;
                    for(it=FIRST[temp[j]].begin();it!=FIRST[temp[j]].end();it++)
                    {
                        if(*it==epsilon)
                            flag=1;
                        else                //A->alpha B belta ��FIRST(belta)/epsilon�ӵ�FOLLOW(B)��
                            FOLLOW[vn].insert(*it);
                    }
                }
                if(!flag)
                    break;
                flag=0;
            }
            //ע���ʱҪ��֤��left��ԭ�ַ�vn������ȣ�����ᵼ�µݹ�����ѭ��
            if(j==temp.size()&&left!=vn)  //A->alpha B
            {                   //������A->alpha B belta��belta->epsilon��FOLLOW(A)�ӵ�FOLLOW(B)��
                if(FOLLOW[left].empty())       //�����ظ���FOLLOW��
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
    cout <<"������LL(1)�ķ���CTRL+Z����" <<endl;
    while(cin >>temp)
    {
        string str;
        for(int i=0;i<temp.size();i++)     //ȥ�������п��ܴ��ڵĿո�
        {
            if(temp[i]!=' ')
                str+=temp[i];
        }
        products[products_num].left=str[0];
        VN.insert(str[0]);       //���ս��
        for(int i=3;i<str.size();i++)  //����-��>
        {
            products[products_num].right+=str[i];
            if(isupper(str[i]))     //���ս��
                VN.insert(str[i]);
            else  //�ս��
                VT.insert(str[i]);
        }
        products_num++;
    }
    cin.clear();  //����CRTL+Z��cin��־λ������λ�������Ҫ�Ա�־λ���и�λ�Ա��������
    set<char>::iterator its,itm;
//    //�������ʽ����
//    cout <<"����ʽ����" <<endl;
//    for(int i=0;i<products_num;i++)
//    {
//        cout <<products[i].left <<"->" <<products[i].right <<endl;
//    }
//    //������ս������
//    cout <<"���ս������"  <<endl;
//    for(it=VN.begin();it!=VN.end();it++)
//        cout <<*it <<" ";
//    cout <<endl;
//    //����ս������
//    cout <<"�ս������" <<endl;
//    for(it=VT.begin();it!=VT.end();it++)
//        cout <<*it <<" ";
//    cout <<endl;
    //FIRST�����
    cout <<"�����ս��FIRST������:" <<endl;
    for(its=VN.begin();its!=VN.end();its++)
    {
        get_first(*its);
        cout <<"FIRST(" <<*its <<")={ ";
        for(itm=FIRST[*its].begin();itm!=FIRST[*its].end();itm++)
            cout <<*itm <<" , ";
        cout <<" };" <<endl;
    }
    char S=products[0].left;  //��һ������ʽ����Ϊ��ʼ����
    FOLLOW[S].insert('#');    //��ʼ���ŵ�FOLLOW��Ҫ����#
    VT.insert('#');         //#��Ϊ�ս�������ս������
    //FOLLOW�����
    cout <<"�����ս��FOLLOW������:" <<endl;
    for(its=VN.begin();its!=VN.end();its++)
    {
        get_follow(*its);
        cout <<"FOLLOW(" <<*its <<")={ ";
        for(itm=FOLLOW[*its].begin();itm!=FOLLOW[*its].end();itm++)
            cout <<*itm <<" , ";
        cout <<" };" <<endl;
    }
    //����ֵ-1�����ж��Ƿ�Ϊ����
    for(set<char>::iterator itn=VN.begin();itn!=VN.end();itn++)
    {
        for(set<char>::iterator itt=VT.begin();itt!=VT.end();itt++)
        {
            pa_table[pair<char,char>(*itn,*itt)]=-1;
        }
    }
    //��ȡԤ�������
    get_pa_table();
}
