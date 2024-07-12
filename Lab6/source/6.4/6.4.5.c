#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_LINE 80 /* The maximum length command */
#define HISTORY_COUNT 10 /* Number of commands to store in history */

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void parse_command(char *input, char **args) {
    char *token;
    int i = 0;

    token = strtok(input, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void handle_sigint(int sig) {
    printf("\n");
    fflush(stdout);
}

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    char history[HISTORY_COUNT][MAX_LINE]; /* command history */
    int history_count = 0; /* number of commands in history */
    int should_run = 1; /* flag to determine when to exit program */
    int history_index = -1; /* index for navigating command history */

    enableRawMode();
    signal(SIGINT, handle_sigint); // Đăng ký xử lý tín hiệu SIGINT
    
    while (should_run) {
        printf("it007sh> ");
        fflush(stdout);

        char input[MAX_LINE] = {0};
        int index = 0;

        // Đọc đầu vào từ người dùng
        while (1) {
            char c;
            if (read(STDIN_FILENO, &c, 1) == 1) {
                if (c == '\n') {
                    input[index] = '\0';
                    putchar('\n');
                    break;
                } else if (c == 27) {
                    char seq[3];
                    if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
                        if (seq[0] == '[') {
                            if (seq[1] == 'A') {
                                // Phím lên
                                if (history_index < history_count - 1) {
                                    history_index++;
                                    strcpy(input, history[history_count - history_index - 1]);
                                    index = strlen(input);
                                    printf("\33[2K\r"); // Xóa dòng hiện tại
                                    printf("it007sh> %s", input);
                                    fflush(stdout);
                                }
                            } else if (seq[1] == 'B') {
                                // Phím xuống
                                if (history_index > 0) {
                                    history_index--;
                                    strcpy(input, history[history_count - history_index - 1]);
                                    index = strlen(input);
                                    printf("\33[2K\r"); // Xóa dòng hiện tại
                                    printf("it007sh> %s", input);
                                    fflush(stdout);
                                } else if (history_index == 0) {
                                    history_index--;
                                    input[0] = '\0';
                                    index = 0;
                                    printf("\33[2K\r"); // Xóa dòng hiện tại
                                    printf("it007sh> ");
                                    fflush(stdout);
                                }
                            }
                        }
                    }
                } else {
                    if (index < MAX_LINE - 1) {
                        input[index++] = c;
                        putchar(c);
                        fflush(stdout);
                    }
                }
            }
        }

        // Xử lý lệnh
        if (strlen(input) > 0) {
            // Lưu lệnh vào lịch sử
            if (history_count < HISTORY_COUNT) {
                strcpy(history[history_count++], input);
            } else {
                for (int i = 1; i < HISTORY_COUNT; i++) {
                    strcpy(history[i - 1], history[i]);
                }
                strcpy(history[HISTORY_COUNT - 1], input);
            }
            history_index = -1; // Reset history index

            // Tách dòng lệnh thành các tham số
            parse_command(input, args);

            // Kiểm tra nếu lệnh là "exit" thì thoát
            if (strcmp(args[0], "exit") == 0) {
                should_run = 0;
                continue;
            }

            // Kiểm tra xem có dấu '|' trong lệnh hay không
            int pipe_index = -1;
            for (int i = 0; args[i] != NULL; i++) {
                if (strcmp(args[i], "|") == 0) {
                    pipe_index = i;
                    break;
                }
            }

            if (pipe_index != -1) {
                // Tạo pipe
                int pipefd[2];
                if (pipe(pipefd) == -1) {
                    perror("pipe");
                    return 1;
                }

                // Tạo tiến trình con 1
                pid_t pid1 = fork();
                if (pid1 < 0) {
                    perror("fork");
                    return 1;
                } else if (pid1 == 0) {
                    // Thiết lập đầu ra của tiến trình con 1 là đầu vào của pipe
                    close(pipefd[0]); // Đóng đầu đọc của pipe
                    dup2(pipefd[1], STDOUT_FILENO); // Thay đổi STDOUT thành đầu ra của pipe
                    close(pipefd[1]); // Đóng đầu ghi của pipe

                    // Thực thi lệnh trước dấu '|'
                    args[pipe_index] = NULL; // Kết thúc danh sách tham số ở đây
                    if (execvp(args[0], args) == -1) {
                        perror("execvp");
                        return 1;
                    }
                } else {
                    // Tiến trình cha đợi tiến trình con 1 kết thúc
                    waitpid(pid1, NULL, 0);

                    // Tạo tiến trình con 2
                    pid_t pid2 = fork();
                    if (pid2 < 0) {
                        perror("fork");
                        return 1;
                    } else if (pid2 == 0) {
                        // Thiết lập đầu vào của tiến trình con 2 là đầu ra của pipe
                        close(pipefd[1]); // Đóng đầu ghi của pipe
                        dup2(pipefd[0], STDIN_FILENO); // Thay đổi STDIN thành đầu vào của pipe
                        close(pipefd[0]); // Đóng đầu đọc của pipe

                        // Thực thi lệnh sau dấu '|'
                        if (execvp(args[pipe_index + 1], &args[pipe_index + 1]) == -1) {
                            perror("execvp");
                           
                            return 1;
                        }
                    } else {
                        // Tiến trình cha đóng pipe và đợi tiến trình con 2 kết thúc
                        close(pipefd[0]);
                        close(pipefd[1]);

                        waitpid(pid2, NULL, 0);
                    }
                }
            } else {
                // Không có dấu '|' trong lệnh, thực thi lệnh bình thường
                pid_t pid = fork();
                if (pid < 0) {
                    perror("fork");
                    return 1;
                } else if (pid == 0) {
                    // Tiến trình con
                    if (execvp(args[0], args) == -1) {
                        perror("execvp");
                        return 1;
                    }
                } else {
                    // Tiến trình cha đợi tiến trình con kết thúc
                    waitpid(pid, NULL, 0);
                }
            }
        }
    }

    return 0;
}
