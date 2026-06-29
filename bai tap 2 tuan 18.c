#include <stdio.h>
#include <stdbool.h>

#define INF 1e9
#define MAX 20 // Tăng kích thước tối đa để chứa đủ 11 tỉnh thành

typedef struct {
    int u, v, weight;
} Edge;

/// 1. THUẬT TOÁN DIJKSTRA
void dijkstra(int V, int graph[MAX][MAX], int src, char* names[]) {
    int dist[MAX];
    bool visited[MAX] = {false};

    for (int i = 0; i < V; i++) dist[i] = INF;
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int min_dist = INF, u = -1;
        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printf("\n=== KET QUA DIJKSTRA (Tu: %s) ===\n", names[src]);
    for (int i = 0; i < V; i++) {
        printf("Duong di ngan nhat den %-15s: ", names[i]);
        if (dist[i] == INF) printf("INF\n");
        else printf("%d\n", dist[i]);
    }
}

