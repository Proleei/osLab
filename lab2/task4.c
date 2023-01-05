#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<semaphore.h>

#define DATA_RANGE 1000
#define TIME_BASE_USEC  100000 // 100 msec
#define TIME_RANGE_USEC 900000 // 900 msec
#define USEC_TO_MSEC 1000
#define SMALL_ID 3

int share[10];	// 共享缓冲区
int indexc = 0;	// 生产者临界资源
int indexp = 0;	// 消费者临界资源
static int index = 0; // 缓冲区计数
sem_t is_empty;	// 缓冲区有空位信号量
sem_t is_full;	// 缓冲区满信号量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //互斥锁

int getRandomData(int dataBase);
int getRandomTime();
void* producer(void* arg);
void* consumer(void* arg);

int main(){
	srand((unsigned)time(NULL));
	sem_init(&is_empty,0,0);// 初值为0，初始缓冲区为0
	sem_init(&is_full,0,9);	// 初值为9，初始缓冲区为10
	pthread_t p1, p2; //两个生产者
	pthread_t c1, c2, c3; // 三个消费者
	pthread_create(&p1, NULL, producer, (void*)1000); // 创建线程
	pthread_create(&p2, NULL, producer, (void*)2000);
	pthread_create(&c1, NULL, consumer, NULL);
	pthread_create(&c2, NULL, consumer, NULL);
	pthread_create(&c3, NULL, consumer, NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(c1,NULL);
	pthread_join(c2,NULL);
	pthread_join(c3,NULL);
	return 0;
}

int getRandomData(int dataBase) {
	int a;
	a = rand() % DATA_RANGE + dataBase;
	return a;
}

int getRandomTime() {
	int a;
	a = rand() % TIME_RANGE_USEC + TIME_BASE_USEC;
	return a;
}

void* producer(void* arg) {
	int base = (int)arg;
        while(1) {
			sem_wait(&is_full); // is_full + 1
            pthread_mutex_lock(&mutex);
			int data;
			data = getRandomData(base);
            share[index]=data;
			float sleepTime;
			sleepTime = getRandomTime();
            printf("ProducerID: %ld\tput:  %d block\t\tdata: %d\tsleep: %.2fms\n", syscall(SYS_gettid) % 3, index, data, sleepTime / USEC_TO_MSEC);
			index++;
			pthread_mutex_unlock(&mutex);
            sem_post(&is_empty); // is_empty + 1
            usleep(sleepTime);
        }
}

void* consumer(void* arg) {
	while(1) {
		sem_wait(&is_empty); // is_empty - 1
		pthread_mutex_lock(&mutex);
		float sleepTime;
		sleepTime = getRandomTime();
		index--;
        printf("ConsumerID: %ld\ttake: %d block\t\tdata: %d\tsleep: %.2fms\n", syscall(SYS_gettid) % 3, index, share[index], sleepTime / USEC_TO_MSEC);
        pthread_mutex_unlock(&mutex);
		sem_post(&is_full); // is_full + 1
        usleep(sleepTime);
    }

}


/*
rules:
	1.缓冲区满不能生产
	2.缓冲区空不能消费
	3.每时刻只能使一个生产者或消费者存或取
	4.生产者或消费者一次只能存或取一个数据
*/
