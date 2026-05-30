#include <iostream>
#include <cstring>

using namespace std;

struct Ngay {
    int ngay, thang, nam;
};

struct SinhVien {
    char maSV[10];
    char hoTen[50];
    int gioiTinh;
    Ngay ngaySinh;
    char diaChi[100];
    char lop[12];
    char khoa[7];
};

struct Node {
    SinhVien data;
    Node *link;
};

struct List {
    Node *first;
    Node *last;
};

// Cần hàm khởi tạo danh sách
void init(List &l) {
    l.first = l.last = NULL;
}

int so_sanh_masv(SinhVien a, SinhVien b) {
    return strcmp(a.maSV, b.maSV);
}

void sap_xep_thu_tu_sinh_vien(List &l, SinhVien a) {
    Node *p = new Node{a, NULL}; // Sửa sv thành a
    
    if (l.first == NULL) {
        l.first = l.last = p;
        return;
    }

    // Chèn vào đầu danh sách
    if (so_sanh_masv(a, l.first->data) < 0) {
        p->link = l.first;
        l.first = p;
        return;
    }

    Node *q = l.first;
    // Sửa While thành while
    while (q->link != NULL && so_sanh_masv(a, q->link->data) > 0) {
        q = q->link;
    }
    
    p->link = q->link;
    q->link = p;

    if (q == l.last) l.last = p;
}

bool cung_ngay(Ngay a, Ngay b) {
    return a.nam == b.nam && a.ngay == b.ngay && a.thang == b.thang;
}

// Thêm tham số List l vào hàm
void in_sv_cung_ngaySinh(List l) {
    bool tim_thay_tong_the = false;

    for (Node *p = l.first; p != NULL; p = p->link) {
        bool co_trung = false;
        for (Node *q = l.first; q != NULL; q = q->link) {
            if (p != q && cung_ngay(p->data.ngaySinh, q->data.ngaySinh)) {
                co_trung = true;
                tim_thay_tong_the = true;
                break;
            }
        }
        if (co_trung) {
            cout << "MSSV: " << p->data.maSV << " | Ho ten: " << p->data.hoTen 
                 << " | Ngay sinh: " << p->data.ngaySinh.ngay << "/" 
                 << p->data.ngaySinh.thang << "/" << p->data.ngaySinh.nam << endl;
        }
    }
    if (!tim_thay_tong_the) {
        cout << "Khong tim thay sinh vien cung ngay sinh" << endl;
    }
}

void loai_bo_sinh_vien_cung_ngay_sinh(List &l) {
    Node *curr = l.first;
    Node *prev = NULL;

    while (curr != NULL) {
        bool bi_trung = false;
        for (Node *temp = l.first; temp != NULL; temp = temp->link) {
            if (curr != temp && cung_ngay(curr->data.ngaySinh, temp->data.ngaySinh)) {
                bi_trung = true;
                break;
            }
        }

        if (bi_trung) {
            Node* node_xoa = curr;
            if (prev == NULL) {
                l.first = curr->link;
                curr = l.first;
            } else {
                prev->link = curr->link;
                curr = prev->link;
            }
            if (node_xoa == l.last) {
                l.last = prev;
            }
            delete node_xoa;
        } else {
            prev = curr;
            curr = curr->link;
        }
    }
}

void xuat_danh_sach(List l) {
    for (Node *p = l.first; p != NULL; p = p->link) {
        cout << p->data.maSV << " - " << p->data.hoTen << " (" 
             << p->data.ngaySinh.ngay << "/" << p->data.ngaySinh.thang << ")" << endl;
    }
}

int main() {
    List l;
    init(l);
    
    SinhVien sv1 = {"202414139", "Kieu Xuan Huan", 0, {29, 6, 2006}, "Ha Noi", "dien tu 11", "ET1"};
    SinhVien sv2 = {"202414100", "Nguyen Van An", 0, {29, 6, 2006}, "Ha Noi", "dien tu 11", "ET1"};
    SinhVien sv3 = {"202413424", "Do Dinh Dang", 0, {20, 10, 2006}, "Ha Noi", "dien tu 11", "ET1"};
    SinhVien sv4 = {"202417358", "Nguyen Thi Phuong", 1, {28, 2, 2006}, "Ha Noi", "dien tu 11", "ET1"};

    sap_xep_thu_tu_sinh_vien(l, sv1);
    sap_xep_thu_tu_sinh_vien(l, sv2);
    sap_xep_thu_tu_sinh_vien(l, sv3);
    sap_xep_thu_tu_sinh_vien(l, sv4);

    cout << "--- Danh sach ban dau (da sap xep MSSV) ---" << endl;
    xuat_danh_sach(l);

    cout << "\n--- Sinh vien co cung ngay sinh ---" << endl;
    in_sv_cung_ngaySinh(l);

    cout << "\n--- Danh sach sau khi loai bo sinh vien trung ngay sinh ---" << endl;
    loai_bo_sinh_vien_cung_ngay_sinh(l);
    xuat_danh_sach(l);

    return 0;
}