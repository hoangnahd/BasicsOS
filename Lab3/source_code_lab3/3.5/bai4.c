#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 10

// Cấu trúc dữ liệu của buffer
typedef struct {
    int data[BUFFER_SIZE];
    int count;
    int sum;
} Buffer;

int main() {
    // Tạo key để tạo shared memory
    key_t key = ftok("buffer", 65);

    // Tạo segment của shared memory
    int shmid = shmget(key, sizeof(Buffer), IPC_CREAT | 0666);

    // Attach segment vào vùng nhớ
    Buffer *buffer = (Buffer *)shmat(shmid, NULL, 0);

    // Khởi tạo buffer
    buffer->count = 0;
    buffer->sum = 0;

    // Fork để tạo tiến trình con
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid == 0) {
        // Tiến trình con (Consumer)
        while (1) {
            // Kiểm tra điều kiện dừng
            if (buffer->sum > 100) {
                printf("Consumer: Tổng lớn hơn 100. Dừng lại...\n");
                break;
            }

            // Kiểm tra buffer trống
            if (buffer->count > 0) {
                // Đọc dữ liệu từ buffer
                int num = buffer->data[buffer->count - 1];
                buffer->count--;
                printf("Consumer consumed: %d\n", num);

                // Tính tổng
                buffer->sum += num;
            }

            // Ngừng một chút trước khi đọc dữ liệu mới từ buffer
            usleep(500000);
        }
    } else {
        // Tiến trình cha (Producer)
        while (1) {
            // Kiểm tra điều kiện dừng
            if (buffer->sum > 100) {
                printf("Producer: Tổng lớn hơn 100. Dừng lại...\n");
                break;
            }

            // Kiểm tra buffer đầy
            if (buffer->count < BUFFER_SIZE) {
                // Tạo số ngẫu nhiên từ 10 đến 20
                int num = rand() % 11 + 10;

                // Ghi dữ liệu vào buffer
                buffer->data[buffer->count] = num;
                buffer->count++;
                printf("Producer produced: %d\n", num);
            }

            // Ngừng một chút trước khi thêm số mới vào buffer
            usleep(500000);
        }

        // Chờ tiến trình con kết thúc
        wait(NULL);

        // Detach và xóa shared memory
        shmdt(buffer);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}