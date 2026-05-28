#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 1. Định nghĩa cấu trúc của một nút (Node) trên cây
struct Node {
    int namSinh;
    struct Node* left;
    struct Node* right;
};

// Hàm tiện ích để tạo một nút mới
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->namSinh = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// ham them nut vao cay
// Trái <= , Phải >
struct Node* insert(struct Node* root, int value) {
    // Nếu cây (hoặc nhánh) đang rỗng, tạo nút mới tại đây
    if (root == NULL) {
        return createNode(value);
    }

    // So sánh giá trị để phân nhánh
    if (value <= root->namSinh) {
        // Nếu giá trị nhỏ hơn hoặc bằng nút hiện tại -> Sang TRÁI
        root->left = insert(root->left, value);
    } else {
        // Nếu giá trị lớn hơn nút hiện tại -> Sang PHẢI
        root->right = insert(root->right, value);
    }
    return root;
}

// ham tim gia tri va in tien trinh tung buoc
bool searchBST(struct Node* root, int value, int step) {
    // Trường hợp gốc: Nhánh rỗng (không tìm thấy)
    if (root == NULL) {
        printf("-> Buoc %d: Gap cay trong (Khong tim thay %d)\n", step, value);
        return false;
    }

    printf("-> Buoc %d: So sanh voi nut [%d]", step, root->namSinh);

    // Trường hợp tìm thấy giá trị bằng nhau
    if (value == root->namSinh) {
        printf(" => Tim thay %d!\n", value);
        
        // Vì có dấu '<=' nên các phần tử trùng nhau (nếu có) sẽ nằm ở nhánh TRÁI
        // Ta kiểm tra xem còn nút trùng nào ở ngay phía dưới không
        if (root->left != NULL && root->left->namSinh == value) {
            printf("   (Luu y: Co them sinh vien trung nam sinh o nhanh ben trai)\n");
            searchBST(root->left, value, step + 1);
        }
        return true;
    }

    // Di chuyển tiếp dựa theo giá trị
    if (value < root->namSinh) {
        printf(" -> Di sang TRAI\n");
        return searchBST(root->left, value, step + 1);
    } else {
        printf(" -> Di sang PHAI\n");
        return searchBST(root->right, value, step + 1);
    }
}

//ham duyet cay theo thu tu tang dan de kiem tra cau truc cay
void inOrder(struct Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->namSinh);
        inOrder(root->right);
    }
}

// giai phong bo nho cua cay khi ket thuc chuong trinh
void freeTree(struct Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main() {
    int danhSachNamSinh[] = {2001, 2002, 2006, 2007, 2003, 2004, 2005, 2001, 1999, 2004};
    int n = sizeof(danhSachNamSinh) / sizeof(danhSachNamSinh[0]);

    struct Node* root = NULL;

    // Bước 1 & Bước 2: Dựng cây nhị phân tìm kiếm từ dãy số
    printf("--- 1. Dang dung cay BST tu day so --- \n");
    for (int i = 0; i < n; i++) {
        root = insert(root, danhSachNamSinh[i]);
    }
    printf("Dung cay thanh cong!\n\n");

    // In thử cây theo thứ tự LNR để xác nhận xem các phần tử đã được sắp xếp chuẩn chưa
    printf("Danh sach sau khi duyet cay (In-order LNR): ");
    inOrder(root);
    printf("\n\n");

    // Bước 3: Tìm kiếm sinh viên sinh năm 2004
    int target = 2004;
    printf("--- 2. Tien hanh tim kiem sinh vien nam sinh %d ---\n", target);
    bool ketQua = searchBST(root, target, 1);

    if (!ketQua) {
        printf("Khong co sinh vien nao sinh nam %d trong he thong.\n", target);
    }

    // Giải phóng bộ nhớ trước khi thoát
    freeTree(root);
    return 0;
}
