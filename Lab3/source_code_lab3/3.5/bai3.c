#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void handler() {

    printf("count.sh has stopped\n");
    exit(0);
    
}
int main() {
    // In ra dòng chữ chào mừng
    printf("Welcome to IT007, I am 22520041!\n");

    // Đăng ký signal handler để bắt SIGINT (CTRL+C)
    signal(SIGINT, handler);

    // Thực thi count.sh với số lần đếm là 120
    system("./count.sh 120");

    return 0;
}
