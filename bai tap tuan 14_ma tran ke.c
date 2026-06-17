#include <stdio.h>

#define V 5 // Số lượng thành phố

// Mảng ánh xạ ID sang tên thành phố
const char* cities[V] = {"Ha Noi", "Hai Phong", "Da Nang", "TP.HCM", "Can Tho"};

// Hàm duyệt BFS bằng Ma trận kề
void BFS_Matrix(int matrix[V][V], int startNode) {
    int visited[V] = {0}; // Mảng đánh dấu đỉnh đã thăm (0: chưa, 1: rồi)
    
    // Tự cài đặt Hàng đợi (Queue) bằng mảng
    int queue[V];
    int front = 0, rear = 0;

    // Đưa đỉnh xuất phát (Hà Nội) vào queue
    visited[startNode] = 1;
    queue[rear++] = startNode;

    printf("BFS (Ma tran ke): ");

    while (front < rear) {
        // Lấy phần tử ra khỏi queue
        int u = queue[front++];
        printf("%s -> ", cities[u]);

        // Tìm các đỉnh v kề với u
        for (int v = 0; v < V; v++) {
            if (matrix[u][v] == 1 && !visited[v]) {
                visited[v] = 1;
                queue[rear++] = v; // Đẩy vào queue
            }
        }
    }
    printf("End\n");
}

int main() {
    // Khởi tạo ma trận kề cho mạng lưới giao thông
    // 1: có đường đi, 0: không có đường đi
    int graphMatrix[V][V] = {
        {0, 1, 1, 0, 0}, // Ha Noi (kề với Hai Phong, Da Nang)
        {1, 0, 0, 0, 0}, // Hai Phong
        {1, 0, 0, 1, 0}, // Da Nang (kề với Ha Noi, TP.HCM)
        {0, 0, 1, 0, 1}, // TP.HCM (kề với Da Nang, Can Tho)
        {0, 0, 0, 1, 0}  // Can Tho
    };

    // Duyệt từ Hà Nội (ID là 0)
    BFS_Matrix(graphMatrix, 0);

    return 0;
}