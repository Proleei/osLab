#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<semaphore.h>

#define PHILOS_NUMBER  5 // also chopsticks number.
#define TIME_RANGE_USEC 400000 // units: usec
#define TIME_BASE_USEC  100000 // units: usec
#define USEC_TO_MSEC 1000

pthread_mutex_t cts[PHILOS_NUMBER];	// 筷子互斥锁数组
sem_t count;			// 最多(PHILOS_NUMBER-1)个人同时开始吃饭,破坏死锁的部分分配条件

int getRandomTime();
void* philos(void* arg);

int main(){
	sem_init(&count, 0, PHILOS_NUMBER - 2 );	// 最多同时4个吃饭
	srand((unsigned)time(NULL));
	pthread_t pid[PHILOS_NUMBER];
	int i;
	/*
	for (i = 0; i < PHILOS_NUMBER; i++) {
		cts[i] = PTHREAD_MUTEX_INITIALIZER; // 初始化互斥锁
	}
	*/
	for (i = 0; i < PHILOS_NUMBER; i++) {
		pthread_create(&pid[i], NULL, philos, (void*)i);
	}
	for (i = 0; i < PHILOS_NUMBER; i++) {
		pthread_join(pid[i], NULL);
	}
	return 0;
}

int getRandomTime() { //100000us~500000us
	int a;
	a = rand() % TIME_RANGE_USEC + TIME_BASE_USEC;
	return a;
}

void* philos(void* arg){
	int leftIndex  = (int)arg; // left hand
	int rightIndex = (leftIndex + 4) % PHILOS_NUMBER; // right hand
	float sleepTime;
    while(1) {
		// 思考
		sleepTime = getRandomTime();
		printf("PhilosID: %ld\tthinking\t\t\tduration: %.2fms\n", syscall(SYS_gettid) % PHILOS_NUMBER, sleepTime / USEC_TO_MSEC);
		usleep(sleepTime);
		// 休息
        sleepTime = getRandomTime();
        printf("PhilosID: %ld\tresting \t\t\tduration: %.2fms\n", syscall(SYS_gettid) % PHILOS_NUMBER, sleepTime / USEC_TO_MSEC);
        usleep(sleepTime);
		// 先拿左边的筷子
		sem_wait(&count); // count - 1
		pthread_mutex_lock(&cts[leftIndex]);
		printf("PhilosID: %ld\ttaking left sticks:  %d\n", syscall(SYS_gettid) % PHILOS_NUMBER, leftIndex);
		// 再拿右边的筷子并吃饭
		pthread_mutex_lock(&cts[rightIndex]);
		sleepTime = getRandomTime();
		printf("PhilosID: %ld\ttaking right sticks: %d\t\teating\tduration: %.2fms\n", syscall(SYS_gettid) % PHILOS_NUMBER, rightIndex, sleepTime / USEC_TO_MSEC);
		usleep(sleepTime);
		// 吃完了，先放下右边的筷子
		pthread_mutex_unlock(&cts[rightIndex]);
		printf("PhilosID: %ld\tputing down right sticks: %d\n", syscall(SYS_gettid) % PHILOS_NUMBER, rightIndex);
		// 再放下左边的筷子
		pthread_mutex_unlock(&cts[leftIndex]);
		printf("PhilosID: %ld\tputing down left sticks:  %d\n", syscall(SYS_gettid) % PHILOS_NUMBER, leftIndex);
		sem_post(&count); // count + 1
    }
}
