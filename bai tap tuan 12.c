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

int main(){

}
