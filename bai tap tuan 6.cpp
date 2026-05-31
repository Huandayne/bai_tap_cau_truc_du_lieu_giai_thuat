#include <iostream>
#include <string>

using namespace std;
#toi bi ngu
struct File {
    string name;
    double size;
    double date;
};

struct Node {
    File data;
    Node* next;
};

struct List {
    Node* head;
};

void init(List &l) {
    l.head = NULL;
}

Node* createNode(File x) {
    Node* p = new Node;
    p->data = x;
    p->next = NULL;
    return p;
}

void insertSorted(List &l, File x) {
    Node* newNode = createNode(x);
    if (l.head == NULL || l.head->data.date >= x.date) {
        newNode->next = l.head;
        l.head = newNode;
        return;
    }
    Node* current = l.head;
    while (current->next != NULL && current->next->data.date < x.date) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

double calculateTotalSize(List l) {
    double total = 0;
    Node* p = l.head;
    while (p != NULL) {
        total += p->data.size;
        p = p->next;
    }
    return total;
}

void removeSmallestFile(List &l) {
    if (l.head == NULL) return;

    Node *minNode = l.head, *preMin = NULL;
    Node *curr = l.head, *pre = NULL;
    double minSize = l.head->data.size;

    while (curr != NULL) {
        if (curr->data.size < minSize) {
            minSize = curr->data.size;
            minNode = curr;
            preMin = pre;
        }
        pre = curr;
        curr = curr->next;
    }

    if (minNode == l.head) {
        l.head = l.head->next;
    } else {
        preMin->next = minNode->next;
    }
    delete minNode;
}

void backToRemove(List &l, double Capacity = 32.0) {
    while (calculateTotalSize(l) > Capacity) {
        removeSmallestFile(l);
    }
}