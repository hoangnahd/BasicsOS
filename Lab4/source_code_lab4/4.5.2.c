#include <stdio.h>
#include <string.h>

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
};

int main() {

    int x;
    struct process p[100];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilization;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    float throughput;
    int burst_remaining[100];
    int is_completed[100];
    memset(is_completed, 0, sizeof(is_completed)); // Khởi tạo mảng is_completed với giá trị 0.

    printf("Nhập số lượng tiến trình: ");
    scanf("%d", &x); // Nhập số lượng tiến trình.

    for (int i = 0; i < x; i++) {
        printf("Nhập thời gian đến của tiến trình %d: ", i + 1);
        scanf("%d", &p[i].arrival_time); // Nhập thời gian đến của tiến trình.
        printf("Nhập thời gian thực thi của tiến trình %d: ", i + 1);
        scanf("%d", &p[i].burst_time); // Nhập thời gian thực thi của tiến trình.
        p[i].pid = i + 1;
        burst_remaining[i] = p[i].burst_time;
        printf("\n");
    }

    int current_time = 0;
    int completed = 0;
    int prev = 0;

    while (completed != x) {
        int idx = -1;
        int mn = 10000000;
        for (int i = 0; i < x; i++) {
            if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (burst_remaining[i] < mn) {
                    mn = burst_remaining[i];
                    idx = i;
                }
                if (burst_remaining[i] == mn) {
                    if (p[i].arrival_time < p[idx].arrival_time) {
                        mn = burst_remaining[i];
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            if (burst_remaining[idx] == p[idx].burst_time) {
                p[idx].start_time = current_time;
                total_idle_time += p[idx].start_time - prev; // Tổng thời gian CPU đang không hoạt động.
            }
            burst_remaining[idx] -= 1;
            current_time++;
            prev = current_time;

            if (burst_remaining[idx] == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time; // Thời gian quay vòng của tiến trình.
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time; // Thời gian chờ đợi của tiến trình.
                p[idx].response_time = p[idx].start_time - p[idx].arrival_time; // Thời gian phản hồi của tiến trình.

                total_turnaround_time += p[idx].turnaround_time; // Tổng thời gian quay vòng của tất cả các tiến trình.
                total_waiting_time += p[idx].waiting_time; // Tổng thời gian chờ đợi của tất cả các tiến trình.
                total_response_time += p[idx].response_time; // Tổng thời gian phản hồi của tất cả các tiến trình.

                is_completed[idx] = 1; // Đánh dấu tiến trình đã hoàn thành.
                completed++; // Tăng số tiến trình đã hoàn thành.
            }
        } else {
            current_time++; // Tăng thời gian hiện tại.
        }
    }


    avg_turnaround_time = (float) total_turnaround_time / x; // Tính trung bình thời gian quay vòng.
    avg_waiting_time = (float) total_waiting_time / x; // Tính trung bình thời gian chờ đợi.
    avg_response_time = (float) total_response_time / x; // Tính trung bình thời gian phản hồi.

    printf("\n\n");
    printf("Process\tArrival Time\tBurst Time\tST\tCT\tTAT\tWT\tRT\n\n");

    for (int i = 0; i < x; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].start_time, p[i].completion_time, p[i].turnaround_time, p[i].waiting_time, p[i].response_time);
    }
    printf("Thời gian quay vòng trung bình = %.2f\n", avg_turnaround_time);
    printf("Thời gian chờ đợi trung bình = %.2f\n", avg_waiting_time);
    printf("Thời gian phản hồi trung bình = %.2f\n", avg_response_time);

    return 0;
}
