#include"OperPreAna.h"

/*
样例数据1
E->E+T|E-T|T
T->T*F|T/F|F
F->(E)|i

输入串样例：
i+i#
((i-i)/i+i#  错误
(i+i)/i+i-(i+i/i)#  正确
i+i*(i+i)#  正确


样例数据2

E->E+T|E-T|T
T->T*F|T/F|F
F->(E)|0|1|2|3|4|5|6|7|8|9

1+2#  正确
(1+2)/3+4-(5+6/7)#   正确
((1-2)/3+4#     错误

*/

int main(void)
{
    OperPreAna opa;  //调用构造方法
    opa.analyze();
    return 0;
}
