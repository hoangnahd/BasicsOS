#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3
typedef struct{
 int iPID;
 int iArrival, iBurst;
 int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;
void inputProcess(int n, PCB P[]) {
    printf("Enter process details (PID, Arrival Time, Burst Time):\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d:\n", i + 1);
        printf("PID: ");
        scanf("%d", &P[i].iPID);
        printf("Arrival Time: ");
        scanf("%d", &P[i].iArrival);
        printf("Burst Time: ");
        scanf("%d", &P[i].iBurst);
    }
}

void printProcess(int n, PCB P[]) {
    for (int i = 0; i < n; i++) {
        printf("PID: %d, Arrival Time: %d, Burst Time: %d\n", P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}

void exportGanttChart(int n, PCB P[]) {
    printf("Gantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printf("| P%d ", P[i].iPID);
    }
    printf("|\n");
    for (int i = 0; i < n; i++) {
        printf("%d    ", P[i].iStart);
    }
    printf("%d\n", P[n-1].iFinish);
}

void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[]) {
    for (int i = index; i < *n - 1; i++) {
        P[i] = P[i + 1];
    }
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
}

int partition(PCB P[], int low, int high, int iCriteria) {
    int pivot = P[high].iArrival;
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (iCriteria == SORT_BY_ARRIVAL && P[j].iArrival <= pivot ||
            iCriteria == SORT_BY_PID && P[j].iPID <= pivot ||
            iCriteria == SORT_BY_BURST && P[j].iBurst <= pivot ||
            iCriteria == SORT_BY_START && P[j].iStart <= pivot) {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int iCriteria) {
    if (low < high) {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void calculateAWT(int n, PCB P[]) {
    int totalWaitingTime = 0;
    for (int i = 0; i < n; i++) {
        totalWaitingTime += P[i].iWaiting;
    }
    float avgWaitingTime = (float)totalWaitingTime / n;
    printf("Average Waiting Time: %.2f\n", avgWaitingTime);
}

void calculateATaT(int n, PCB P[]) {
    int totalTurnaroundTime = 0;
    for (int i = 0; i < n; i++) {
        totalTurnaroundTime += P[i].iTaT;
    }
    float avgTurnaroundTime = (float)totalTurnaroundTime / n;
    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime);
}

int main()
{
 PCB Input[10];
 PCB ReadyQueue[10];
 PCB TerminatedArray[10];
 int iNumberOfProcess;
 printf("Please input number of Process: ");
 scanf("%d", &iNumberOfProcess);
 int iRemain = iNumberOfProcess, iReady = 0, iTerminated =
0;
 inputProcess(iNumberOfProcess, Input);
 quickSort(Input, 0, iNumberOfProcess - 1,
SORT_BY_ARRIVAL);
 pushProcess(&iReady, ReadyQueue, Input[0]);
 removeProcess(&iRemain, 0, Input);
 ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
 ReadyQueue[0].iFinish = ReadyQueue[0].iStart +
ReadyQueue[0].iBurst;
 ReadyQueue[0].iResponse = ReadyQueue[0].iStart -
ReadyQueue[0].iArrival;
 ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
 ReadyQueue[0].iTaT = ReadyQueue[0].iFinish -
ReadyQueue[0].iArrival;
 printf("\nReady Queue: ");
 printProcess(1, ReadyQueue);
 while (iTerminated < iNumberOfProcess)
 {
 while (iRemain > 0)
 {
 if (Input[0].iArrival <= ReadyQueue[0].iFinish)
 {
 pushProcess(&iReady, ReadyQueue, Input[0]);
 removeProcess(&iRemain, 0, Input);
 continue;
 }
 else
 break;
 }
 if (iReady > 0)
 {pushProcess(&iTerminated, TerminatedArray,
ReadyQueue[0]);
 removeProcess(&iReady, 0, ReadyQueue);

 ReadyQueue[0].iStart = TerminatedArray[iTerminated
- 1].iFinish;
 ReadyQueue[0].iFinish = ReadyQueue[0].iStart +
ReadyQueue[0].iBurst;
 ReadyQueue[0].iResponse = ReadyQueue[0].iStart -
ReadyQueue[0].iArrival;
 ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
 ReadyQueue[0].iTaT = ReadyQueue[0].iFinish -
ReadyQueue[0].iArrival;
 }
 }
 printf("\n===== FCFS Scheduling =====\n");
 exportGanttChart(iTerminated, TerminatedArray);
 quickSort(TerminatedArray, 0, iTerminated - 1,
SORT_BY_PID);
 calculateAWT(iTerminated, TerminatedArray);
 calculateATaT(iTerminated, TerminatedArray);

 return 0;
}
