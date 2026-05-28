#include<stdio.h>

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

