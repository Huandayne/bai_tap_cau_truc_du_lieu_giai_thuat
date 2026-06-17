#include <stdio.h>

#define MAX_NODES 100

// Định nghĩa cấu trúc một nút trong cây AVL
struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height;
};

// Cấp phát tĩnh thay cho malloc
struct Node nodePool[MAX_NODES];
int nodeCount = 0;

// Hàm tạo một nút mới
struct Node* createNode(int key) {
    struct Node* newNode = &nodePool[nodeCount++];
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // Nút mới thêm vào tạm thời có chiều cao là 1
    return newNode;
}

// Hàm lấy chiều cao của nút
int getHeight(struct Node* n) {
    if (n == NULL) return 0;
    return n->height;
}

// Hàm lấy giá trị lớn hơn giữa 2 số
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Hàm tính độ cân bằng (Balance Factor) của nút
int getBalance(struct Node* n) {
    if (n == NULL) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

// ==================== CÁC PHÉP XOAY CÂN BẰNG ====================

// Phép xoay phải (Right Rotate - dùng khi mất cân bằng Trái-Trái / LL)
struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    // Thực hiện xoay
    x->right = y;
    y->left = T2;

    // Cập nhật lại chiều cao
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x; // Gốc mới
}

// Phép xoay trái (Left Rotate - dùng khi mất cân bằng Phải-Phải / RR)
struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    // Thực hiện xoay
    y->left = x;
    x->right = T2;

    // Cập nhật lại chiều cao
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y; // Gốc mới
}

// ==================== HÀM THÊM PHẦN TỬ (INSERT) ====================
struct Node* insert(struct Node* node, int key) {
    // 1. Thực hiện thêm nút như cây BST thông thường
    if (node == NULL) {
        return createNode(key);
    }

    if (key < node->key) {
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        node->right = insert(node->right, key);
    } else {
        return node; // Không thêm phần tử trùng lặp
    }

    // 2. Cập nhật chiều cao của nút tổ tiên này
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // 3. Lấy độ cân bằng để kiểm tra xem nút này có bị mất cân bằng không
    int balance = getBalance(node);

    // Trường hợp 1: Trái Trái (LL)
    if (balance > 1 && key < node->left->key) {
        return rightRotate(node);
    }

    // Trường hợp 2: Phải Phải (RR)
    if (balance < -1 && key > node->right->key) {
        return leftRotate(node);
    }

    // Trường hợp 3: Trái Phải (LR)
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Trường hợp 4: Phải Trái (RL)
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // Trả về con trỏ nút (không đổi nếu đã cân bằng)
}

// ==================== CÁC PHÉP DUYỆT CÂY ====================

// Duyệt Tiền thứ tự (NLR - Root, Left, Right)
void preOrder(struct Node* root) {
    if (root != NULL) {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

// Duyệt Trung thứ tự (LNR - Left, Root, Right -> In ra dãy tăng dần)
void inOrder(struct Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

// Duyệt Hậu thứ tự (LRN - Left, Right, Root)
void postOrder(struct Node* root) {
    if (root != NULL) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->key);
    }
}

// ==================== HÀM MAIN ====================
int main() {
    struct Node* root = NULL;

    // Dãy số đầu vào theo yêu cầu của bạn
    int inputArray[] = {32, 51, 27, 83, 96, 11, 45, 75, 66, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = sizeof(inputArray) / sizeof(inputArray[0]);

    printf("Tien hanh them cac phan tu vao cay AVL...\n\n");
    for (int i = 0; i < n; i++) {
        root = insert(root, inputArray[i]);
    }

    // In kết quả theo các thứ tự duyệt
    printf("1. Duyet TRUNG THU TU (Inorder - LNR):\n");
    printf("   -> Chon loc tang dan: ");
    inOrder(root);
    printf("\n\n");

    printf("2. Duyet TIEN THU TU (Preorder - NLR):\n");
    printf("   -> Cau truc cay: ");
    preOrder(root);
    printf("\n\n");

    printf("3. Duyet HAU THU TU (Postorder - LRN):\n");
    printf("   -> ");
    postOrder(root);
    printf("\n");

    return 0;
}