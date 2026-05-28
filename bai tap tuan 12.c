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


int main(){

}
