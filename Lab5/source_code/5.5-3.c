#include <stdio.h>
#include <pthread.h>

int x = 0; // Biến x toàn cục được sử dụng bởi cả hai tiến trình
pthread_mutex_t mutex; // Mutex để đồng bộ hóa
// Hàm thực thi cho tiến trình A
void *processA(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("Process A: %d\n", x);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Hàm thực thi cho tiến trình B
void *processB(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("Process B: %d\n", x);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t tidA, tidB;

    pthread_mutex_init(&mutex, NULL); // Khởi tạo mutex

    // Tạo tiến trình A
    pthread_create(&tidA, NULL, processA, NULL);

    // Tạo tiến trình B
    pthread_create(&tidB, NULL, processB, NULL);

    // Chờ tiến trình A và tiến trình B hoàn thành
    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);

    pthread_mutex_destroy(&mutex); // Hủy mutex

    return 0;
}
