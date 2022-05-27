#ifndef LIST_H
#define LIST_H
//宏定义常量
#define TOKENLEGNTH 100
//二元组结点定义
typedef struct Node{
    int syn;  //种别码
    char token[TOKENLEGNTH];   //标识符或整型常量的内码值
    struct Node *next;  //下一结点
    struct Node *pre;  //上一结点
}Node;

//二元组序列双向链表定义
typedef struct List{
    Node *head;
    Node *tail;
    int count;  //链表结点个数
}List;
//链表相关操作
void list_init(List *root);
void push_back(Node* node, List* list);
void push_front(Node *node, List* list);
int insert(Node *node, List *list, int locate);
//Node* search(Node *node, List *list, int &locate);
int remove(List *list, int locate);

#endif // LIST_H
