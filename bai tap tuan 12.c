#include <iostream>	 

 

using namespace std; 

 

void inMang(int A[], int n) { 

    for (int i = 1; i <= n; i++) { 

        cout << A[i] << " "; 

    } 

    cout << "\n"; 

} 

 

void vunDongTaiNut(int A[], int n, int i) { 

    int lonNhat = i; 

    int conTrai = 2 * i; 

    int conPhai = 2 * i + 1; 

 

    if (conTrai <= n && A[conTrai] > A[lonNhat]) { 

        lonNhat = conTrai; 

    } 

 

    if (conPhai <= n && A[conPhai] > A[lonNhat]) { 

        lonNhat = conPhai; 

    } 

 

    if (lonNhat != i) { 

        swap(A[i], A[lonNhat]); 

        vunDongTaiNut(A, n, lonNhat); 

    } 

} 

 

void sapXepVunDong(int A[], int n) { 

    cout << "--- BAT DAU VUN DONG ---\n"; 

    for (int i = n / 2; i >= 1; i--) { 

        vunDongTaiNut(A, n, i); 

        cout << "Sau khi vun tai i = " << i << ": "; 

        inMang(A, n); 

    } 

 

    cout << "\n--- BAT DAU SAP XEP ---\n"; 

    int kichThuocHienTai = n; 

    for (int i = n; i > 1; i--) { 

        swap(A[1], A[i]); 

        kichThuocHienTai--; 

         

        vunDongTaiNut(A, kichThuocHienTai, 1); 

         

        cout << "Buoc dua goc ve cuoi: "; 

        inMang(A, n); 

    } 

} 

 

int main() { 

    int cay1[] = {0, 4, 1, 3, 2, 16, 9, 10, 14, 8, 7}; 

    int soPhanTu1 = 10;  

 

    cout << "================ CAY 1 ================\n"; 

    cout << "Mang ban dau: "; 

    inMang(cay1, soPhanTu1); 

    cout << "---------------------------------------\n"; 

    sapXepVunDong(cay1, soPhanTu1); 

    cout << "Ket qua:      "; 

    inMang(cay1, soPhanTu1); 

     

    cout << "\n\n"; 

 

    int cay2[] = {0, 11, 54, 32, 106, 38, 78, 208, 16, 84, 17, 39, 15}; 

    int soPhanTu2 = 12;  

 

    cout << "================ CAY 2 ================\n"; 

    cout << "Mang ban dau: "; 

    inMang(cay2, soPhanTu2); 

    cout << "---------------------------------------\n"; 

    sapXepVunDong(cay2, soPhanTu2); 

    cout << "Ket qua:      "; 

    inMang(cay2, soPhanTu2); 

 

    return 0; 

} 