#include<stdio.h>

int main() {
  int time, burst_time[10], at[10], sum_burst_time = 0, smallest, n, i;
  int sumt = 0, sumw = 0;
  printf("Nhập số lượng tiến trình: ");
  scanf("%d", &n); // Yêu cầu người dùng nhập số lượng tiến trình và lưu vào biến 'n'.
  for (i = 0; i < n; i++) {
    printf("Thời gian đến của tiến trình P%d: ", i + 1);
    scanf("%d", &at[i]); // Yêu cầu người dùng nhập thời gian đến cho mỗi tiến trình và lưu vào mảng 'at'.
    printf("Thời gian thực thi của tiến trình P%d: ", i + 1);
    scanf("%d", &burst_time[i]); // Yêu cầu người dùng nhập thời gian thực thi cho mỗi tiến trình và lưu vào mảng 'burst_time'.
    sum_burst_time += burst_time[i]; // Tính tổng thời gian thực thi của tất cả các tiến trình.
  }
  burst_time[9] = 9999; // Đặt thời gian thực thi của tiến trình cuối cùng thành một giá trị cao để so sánh.
  for (time = 0; time < sum_burst_time;) {
    smallest = 9; // Khởi tạo chỉ số nhỏ nhất là chỉ số của tiến trình cuối cùng.
    for (i = 0; i < n; i++) {
      if (at[i] <= time && burst_time[i] > 0 && burst_time[i] < burst_time[smallest])
        smallest = i; // Tìm tiến trình có thời gian thực thi nhỏ nhất đã đến và chưa thực thi.
    }
    printf("P[%d]\t|\t%d\t|\t%d\n", smallest + 1, time + burst_time[smallest] -at[smallest], time - at[smallest]); // In ra thông tin của tiến trình đang thực thi.
    sumt += time + burst_time[smallest] - at[smallest]; // Cập nhật tổng thời gian chờ đợi.
    sumw += time - at[smallest]; // Cập nhật tổng thời gian chờ đợi.
    time += burst_time[smallest]; // Cập nhật thời gian hiện tại sau khi thực thi tiến trình.
    burst_time[smallest] = 0; // Đặt thời gian thực thi của tiến trình đã thực thi thành 0.
  }
  printf("\n\n average waiting time = %f", sumw * 1.0 / n);
  printf("\n\n average turnaround time = %f", sumt * 1.0 / n);
  return 0;
}

