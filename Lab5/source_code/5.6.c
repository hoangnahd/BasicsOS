#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6;
int w, v, y, z, ans;
sem_t sem_ab, sem_cd, sem_ef, sem_g;

void *processAB(void *arg) {
    while (1) {
        sem_wait(&sem_ab);
        w = x1 * x2;
        v = x3 * x4;
        printf("Process AB: w: %d, v: %d\n", w, v);
        sem_post(&sem_cd);
    }
}

void *processCD(void *arg) {
    while (1) {
        sem_wait(&sem_cd);
        y = v * x5;
        z = v * x6;
        printf("Process CD: y: %d, z: %d\n", y, z);
        sem_post(&sem_ef);
    }
}

void *processEF(void *arg) {
    while (1) {
        sem_wait(&sem_ef);
        y = w * y;
        z = w * z;
        printf("Process EF: y: %d, z: %d\n", y, z);
        sem_post(&sem_g);
    }
}

void *processG(void *arg) {
    while (1) {
        sem_wait(&sem_g);
        ans = y + z;
        printf("Process G: ans: %d\n", ans);
        sem_post(&sem_ab);
    }
}

int main() {
    // Khởi tạo Semaphore
    sem_init(&sem_ab, 0, 1);
    sem_init(&sem_cd, 0, 0);
    sem_init(&sem_ef, 0, 0);
    sem_init(&sem_g, 0, 0);

    // Tạo Thread
    pthread_t tidAB, tidCD, tidEF, tidG;
    pthread_create(&tidAB, NULL, processAB, NULL);
    pthread_create(&tidCD, NULL, processCD, NULL);
    pthread_create(&tidEF, NULL, processEF, NULL);
    pthread_create(&tidG, NULL, processG, NULL);

    // Chờ Thread kết thúc
    pthread_join(tidAB, NULL);
    pthread_join(tidCD, NULL);
    pthread_join(tidEF, NULL);
    pthread_join(tidG, NULL);

    // Hủy Semaphore
    sem_destroy(&sem_ab);
    sem_destroy(&sem_cd);
    sem_destroy(&sem_ef);
    sem_destroy(&sem_g);

    return 0;
}
