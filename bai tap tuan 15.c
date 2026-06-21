#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INF 1e9
#define MAX_NODES 20
#define MAX_NAME 50

// Cấu trúc danh sách kề để lưu các tỉnh kề và khoảng cách
struct Edge {
    int neighbor_id;
    int weight;
    struct Edge* next;
};

// Cấu trúc thông tin của một Tỉnh thành
struct Graph {
    char names[MAX_NODES][MAX_NAME];
    struct Edge* head[MAX_NODES];
    int num_nodes;
};

// Hàm khởi tạo đồ thị
struct Graph* createGraph() {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->num_nodes = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->head[i] = NULL;
    }
    return graph;
}

// Hàm thêm hoặc lấy ID của một tỉnh dựa vào tên
int get_or_add_node(struct Graph* graph, const char* name) {
    for (int i = 0; i < graph->num_nodes; i++) {
        if (strcmp(graph->names[i], name) == 0) {
            return i;
        }
    }
    // Nếu chưa có, thêm tỉnh mới vào danh sách
    strcpy(graph->names[graph->num_nodes], name);
    graph->num_nodes++;
    return graph->num_nodes - 1;
}

// Hàm thêm tuyến đường (cạnh) giữa 2 tỉnh thành (Đồ thị vô hướng)
void add_edge(struct Graph* graph, const char* src_name, const char* dest_name, int weight) {
    int src = get_or_add_node(graph, src_name);
    int dest = get_or_add_node(graph, dest_name);

    // Thêm từ src -> dest
    struct Edge* newNode1 = (struct Edge*)malloc(sizeof(struct Edge));
    newNode1->neighbor_id = dest;
    newNode1->weight = weight;
    newNode1->next = graph->head[src];
    graph->head[src] = newNode1;

    // Thêm từ dest -> src (vì đường đi 2 chiều)
    struct Edge* newNode2 = (struct Edge*)malloc(sizeof(struct Edge));
    newNode2->neighbor_id = src;
    newNode2->weight = weight;
    newNode2->next = graph->head[dest];
    graph->head[dest] = newNode2;
}

// Thuật toán Dijkstra
void dijkstra(struct Graph* graph, const char* start_name, const char* end_name) {
    int start = get_or_add_node(graph, start_name);
    int end = get_or_add_node(graph, end_name);
    int n = graph->num_nodes;

    int distances[MAX_NODES];
    int trace[MAX_NODES];
    bool visited[MAX_NODES];

    // Khởi tạo các mảng trạng thái
    for (int i = 0; i < n; i++) {
        distances[i] = INF;
        trace[i] = -1;
        visited[i] = false;
    }
    distances[start] = 0;

    // Vòng lặp chính của Dijkstra
    for (int i = 0; i < n - 1; i++) {
        // Tìm tỉnh chưa duyệt có khoảng cách ngắn nhất hiện tại
        int min_dist = INF;
        int u = -1;

        for (int j = 0; j < n; j++) {
            if (!visited[j] && distances[j] < min_dist) {
                min_dist = distances[j];
                u = j;
            }
        }

        // Nếu không tìm thêm được đường đi khả thi hoặc đã đến đích, dừng lại
        if (u == -1 || u == end) break;
        visited[u] = true;

        // Cập nhật khoảng cách cho các tỉnh kề với u
        struct Edge* curr = graph->head[u];
        while (curr != NULL) {
            int v = curr->neighbor_id;
            int weight = curr->weight;

            if (!visited[v] && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                trace[v] = u;
            }
            curr = curr->next;
        }
    }

    // In kết quả
    printf("=== TÌM ĐƯỜNG ĐI NGẮN NHẤT TỪ %s ĐẾN %s ===\n", start_name, end_name);
    if (distances[end] == INF) {
        printf("❌ Không tìm thấy đường đi giữa hai tỉnh.\n");
        return;
    }

    // Tái tạo lộ trình bằng mảng lưu vết (trace)
    int path[MAX_NODES];
    int count = 0;
    int curr_node = end;

    while (curr_node != -1) {
        path[count++] = curr_node;
        curr_node = trace[curr_node];
    }

    printf("👉 Lộ trình tối ưu: ");
    for (int i = count - 1; i >= 0; i--) {
        printf("%s", graph->names[path[i]]);
        if (i > 0) printf(" -> ");
    }
    printf("\nTotal quãng đường: %d km\n", distances[end]);
}

int main() {
    struct Graph* graph = createGraph();

    // Nạp dữ liệu bản đồ các tỉnh thành vào đồ thị
    add_edge(graph, "Hà Nội", "Hải Phòng", 120);
    add_edge(graph, "Hà Nội", "Ninh Bình", 95);
    add_edge(graph, "Hải Phòng", "Hạ Long", 70);
    add_edge(graph, "Ninh Bình", "Thanh Hóa", 60);
    add_edge(graph, "Thanh Hóa", "Vinh", 140);
    add_edge(graph, "Vinh", "Đà Nẵng", 460);
    add_edge(graph, "Đà Nẵng", "Nha Trang", 530);
    add_edge(graph, "Nha Trang", "TP.HCM", 430);

    // Chạy thử nghiệm
    dijkstra(graph, "Hà Nội", "Vinh");

    // Giải phóng bộ nhớ (Giải phóng con trỏ Linked List)
    for (int i = 0; i < graph->num_nodes; i++) {
        struct Edge* curr = graph->head[i];
        while (curr != NULL) {
            struct Edge* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(graph);

    return 0;
}