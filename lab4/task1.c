#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>

#define PROCESS_SIZE 2400
#define PAGE_TABLE_SIZE PROCESS_SIZE
#define PAGE_NUM 3          // 3-8 pages.
#define PAGE_SIZE 10
#define RANDOM_INT 16
#define EMPTY -1
#define NO_EMPTY_RECORD -1
#define PAGE_FRAME_SIZE 5
#define TEST_NUMBER 100

typedef struct t {
    int stayTime;
    int page;
} pageFrame;

int process[PROCESS_SIZE];
int lostCounter = 0;
int allCounter = 0;
pageFrame pf[PAGE_FRAME_SIZE];

int getRandInt(); // generate random integer number range 0 from RANDOM_INT.
void initProcess(int *pc); // initiallize process array.
void initPageFrame(pageFrame* pf);  // initiallize page Frame;
int checkEmptyPos(pageFrame* pf);   // check if any empty frame in page frame.
int getPosFIFO(pageFrame *pf);      // get appropriate page frame for FIFO.
int isNotLostFrame(pageFrame* pf, int processNumber); // if lost frame return -1, else return page frame number.
void FIFO(pageFrame *pf, int processID);

int main() {
    int i;
    
    initProcess(process);
    initPageFrame(pf);
    for (i = 0; i < TEST_NUMBER; i++) {
        printf("==============第%d次页面读取===============\n", i);
        FIFO(pf, process[i]);
        usleep(200000);
    }
    printf("缺页率 : %.3f\n", (float)lostCounter / allCounter);
    return 0;
}

int getRandInt() {
    int a;
    a = rand() % RANDOM_INT; // 0 - RANDOM_INT processesID.
    return a;
}

void initProcess(int *pc) {
    int i;
    srand((unsigned)time(NULL));
    for (i = 0; i < PROCESS_SIZE; i++) {
        pc[i] = getRandInt();
    }
}

void initPageFrame(pageFrame* pf) {
    int i;
    for (i = 0; i < PAGE_FRAME_SIZE; i++) {
        pf[i].page = EMPTY;
        pf[i].stayTime = 0;
    }
}

void FIFO(pageFrame *pf, int processID) {
    int pos;
    int i;
    int frame;
    if ((frame = isNotLostFrame(pf, processID)) == -1) { // lost frame
        pos = getPosFIFO(pf);
        pf[pos].page = processID;
        pf[pos].stayTime = 0;
        lostCounter++;  // 缺页次数++
        printf("发生缺页, 缺页页框号 : %d, 载入程序 : %d\n", pos, processID);
        printf("当前页框信息\n");
        for (i = 0; i < PAGE_FRAME_SIZE; i++) {
            printf("页框 : %d\t", i);
        }
        printf("\n");
        for (i = 0; i < PAGE_FRAME_SIZE; i++) {
            printf("进程 : %d\t", pf[i].page);
        }
        printf("\n");
    }
    else {
        printf("未发生缺页, 需读取的页面 : %d 在页框中 : %d\n", processID, frame);
        printf("当前页框信息\n");
        for (i = 0; i < PAGE_FRAME_SIZE; i++) {
            printf("页框 : %d\t", i);
        }
        printf("\n");
        for (i = 0; i < PAGE_FRAME_SIZE; i++) {
            printf("进程 : %d\t", pf[i].page);
        }
        printf("\n");
    }
    allCounter++;   // 总次数++
    for (i = 0; i < PAGE_FRAME_SIZE; i++) {
        pf[i].stayTime++;
    }
}

int checkEmptyPos(pageFrame* pf) {
    int i, j;
    for (i = 0; i < PAGE_FRAME_SIZE; i++) {
        if (pf[i].page == EMPTY) {
            return i;
        }
    }
    return NO_EMPTY_RECORD;
}

int getPosFIFO(pageFrame *pf) {
    int pos;
    if ((pos = checkEmptyPos(pf)) != NO_EMPTY_RECORD) {
        return pos;
    }
    else {
        int maxStayTime = pf[0].stayTime;
        int frame = 0;
        int i;
        for (i = 1; i < PAGE_FRAME_SIZE; i++) {
            if (pf[i].stayTime > maxStayTime) {
                maxStayTime = pf[i].stayTime;
                frame = i;
            }
        }
        return frame;
    }
}

int isNotLostFrame(pageFrame* pf, int processNumber) {
    int i;
    for (i = 0; i < PAGE_FRAME_SIZE; i++) {
        if (pf[i].page == processNumber) { // 不缺页返回包含该页面的页框号
            return i;
        }
    }
    return -1; // 缺页返回-1
}