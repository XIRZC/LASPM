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
    if(list->head == NULL){//如果list头结点为空
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

//插入到队列头部
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


//插入到位置locate
int insert(Node *node, List *list, int locate){
	  //异常处理
    if(locate > list->count || locate < 0)return -1;
    //插入到头部
    if(locate == 0){push_front(node, list); return 1;}
    //插入到尾部
    if(locate == list->count){push_back(node, list); return 1;}
        //其它
    int curValue = -1;
    //查找并插入到cur的前面
    Node *cur = list->head;
    while(++curValue < locate)
        cur = cur->next;
    //插入
    node->next = cur;
    node->pre = cur->pre;
    cur->pre->next = node;
    cur->pre = node;
    //容量加1
    list->count++;
    return 1;
}

/* Node* search(Node *node, List *list, int &locate){
 *     //返回相应的头结点，并记录该结点的位置到locate中
 *     locate = 0;
 *     Node *cur = list->head;
 *     while(cur != NULL && cur->data != node->data){cur = cur->next; locate++;}
 *     if(cur == NULL) locate = - 1;
 *     return cur;
 * }
 */

int remove(List *list, int locate){
	  //异常
    if(locate >= list->count || locate < 0)
        return -1;

    if(locate == 0){//头部
        Node *p = list->head->next;
        free(list->head);
        p->pre = NULL;
        list->head = p;
    }
    else if(locate == list->count - 1){//尾部
        Node *p = list->tail->pre;
        free(list->tail);
        p->next = NULL;
        list->tail = p;
    }
    else{//其它
        //查找并保存到cur中
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
