#include<stdio.h>
#include <stdlib.h>
#include "queue.h"

//初始化队列
linkqueue *init_queue()
{
	linkqueue *q = (linkqueue *)malloc(sizeof(*q));//给队列分配空间
	q->front = q->rear = NULL;
	q->num = 0;

	return q;
}

//销毁队列
linkqueue *destory_queue(linkqueue *q)
{
    if(q == NULL)
        return q;
    
    clear_queue(q);

    free(q);
    q = NULL;
    return q;
}

//清空队列
void clear_queue(linkqueue *q)
{
    if(q == NULL || q->front == NULL)
        return ;
    
    qnode *px = q->front;

    while(px)
    {
        q->front = q->front->next;
        px->next = NULL;
        free(px);

        px = q->front;
    }

    q->rear = NULL;
    q->num = 0;
    
}

//队列判空
int queue_is_empty(linkqueue *q)
{
    if(q == NULL || q->num == 0)
        return 1;
    return 0;
}

//获取对头元素
int get_front(linkqueue *q,int*d,int*e)
{
    if(queue_is_empty(q))
        return 0;
    
    *d = q->front->id;
    *e = q->front->index;
    return 1;
}

//入队
int enqueue(linkqueue *q,int d,int e)
{
    if(q == NULL)
        return 0;
    
    //开辟空间
    qnode*pnew =(qnode*)malloc(sizeof(qnode));
    pnew->id = d;
    pnew->index = e;
    pnew->next = NULL;

    //插入
    if(q->front == NULL)
    {
        q->front = q->rear = pnew;
    }
    else
    {
        //尾插
        q->rear->next = pnew;
        q->rear = pnew;
    }
    q->num++;
    return 1;
}

//出队
int dequeue(linkqueue *q,int *d,int *e)
{
    if(queue_is_empty(q))
        return 0;
    
    *d = q->front->id;//获取对头元素
    *e = q->front->index;
    qnode *px = q->front;//删除的结点
    q->front = q->front->next;
    px->next = NULL;
    free(px);
    px = NULL;
    //只有一个结点
    if(q->front == NULL)
        q->rear = NULL;
    q->num--;
    return 1;
}