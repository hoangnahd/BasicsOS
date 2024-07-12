#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    char buffer[20];

    if (pipe(fd) == -1) {
        perror("pipe failed");
        return 1;
    }

    if (fork() == 0) {
        // Tiến trình con
        close(fd[0]); // Đóng đầu đọc
        write(fd[1], "Hello from child", 16);
        close(fd[1]);
    } else {
        // Tiến trình cha
        close(fd[1]); // Đóng đầu ghi
        read(fd[0], buffer, 16);
        printf("Parent read: %s\n", buffer);
        close(fd[0]);
    }

    return 0;
}
