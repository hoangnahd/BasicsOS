#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Kiểm tra số lượng đối số
    if (argc < 2) {
        fprintf(stderr, "Sử dụng: %s <command>\n", argv[0]);
        return 1;
    }

    // Khai báo biến
    struct timeval start, end;
    pid_t pid;
    int status;

    // Lấy thời gian bắt đầu
    gettimeofday(&start, NULL);

    // Fork một tiến trình con
    pid = fork();

    if (pid < 0) {
        // Fork không thành công
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Tiến trình con: thực thi lệnh shell
        execl("/bin/sh", "sh", "-c", argv[1], (char *)NULL);
        // Nếu exec lỗi, in ra thông báo và thoát
        perror("exec");
        exit(1);
    } else {
        // Tiến trình cha: đợi tiến trình con kết thúc
        waitpid(pid, &status, 0);
        
        // Lấy thời gian kết thúc
        gettimeofday(&end, NULL);

        // Tính thời gian thực thi
        double exec_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

        // In ra thời gian thực thi
        printf("Thời gian thực thi: %.5f\n", exec_time);
    }

    return 0;
}
