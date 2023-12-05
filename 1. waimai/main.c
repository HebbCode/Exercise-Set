#include "queue.h"
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFMAX 20
#define ORDER_NUM_MAX 100

char * name[] = {"辣椒炒肉","西红柿炒蛋","香干炒肉","红烧排骨","红烧肉","水煮鱼"};
// 订单号
int OrderNum = 0; 

//创建两个互斥锁
pthread_mutex_t mutex1,mutex2;

// int BufNum = 0; 使用队列的数量代替

void* placeOrder(void *order)
{
	linkqueue *orderBuf = (linkqueue *)order;
	int randIndex = 0;
	while(1)
	{
		// 访问订单量 加锁
		pthread_mutex_lock(&mutex2);
		if(orderBuf->num > ORDER_NUM_MAX) {
			pthread_mutex_unlock(&mutex2);
			break;
		}
		else {
			pthread_mutex_unlock(&mutex2);

			pthread_mutex_lock(&mutex1);
			OrderNum++;
			pthread_mutex_unlock(&mutex1);
			
		}

		// 访问订单缓冲区 加锁
		pthread_mutex_lock(&mutex2);
		if(orderBuf->num < BUFMAX) {	
			randIndex = rand()%6;
			enqueue(orderBuf,OrderNum,randIndex);
			printf("////顾客%lu已下单，订单号是：%d,菜品名是:%s\n",pthread_self(),OrderNum,name[randIndex]);
			pthread_mutex_unlock(&mutex2);
			sleep(rand()%2+1);//随机延时 1-2 秒，模拟下单间隔时间
		}
		else pthread_mutex_unlock(&mutex2);
	}
}
void* takingOrder(void *order)
{
	linkqueue *orderBuf = (linkqueue *)order;
	int id,index;
	while(1)
	{
		// 给buf加锁
		pthread_mutex_lock(&mutex2);
        if(!queue_is_empty(orderBuf))//有订单才接单
        {
            dequeue(orderBuf,&id,&index);
            printf("外卖员%lu已接单，订单号是：%d,菜品名是:%s\n",pthread_self(),id,name[index]);
            pthread_mutex_unlock(&mutex2);

            sleep(rand()%2+1);
        }
        else
        {
			pthread_mutex_unlock(&mutex2);

			// 给OrderNum加锁
            pthread_mutex_lock(&mutex1);
            if(OrderNum > ORDER_NUM_MAX)
            {
                pthread_mutex_unlock(&mutex1);
                break;
            }
            else
            {
                pthread_mutex_unlock(&mutex1);
            } 
        }
	}
}

int main()
{
	srand(time(NULL));

	// 点单线程
	pthread_t potid1,potid2; 
	// 送单线程
	pthread_t totid1,totid2; 
	
	// 初始化锁
	pthread_mutex_init(&mutex1,NULL);
	pthread_mutex_init(&mutex2,NULL);

	linkqueue *orderBuf = init_queue();

	//开四个线程
	pthread_create(&potid1,NULL,placeOrder,orderBuf);
	pthread_create(&potid2,NULL,placeOrder,orderBuf);
	pthread_create(&totid1,NULL,takingOrder,orderBuf);
	pthread_create(&totid2,NULL,takingOrder,orderBuf);

	// 等待线程回收资源
	pthread_join(potid1,NULL);
	pthread_join(potid2,NULL);
	pthread_join(totid1,NULL);
	pthread_join(totid2,NULL);

	// 销毁锁
	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
	// 销毁队列 
	destory_queue(orderBuf);
	return 0;
}