#include <stdio.h>
#include <unistd.h>

int main() {
    char *args[] = {"ls", "-l", NULL};
    execvp("ls", args);
    
    // Nếu exec thành công, đoạn code dưới sẽ không bao giờ được thực thi
    perror("execvp failed");
    return 1;
}
