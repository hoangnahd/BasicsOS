#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>

int sells = 0; // Biến đếm số lượng bán
int products = 0; // Biến đếm số lượng sản phẩm
sem_t sem, sem1; // Khai báo hai semaphore
pthread_mutex_t lock; // Khai báo mutex để khóa
bool stop = false; // Biến dừng quá trình

// Hàm xử lý cho processA
void *processA(void *mess)
{
    while (1)
    {
        sem_wait(&sem); // Chờ sem được giải phóng
        pthread_mutex_lock(&lock); // Khóa mutex
        if(stop)
        {
            pthread_mutex_unlock(&lock); // Mở khóa mutex
            sem_post(&sem1); // Giải phóng sem1
            break; // Thoát vòng lặp
        }

        sells++; // Tăng biến đếm số lượng bán
        if (sells == 41*2) // Kiểm tra nếu số lượng bán đạt 82
        {
            stop = true; // Dừng quá trình
        }
        pthread_mutex_unlock(&lock); // Mở khóa mutex

        printf("Sell = %d\n", sells); // In ra số lượng bán hiện tại
        printf("Sell1 = %d\n", sells + 41); // In ra số lượng bán cộng 41

        sem_post(&sem1); // Giải phóng sem1
    }
    return NULL;
}

// Hàm xử lý cho processB
void *processB(void *mess)
{
    while (1)
    {
        sem_wait(&sem1); // Chờ sem1 được giải phóng

        pthread_mutex_lock(&lock); // Khóa mutex
        if (stop)
        {
            pthread_mutex_unlock(&lock); // Mở khóa mutex
            sem_post(&sem); // Giải phóng sem
            break; // Thoát vòng lặp
        }

        if (products <= sells + 41)  // Đảm bảo sản phẩm <= số lượng bán + 41
        {
            products++; // Tăng biến đếm sản phẩm
            pthread_mutex_unlock(&lock); // Mở khóa mutex

            printf("Products = %d\n", products); // In ra số lượng sản phẩm

            sem_post(&sem); // Giải phóng sem
        }
        else
        {
            pthread_mutex_unlock(&lock); // Mở khóa mutex
            sem_post(&sem); // Giải phóng sem
        }
    }
    return NULL;
}

int main()
{
    sem_init(&sem, 0, 0); // Khởi tạo semaphore sem
    sem_init(&sem1, 0, sells+41); // Khởi tạo semaphore sem1
    pthread_mutex_init(&lock, NULL); // Khởi tạo mutex

    pthread_t pA, pB;
    pthread_create(&pA, NULL, &processA, NULL); // Tạo thread cho processA
    pthread_create(&pB, NULL, &processB, NULL); // Tạo thread cho processB

    pthread_join(pA, NULL); // Chờ thread pA hoàn thành
    pthread_join(pB, NULL); // Chờ thread pB hoàn thành

    sem_destroy(&sem); // Hủy semaphore sem
    sem_destroy(&sem1); // Hủy semaphore sem1
    pthread_mutex_destroy(&lock); // Hủy mutex

    return 0;
}
