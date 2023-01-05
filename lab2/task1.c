#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define SLEEP_TIME_USEC 500000

//线程1
void* f1() {
	for (int i = 1; i <= 1000; i++) {
		printf("A:%d\n",i);
		usleep(SLEEP_TIME_USEC);
	}
}

//线程2
void* f2() {
	for(int i = 1000; i > 0; i--) {
		printf("B:%d\n",i);
		usleep(SLEEP_TIME_USEC);
	}
}

int main() {
    pthread_t p1, p2;
    int res1, res2;
    res1 = pthread_create(&p1, NULL, f1, NULL);
    if (res1 != 0) {
        printf("Create p1 failed.\n");
        return 0;
    }
    res2 = pthread_create(&p2, NULL, f2, NULL);
    if (res2 != 0) {
        printf("Create p2 failed.\n");
        return 0;
    }
    pthread_join(p1, NULL);
	pthread_join(p2, NULL);
    return 0;
}