#ifndef LIST_H
#define LIST_H
//�궨�峣��
#define TOKENLEGNTH 100
//��Ԫ���㶨��
typedef struct Node{
    int syn;  //�ֱ���
    char token[TOKENLEGNTH];   //��ʶ�������ͳ���������ֵ
    struct Node *next;  //��һ���
    struct Node *pre;  //��һ���
}Node;

//��Ԫ������˫��������
typedef struct List{
    Node *head;
    Node *tail;
    int count;  //���������
}List;
//������ز���
void list_init(List *root);
void push_back(Node* node, List* list);
void push_front(Node *node, List* list);
int insert(Node *node, List *list, int locate);
//Node* search(Node *node, List *list, int &locate);
int remove(List *list, int locate);

#endif // LIST_H
