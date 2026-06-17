#include <stdio.h>

#define V 5

const char* citiesList[V] = {"Ha Noi", "Hai Phong", "Da Nang", "TP.HCM", "Can Tho"};

// Định nghĩa cấu trúc nút cho Danh sách liên kết
struct Node {
    int dest;
    struct Node* next;
};

// Mảng tĩnh chứa các Node để cấp phát thủ công (thay thế cho malloc)
struct Node nodePool[V * 4]; 
int nodeCount = 0;

// Hàm tạo một Node mới từ bộ nhớ pool tĩnh
struct Node* createNode(int dest) {
    struct Node* newNode = &nodePool[nodeCount++];
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// Hàm thêm tuyến đường giao thông (cạnh) vào đồ thị vô hướng
void addEdge(struct Node* adj[], int u, int v) {
    // Thêm v vào danh sách của u
    struct Node* newNode = createNode(v);
    newNode->next = adj[u];
    adj[u] = newNode;

    // Thêm u vào danh sách của v (Đồ thị vô hướng)
    newNode = createNode(u);
    newNode->next = adj[v];
    adj[v] = newNode;
}

// Hàm duyệt BFS bằng Danh sách kề
void BFS_AdjList(struct Node* adj[], int startNode) {
    int visited[V] = {0};
    int queue[V];
    int front = 0, rear = 0;

    visited[startNode] = 1;
    queue[rear++] = startNode;

    printf("BFS (Danh sach ke): ");

    while (front < rear) {
        int u = queue[front++];
        printf("%s -> ", citiesList[u]);

        // Duyệt qua danh sách liên kết các đỉnh kề của u
        struct Node* temp = adj[u];
        while (temp != NULL) {
            int v = temp->dest;
            if (!visited[v]) {
                visited[v] = 1;
                queue[rear++] = v;
            }
            temp = temp->next; // Chuyển sang nút tiếp theo
        }
    }
    printf("End\n");
}

int main() {
    // Khởi tạo mảng các con trỏ danh sách kề bằng NULL
    struct Node* adj[V];
    for (int i = 0; i < V; i++) {
        adj[i] = NULL;
    }

    // Thiết lập các tuyến đường
    addEdge(adj, 0, 1); // Ha Noi - Hai Phong
    addEdge(adj, 0, 2); // Ha Noi - Da Nang
    addEdge(adj, 2, 3); // Da Nang - TP.HCM
    addEdge(adj, 3, 4); // TP.HCM - Can Tho

    // Duyệt từ Hà Nội (ID là 0)
    BFS_AdjList(adj, 0);

    return 0;
}