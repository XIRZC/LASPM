

/*
*斐波那契递归函数定义 
*/
int fibonacci(int i) {
    if(i<=1){
        return 1;
    }
    return fibonacci(i-1)+fibonacci(i-2);
}
//主函数 
int main()
{
    int i;
    i=0;
    while(i<=0){
        printf("fibonacci(%2d) = %d\n",i,fibonacci(i));
        i=i+1;
    }
    return 0;
}
