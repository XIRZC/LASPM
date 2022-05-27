#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"

void list_init(List *root){
    root->head=NULL;
    root->tail=NULL;
    root->count=0;
}

void push_back(Node* node, List* list){
    if(list->head == NULL){//���listͷ���Ϊ��
        list->head = list->tail = node;
        node->next = node->pre = NULL;
        list->count++;
        return;
    }
    node->next = NULL;
    node->pre = list->tail;
    list->tail->next = node;
    list->tail = node;
    list->count++;
}

//���뵽����ͷ��
void push_front(Node *node, List* list){
    if(list->head == NULL){
        list->head = list->tail = node;
        node->next = node->pre = NULL;
        list->count++;
        return;
    }
    node->pre= NULL;
    node->next = list->head;
    list->head->pre = node;
    list->head = node;
    list->count++;
}


//���뵽λ��locate
int insert(Node *node, List *list, int locate){
	  //�쳣����
    if(locate > list->count || locate < 0)return -1;
    //���뵽ͷ��
    if(locate == 0){push_front(node, list); return 1;}
    //���뵽β��
    if(locate == list->count){push_back(node, list); return 1;}
        //����
    int curValue = -1;
    //���Ҳ����뵽cur��ǰ��
    Node *cur = list->head;
    while(++curValue < locate)
        cur = cur->next;
    //����
    node->next = cur;
    node->pre = cur->pre;
    cur->pre->next = node;
    cur->pre = node;
    //������1
    list->count++;
    return 1;
}

/* Node* search(Node *node, List *list, int &locate){
 *     //������Ӧ��ͷ��㣬����¼�ý���λ�õ�locate��
 *     locate = 0;
 *     Node *cur = list->head;
 *     while(cur != NULL && cur->data != node->data){cur = cur->next; locate++;}
 *     if(cur == NULL) locate = - 1;
 *     return cur;
 * }
 */

int remove(List *list, int locate){
	  //�쳣
    if(locate >= list->count || locate < 0)
        return -1;

    if(locate == 0){//ͷ��
        Node *p = list->head->next;
        free(list->head);
        p->pre = NULL;
        list->head = p;
    }
    else if(locate == list->count - 1){//β��
        Node *p = list->tail->pre;
        free(list->tail);
        p->next = NULL;
        list->tail = p;
    }
    else{//����
        //���Ҳ����浽cur��
        int curValue = -1;
        Node *cur = list->head;
        while(++curValue < locate)
            cur = cur->next;
        cur->pre->next = cur->next;
        cur->next->pre = cur->pre;
        free(cur);
    }
    list->count--;
    return 1;
}
