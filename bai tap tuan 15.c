#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INF 1e9
#define MAX_NODES 30
#define MAX_NAME 50

// Cấu trúc danh sách kề để lưu các tỉnh kề và khoảng cách
struct Edge {
    int neighbor_id;
    int weight;
    struct Edge* next;
};

struct Graph {
    char names[MAX_NODES][MAX_NAME];
    struct Edge* head[MAX_NODES];
    int num_nodes;
};

struct Graph* createGraph() {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->num_nodes = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->head[i] = NULL;
    }
    return graph;
}

int get_or_add_node(struct Graph* graph, const char* name) {
    for (int i = 0; i < graph->num_nodes; i++) {
        if (strcmp(graph->names[i], name) == 0) {
            return i;
        }
    }
    strcpy(graph->names[graph->num_nodes], name);
    graph->num_nodes++;
    return graph->num_nodes - 1;
}

void add_edge(struct Graph* graph, const char* src_name, const char* dest_name, int weight) {
    int src = get_or_add_node(graph, src_name);
    int dest = get_or_add_node(graph, dest_name);

    struct Edge* newNode1 = (struct Edge*)malloc(sizeof(struct Edge));
    newNode1->neighbor_id = dest;
    newNode1->weight = weight;
    newNode1->next = graph->head[src];
    graph->head[src] = newNode1;

    struct Edge* newNode2 = (struct Edge*)malloc(sizeof(struct Edge));
    newNode2->neighbor_id = src;
    newNode2->weight = weight;
    newNode2->next = graph->head[dest];
    graph->head[dest] = newNode2;
}

void dijkstra(struct Graph* graph, const char* start_name, const char* end_name) {
    int start = get_or_add_node(graph, start_name);
    int end = get_or_add_node(graph, end_name);
    int n = graph->num_nodes;

    int distances[MAX_NODES];
    int trace[MAX_NODES];
    bool visited[MAX_NODES];

    for (int i = 0; i < n; i++) {
        distances[i] = INF;
        trace[i] = -1;
        visited[i] = false;
    }
    distances[start] = 0;

    for (int i = 0; i < n - 1; i++) {
        int min_dist = INF;
        int u = -1;

        for (int j = 0; j < n; j++) {
            if (!visited[j] && distances[j] < min_dist) {
                min_dist = distances[j];
                u = j;
            }
        }

        if (u == -1 || u == end) break;
        visited[u] = true;

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

    printf("=== TÌM ĐƯỜNG ĐI NGẮN NHẤT TỪ %s ĐẾN %s ===\n", start_name, end_name);
    if (distances[end] == INF) {
        printf("❌ Không tìm thấy đường đi giữa hai tỉnh.\n");
        return;
    }

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

    add_edge(graph, "Hà nội", "Hải dương", 1);
    add_edge(graph, "Hải dương", "Hưng yên", 1);
    add_edge(graph, "Hưng yên", "Phủ lý", 1);
    add_edge(graph, "Hà nội", "Phủ lý", 1);
    add_edge(graph, "Hà nội", "Hòa bình", 1);
    add_edge(graph, "Hà nội", "Sơn tây", 1);
    add_edge(graph, "Hà nội", "Thái nguyên", 1);
    add_edge(graph, "Hà nội", "Bắc ninh", 1);
    add_edge(graph, "Bắc ninh", "Bắc giang", 1);
    add_edge(graph, "Bắc giang", "Uông bí", 1);
    add_edge(graph, "Bắc ninh", "Uông bí", 1);
    add_edge(graph, "Uông bí", "Hải phòng", 1);
    add_edge(graph, "Hải dương", "Hải phòng", 1);

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