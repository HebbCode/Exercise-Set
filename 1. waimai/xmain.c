#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "linkqueue.h"
pthread_mutex_t mutex1;//保护 count
pthread_mutex_t mutex2;//保护 队列

int count = 1;
char * name[] = {"辣椒炒肉","西红柿炒蛋","香干炒肉","红烧排骨","红烧肉","水煮鱼"};

//模拟顾客持续下单线程
void* func1(void *arg)
{
    linkqueue * q = (linkqueue *)arg;
    int index;
    while(1)
    {
        index = rand() % 6;
        pthread_mutex_lock(&mutex1);
        if(q->num <= 20)
        {
            enqueue(q,count,index);
            printf("~~~~~~~~~~~~~~~~~顾客%lu已下单，订单号是：%d,菜品名是:%s\n",pthread_self(),count,name[index]);
            count++;
            
            pthread_mutex_unlock(&mutex1);
            sleep(rand()%1+1);//随机延时 1-3 秒，模拟下单间隔时间
        }
        else
        {
            pthread_mutex_unlock(&mutex1);
        }
        

        pthread_mutex_lock(&mutex2);
        if(count > 100)
        {
            pthread_mutex_unlock(&mutex2);
            break;
        }
        else
        {
            pthread_mutex_unlock(&mutex2);
        } 
    }
}

//模拟外卖员接单线程
void* func2(void *arg)
{
    linkqueue * q = (linkqueue *)arg;
    int id;
    int index;
    while(1)
    {
        pthread_mutex_lock(&mutex1);
        if(!queue_is_empty(q))//有订单才接单
        {
            dequeue(q,&id,&index);
            printf("外卖员%lu已接单，订单号是：%d,菜品名是:%s\n",pthread_self(),id,name[index]);
            pthread_mutex_unlock(&mutex1);
            
            sleep(rand()%1+1);//模拟外卖员送单时间
        }
        else
        {
            //pthread_mutex_unlock(&mutex1);

            //pthread_mutex_lock(&mutex2);
            if(count > 100)
            {
                //pthread_mutex_unlock(&mutex2);
                break;
            }
            else
            {
                pthread_mutex_unlock(&mutex2);
            } 
        }

    }
}

int main()
{
    srand(time(NULL));
    linkqueue *q = init_queue();
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);

    pthread_t tid1,tid2,tid3,tid4;

    pthread_create(&tid1,NULL,func1,q);
    pthread_create(&tid2,NULL,func1,q);

    pthread_create(&tid3,NULL,func2,q);
    pthread_create(&tid4,NULL,func2,q);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);

    destory_queue(q);
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    return 0;
}