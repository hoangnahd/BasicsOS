#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>  // For file permission macros

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

void parse_command(char *input, char **args, char **input_file, char **output_file) {
    char *token;
    int i = 0;

    *input_file = NULL;
    *output_file = NULL;

    // Tách lệnh thành các tham số
    token = strtok(input, " ");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            // Xử lý toán tử chuyển hướng đầu vào
            token = strtok(NULL, " ");
            *input_file = token;
        } else if (strcmp(token, ">") == 0) {
            // Xử lý toán tử chuyển hướng đầu ra
            token = strtok(NULL, " ");
            *output_file = token;
        } else {
            // Lưu tham số khác vào mảng args
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

int main(void) {
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    char history[HISTORY_COUNT][MAX_LINE]; /* command history */
    int history_count = 0; /* number of commands in history */
    int should_run = 1; /* flag to determine when to exit program */
    int history_index = -1; /* index for navigating command history */

    enableRawMode();
    
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

            // Tách dòng lệnh thành các tham số và tìm các tệp chuyển hướng
            char *input_file = NULL;
            char *output_file = NULL;
            parse_command(input, args, &input_file, &output_file);

            // Kiểm tra nếu lệnh là "exit" thì thoát
            if (args[0] && strcmp(args[0], "exit") == 0) {
                should_run = 0;
                continue;
            }

            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                return 1;
            } else if (pid == 0) {
                // Chuyển hướng đầu vào
                if (input_file) {
                    int fd = open(input_file, O_RDONLY);
                    if (fd == -1) {
                        perror("open input file");
                        return 1;
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }

                // Chuyển hướng đầu ra
                if (output_file) {
                    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                    if (fd == -1) {
                        perror("open output file");
                        return 1;
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                if (execvp(args[0], args) == -1) {
                    perror("execvp failed");
                }
                return 1;
            } else {
                waitpid(pid, NULL, 0); // Đợi tiến trình con kết thúc
            }
        }
    }
    
    return 0;
}
