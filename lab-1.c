/*gcc lab-1-pthread.c -o lab-1 -lpthread*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

pthread_cond_t condc, condp;   //条件信号量 
pthread_mutex_t mutex;     //互斥锁

int producer_id = 0;
int consumer_id = 0;

int resource = 0;    //缓冲区内数值

void *producer()
{
	int id = ++producer_id;
	while(1)
	{
		sleep(1);
		
		pthread_mutex_lock(&mutex);  //加锁
		while(resource == 10) 
		{
			printf("Producer %d: waitting\n", id);
			pthread_cond_wait(&condp,&mutex); //睡眠，等待消费者唤醒
			printf("Producer %d: resumed\n", id);
		}
		resource++;
		printf("Producer %d: %d -> %d\n", id,resource-1,resource);
		pthread_cond_signal(&condc);  //唤醒消费者
		pthread_mutex_unlock(&mutex);  //解锁
	} 
}

void *consumer()
{
	int id = ++consumer_id;
	while(1)
	{
		sleep(1);
		pthread_mutex_lock(&mutex);
		while(resource == 0)
		{
			printf("Consumer %d: waitting\n",id);
			pthread_cond_wait(&condc,&mutex);
			printf("Consumer %d: resumed\n",id);
		}
		resource--;
		printf("Consumer %d: %d -> %d\n", id,resource+1,resource);
		pthread_cond_signal(&condp);
		pthread_mutex_unlock(&mutex);
	}
}

int main()
{
	int pn,cn;  //生产者、消费者数量
	printf("please input the sum of producer:");
	scanf("%d",&pn);
	printf("please input the sum of consumer:");
	scanf("%d",&cn);
	pthread_t pid[pn];
	pthread_t cid[cn];
	int retp[pn],retc[cn];
	/*初始化公有信号量 mutex*/  
	int initMutex = pthread_mutex_init(&mutex,NULL);
	if (initMutex != 0)
	{
		printf("mutex init failed\n");
		exit(1);
	}
	/*创建生产者线程*/
	for (int i = 0; i < pn; ++i)
	{
		retp[i] = pthread_create(&pid[i],NULL,producer,(void *)(&i));
		if (retp[i] != 0)
		{
			printf("producer%d create failed\n", i);
			exit(1);
		}
	}
	/*创建消费者线程*/  
	for (int i = 0; i < cn; ++i)
	{
		retc[i] = pthread_create(&cid[i],NULL,consumer,NULL);
		if (retc[i] != 0)
		{
			printf("consumer%d create failed\n", i);
			exit(1);
		}
	}
	for(int i = 0; i < pn; i++)  
    {  
            pthread_join(pid[i],NULL);   //对创建的生产者线程进行资源回收   
    }  
    for(int i = 0; i < cn; i++)  
    {   
            pthread_join(cid[i],NULL);   //对创建的消费者线程进行资源回收   
    }  
  	pthread_mutex_destroy(&mutex);
  	pthread_cond_destroy(&condc);
  	pthread_cond_destroy(&condp);
    exit(0);
}