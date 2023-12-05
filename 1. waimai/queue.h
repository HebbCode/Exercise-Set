#ifndef _LINKEQUEUE_H_
#define _LINKEQUEUE_H_

//数据结点的类型
typedef struct qnode
{
	int id;//订单号
	int index;//菜品下标
	struct qnode *next;//指向该结点的下一个结点
}qnode;

//头节点的类型
typedef struct linkqueue
{
	struct qnode *front;//队头
	struct qnode *rear;//队尾
	int num;//实际队列中元素的个数
}linkqueue;

//初始化队列
linkqueue *init_queue();

//销毁队列
linkqueue *destory_queue(linkqueue *q);

//情空队列
void clear_queue(linkqueue *q);

//队列判空
int queue_is_empty(linkqueue *q);

//获取对头元素
int get_front(linkqueue *q,int*d,int *e);

//入队
int enqueue(linkqueue *q,int d,int e);

//出队
int dequeue(linkqueue *q,int *d,int *e);

#endif