#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAX_LINE 80 /* The maximum length command */

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
    
    while (should_run) {
        printf("it007sh> ");
        fflush(stdout);

        char input[MAX_LINE];
        if (!fgets(input, MAX_LINE, stdin)) {
            // Xử lý lỗi khi đọc dòng lệnh
            perror("fgets failed");
            continue;
        }

        // Xóa ký tự newline ở cuối dòng nếu có
        size_t length = strlen(input);
        if (input[length - 1] == '\n') {
            input[length - 1] = '\0';
        }

        // Tách dòng lệnh thành các tham số
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Kiểm tra nếu lệnh là "exit" thì thoát
        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            // Xử lý lỗi khi tạo tiến trình con
            perror("Fork failed");
            return 1;
        } else if (pid == 0) {
            // Tiến trình con
            if (execvp(args[0], args) == -1) {
                perror("execvp failed");
            }
            return 1;
        } else {
            // Tiến trình cha
            waitpid(pid, NULL, 0); // Đợi tiến trình con kết thúc
        }
    }
    
    return 0;
}
