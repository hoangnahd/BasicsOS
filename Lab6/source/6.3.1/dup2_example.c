#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int old_fd = open("file.txt", O_RDONLY); // Mở file.txt và gán mô tả tệp cho old_fd
    int new_fd = 10; // Định nghĩa mô tả tệp mới

    if (dup2(old_fd, new_fd) == -1) {
        perror("dup2 failed");
        return 1;
    }

    // Bây giờ, new_fd có thể được sử dụng để đọc từ file.txt
    close(old_fd);
    // Sử dụng new_fd để thực hiện các thao tác I/O
    // Không tạo tệp mới trên hệ thống, chỉ sao chép mô tả tệp
    char buffer[100];
    read(new_fd, buffer, sizeof(buffer)); // Đọc từ new_fd
    printf("Content: %s\n", buffer);

    close(new_fd); // Đóng mô tả tệp mới

    return 0;
}
