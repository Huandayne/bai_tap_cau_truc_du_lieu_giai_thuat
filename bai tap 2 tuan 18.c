#include <stdio.h>
#include <stdbool.h>

#define INF 1e9
#define MAX 20 // Tăng kích thước tối đa để chứa đủ 11 tỉnh thành

typedef struct {
    int u, v, weight;
} Edge;



// Hàm in ma trận kề của Cây Khung Nhỏ Nhất (MST) kèm tên tỉnh thành
void printMSTMatrix(int V, int parent[], int graph[MAX][MAX], char* names[]) {
    int mstMatrix[MAX][MAX] = {0};
    int totalWeight = 0;

    for (int i = 1; i < V; i++) {
        if (parent[i] != -1) {
            int u = parent[i];
            int v = i;
            int w = graph[u][v];
            mstMatrix[u][v] = w;
            mstMatrix[v][u] = w;
            totalWeight += w;
        }
    }

    printf("\nTong trong so cay khung (MST): %d\n", totalWeight);
    printf("[Ma tran ke cua Cay Khung Nho Nhat (MST)]:\n");
    
    // In tiêu đề cột bằng ID số
    printf("%-15s", "");
    for (int i = 0; i < V; i++) printf("%5d", i);
    printf("\n");

    for (int i = 0; i < V; i++) {
        printf("%-12s [%d] |", names[i], i);
        for (int j = 0; j < V; j++) {
            printf("%5d", mstMatrix[i][j]);
        }
        printf("\n");
    }
}

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

// 2. THUẬT TOÁN PRIM
void primMST(int V, int graph[MAX][MAX], char* names[]) {
    int parent[MAX]; 
    int key[MAX];    
    bool inMST[MAX] = {false};

    for (int i = 0; i < V; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    key[0] = 0; 

    for (int count = 0; count < V - 1; count++) {
        int min_key = INF, u = -1;
        for (int v = 0; v < V; v++) {
            if (!inMST[v] && key[v] < min_key) {
                min_key = key[v];
                u = v;
            }
        }

        if (u == -1) break;
        inMST[u] = true;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !inMST[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    printf("\n=== KET QUA THUAT TOAN PRIM ===");
    printMSTMatrix(V, parent, graph, names);
}


/ 3. THUẬT TOÁN KRUSKAL
int findRoot(int parent[], int i) {
    if (parent[i] == -1) return i;
    return findRoot(parent, parent[i]);
}

void kruskalMST(int V, int E, Edge edges[], int graph[MAX][MAX], char* names[]) {
    // Sắp xếp các cạnh (Bubble Sort)
    for (int i = 0; i < E - 1; i++) {
        for (int j = 0; j < E - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    int parentArray[MAX];
    for (int i = 0; i < V; i++) parentArray[i] = -1;

    int mstParent[MAX]; 
    for (int i = 0; i < V; i++) mstParent[i] = -1;

    int edgeCount = 0;
    int i = 0;

    while (edgeCount < V - 1 && i < E) {
        Edge next_edge = edges[i++];
        int x = findRoot(parentArray, next_edge.u);
        int y = findRoot(parentArray, next_edge.v);

        if (x != y) {
            mstParent[next_edge.v] = next_edge.u; 
            parentArray[x] = y; 
            edgeCount++;
        }
    }

    printf("\n=== KET QUA THUAT TOAN KRUSKAL ===");
    printMSTMatrix(V, mstParent, graph, names);
}



int main() {
    int V = 11; // 11 tỉnh thành
    int graph[MAX][MAX] = {0};

    // Định nghĩa ID cho từng đỉnh
    char* names[] = {
        "Ha Noi", "Hai Duong", "Hung Yen", "Phu Ly", "Hoa Binh", 
        "Son Tay", "Thai Nguyen", "Bac Ninh", "Bac Giang", "Uong Bi", "Hai Phong"
    };

    // Khai báo các cạnh: {Đỉnh u, Đỉnh v, Trọng số = Giá trị số sau chữ D}
    int sampleEdges[][3] = {
        {0, 1, 1},   // D1: Ha Noi - Hai Duong (Trọng số = 1)
        {1, 2, 2},   // D2: Hai Duong - Hung Yen (Trọng số = 2)
        {3, 2, 3},   // D3: Phu Ly - Hung Yen (Trọng số = 3)
        {0, 3, 4},   // D4: Ha Noi - Phu Ly (Trọng số = 4)
        {0, 4, 5},   // D5: Ha Noi - Hoa Binh (Trọng số = 5)
        {0, 5, 6},   // D6: Ha Noi - Son Tay (Trọng số = 6)
        {0, 6, 7},   // D7: Ha Noi - Thai Nguyen (Trọng số = 7)
        {0, 7, 8},   // D8: Ha Noi - Bac Ninh (Trọng số = 8)
        {7, 8, 9},   // D9: Bac Ninh - Bac Giang (Trọng số = 9)
        {8, 9, 10},  // D10: Bac Giang - Uong Bi (Trọng số = 10)
        {7, 9, 11},  // D11: Bac Ninh - Uong Bi (Trọng số = 11)
        {9, 10, 12}, // D12: Uong Bi - Hai Phong (Trọng số = 12)
        {1, 10, 13}  // D13: Hai Duong - Hai Phong (Trọng số = 13)
    };
    int E = sizeof(sampleEdges) / sizeof(sampleEdges[0]);

    Edge edgeList[MAX];

    // Đổ dữ liệu vào ma trận kề gốc
    for (int i = 0; i < E; i++) {
        int u = sampleEdges[i][0];
        int v = sampleEdges[i][1];
        int w = sampleEdges[i][2];
        
        graph[u][v] = w;
        graph[v][u] = w; 

        edgeList[i].u = u;
        edgeList[i].v = v;
        edgeList[i].weight = w;
    }

    // 1. Chạy Dijkstra từ Hà Nội (ID = 0)
    dijkstra(V, graph, 0, names);
    printf("\n========================================================================\n");

    // 2. Chạy thuật toán Prim để tìm và in cây khung nhỏ nhất
    primMST(V, graph, names);
    printf("\n========================================================================\n");

    // 3. Chạy thuật toán Kruskal để kiểm chứng lại cây khung nhỏ nhất
    kruskalMST(V, E, edgeList, graph, names);

    return 0;
}