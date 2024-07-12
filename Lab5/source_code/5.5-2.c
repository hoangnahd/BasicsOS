#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX 100 // Giới hạn kích thước mảng

int a[MAX]; // Mảng a toàn cục
int n = 0; // Số lượng phần tử hiện tại trong mảng
sem_t sem_producer, sem_consumer; // Semaphore để đồng bộ hóa
pthread_mutex_t lock; // Mutex để bảo vệ biến dùng chung

void *producer(void *arg) {
    while (1) {
        int num = rand() % 100; // Sinh số ngẫu nhiên

        sem_wait(&sem_producer); // Chờ semaphore của producer

        pthread_mutex_lock(&lock); // Khóa mutex

        a[n] = num; // Thêm phần tử vào mảng
        n++; // Tăng số lượng phần tử
        printf("Added %d to array. Number of elements: %d\n", num, n);
        for(int i = 0 ; i < n ; i++) {
            printf(" %d ", a[i]);
            
        }
            
        printf("\n");
        pthread_mutex_unlock(&lock); // Mở khóa mutex
        sem_post(&sem_consumer); // Giải phóng semaphore của consumer
        
    }
    return NULL;
}

void *consumer(void *arg) {
    while (1) {
        sem_wait(&sem_consumer); // Chờ semaphore của consumer

        pthread_mutex_lock(&lock); // Khóa mutex
        if(n <= 0) {
            printf("Nothing in array a\n");
        }

        int value = a[n-1];
        n--; // Giảm số lượng phần tử
        printf("Removed %d from array. Number of elements: %d\n", value, n);
        for(int i = 0 ; i < n ; i++) {
            printf(" %d ", a[i]);
            
        }
        pthread_mutex_unlock(&lock); // Mở khóa mutex
        sem_post(&sem_producer);
            
        printf("\n");
    }
    return NULL;
}

int main() {
    srand(time(NULL)); // Khởi tạo seed cho hàm rand()

    sem_init(&sem_producer, 0, MAX); // Khởi tạo semaphore của producer
    sem_init(&sem_consumer, 0, 0); // Khởi tạo semaphore của consumer
    pthread_mutex_init(&lock, NULL); // Khởi tạo mutex

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, producer, NULL); // Tạo thread producer
    pthread_create(&tid2, NULL, consumer, NULL); // Tạo thread consumer

    pthread_join(tid1, NULL); // Chờ thread producer hoàn thành
    pthread_join(tid2, NULL); // Chờ thread consumer hoàn thành

    sem_destroy(&sem_producer); // Hủy semaphore của producer
    sem_destroy(&sem_consumer); // Hủy semaphore của consumer
    pthread_mutex_destroy(&lock); // Hủy mutex

    return 0;
}
