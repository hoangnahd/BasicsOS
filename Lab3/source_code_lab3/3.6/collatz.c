#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

// Cấu trúc dữ liệu của buffer
typedef struct {
    int data[BUFFER_SIZE];
    int count;
} Buffer;

// Kiểm tra xem số nguyên nhập vào có hợp lệ không
int isValidInput(char *input) {
    while (*input != '\0') {
        if (*input < '0' || *input > '9')
            return 0;
        input++;
    }
    return 1;
}

// Tính toán chuỗi Collatz và ghi vào buffer
void collatzSequence(int n, Buffer *buffer) {
    buffer->count = 0;
    while (n != 1) {
        buffer->data[buffer->count++] = n;
        if (n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;
    }
    buffer->data[buffer->count++] = 1; // Đặc biệt, kết thúc chuỗi với 1
}

int main(int argc, char *argv[]) {
    // Kiểm tra số lượng đối số dòng lệnh
    if (argc != 2 || !isValidInput(argv[1])) {
        printf("Input phải là số nguyên dương\n");
        return 1;
    }

    // Chuyển đổi đối số dòng lệnh thành số nguyên
    int startNumber = atoi(argv[1]);
    if (startNumber <= 0) {
        printf("Input phải là số nguyên dương\n");
        return 1;
    }

    // Tạo key để tạo shared memory
    key_t key = ftok("buffer", 65);

    // Tạo segment của shared memory
    int shmid = shmget(key, sizeof(Buffer), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    // Attach segment vào vùng nhớ
    Buffer *buffer = (Buffer *)shmat(shmid, NULL, 0);
    if (buffer == (void *)-1) {
        perror("shmat");
        return 1;
    }

    // Tạo tiến trình con
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Tiến trình con: tính toán chuỗi Collatz
        collatzSequence(startNumber, buffer);
        shmdt(buffer); // Detach shared memory
    } else {
        // Tiến trình cha: đợi tiến trình con kết thúc
        wait(NULL);

        // In kết quả từ buffer ra màn hình
        for (int i = 0; i < buffer->count; ++i) {
            printf("%d ", buffer->data[i]);
        }
        printf("\n");

        // Xóa shared memory
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
