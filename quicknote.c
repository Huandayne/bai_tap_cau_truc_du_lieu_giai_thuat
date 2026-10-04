#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_TITLE 256
#define MAX_CONTENT 1024
#define DB_FILE "notes.dat"

typedef struct {
    int id;
    char title[MAX_TITLE];
    char content[MAX_CONTENT];
} Note;

// Bắn thông báo ra màn hình dạng hộp thoại Message Box của Windows
void send_notification(const char *title, const char *body) {
    wchar_t w_title[MAX_TITLE];
    wchar_t w_body[MAX_CONTENT];

    MultiByteToWideChar(CP_UTF8, 0, title, -1, w_title, MAX_TITLE);
    MultiByteToWideChar(CP_UTF8, 0, body, -1, w_body, MAX_CONTENT);

    MessageBoxW(NULL, w_body, w_title, MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
}

void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
    if (len > 1 && str[len - 2] == '\r') str[len - 2] = '\0';
}

int get_next_id() {
    FILE *f = fopen(DB_FILE, "rb");
    if (!f) return 1;
    Note n;
    int max_id = 0;
    while (fread(&n, sizeof(Note), 1, f)) {
        if (n.id > max_id) max_id = n.id;
    }
    fclose(f);
    return max_id + 1;
}

// 1. Thêm ghi chú
void add_note() {
    Note n;
    printf("\n--- TAO GHI CHU MOI ---\n");
    printf("Tieu de: ");
    if (!fgets(n.title, sizeof(n.title), stdin)) return;
    trim_newline(n.title);

    if (strlen(n.title) == 0) {
        printf("Tieu de khong duoc de trong!\n");
        return;
    }

    printf("Noi dung: ");
    if (!fgets(n.content, sizeof(n.content), stdin)) return;
    trim_newline(n.content);

    n.id = get_next_id();

    FILE *f = fopen(DB_FILE, "ab");
    if (!f) {
        printf("Loi mo file du lieu!\n");
        return;
    }
    fwrite(&n, sizeof(Note), 1, f);
    fclose(f);

    printf("✓ Da luu thanh cong ghi chu [ID: %d]: '%s'\n", n.id, n.title);
    send_notification("QuickNote", "Da tao ghi chu moi thanh cong!");
}

// Liệt kê ghi chú
int list_notes() {
    FILE *f = fopen(DB_FILE, "rb");
    if (!f) {
        printf("\nChua co ghi chu nao.\n");
        return 0;
    }

    Note n;
    int count = 0;
    printf("\n--- DANH SACH GHI CHU ---\n");
    while (fread(&n, sizeof(Note), 1, f)) {
        count++;
        char preview[45];
        strncpy(preview, n.content, 40);
        preview[40] = '\0';
        if (strlen(n.content) > 40) strcat(preview, "...");
        printf("[%d] %s - %s\n", n.id, n.title, preview);
    }
    fclose(f);

    if (count == 0) {
        printf("Chua co ghi chu nao.\n");
    } else {
        printf("--------------------------\n");
    }
    return count;
}

// 2. Xem ghi chú và bắn thông báo
void view_note() {
    if (list_notes() == 0) return;

    printf("\nNhap ID ghi chu muon xem: ");
    char input[32];
    if (!fgets(input, sizeof(input), stdin)) return;
    int target_id = atoi(input);

    FILE *f = fopen(DB_FILE, "rb");
    if (!f) return;

    Note n;
    int found = 0;
    while (fread(&n, sizeof(Note), 1, f)) {
        if (n.id == target_id) {
            printf("\n=== [%d] %s ===\n", n.id, n.title);
            printf("%s\n", n.content);
            printf("=======================\n");
            send_notification(n.title, n.content);
            found = 1;
            break;
        }
    }
    fclose(f);

    if (!found) {
        printf("Khong tim thay ghi chu voi ID da nhap!\n");
    }
}

// 3. Xóa ghi chú
void delete_note() {
    if (list_notes() == 0) return;

    printf("\nNhap ID ghi chu muon xoa: ");
    char input[32];
    if (!fgets(input, sizeof(input), stdin)) return;
    int target_id = atoi(input);

    FILE *f = fopen(DB_FILE, "rb");
    if (!f) return;

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        fclose(f);
        return;
    }

    Note n;
    int found = 0;
    while (fread(&n, sizeof(Note), 1, f)) {
        if (n.id == target_id) {
            found = 1;
        } else {
            fwrite(&n, sizeof(Note), 1, temp);
        }
    }
    fclose(f);
    fclose(temp);

    if (found) {
        remove(DB_FILE);
        rename("temp.dat", DB_FILE);
        printf("✓ Da xoa thanh cong ghi chu ID: %d\n", target_id);
        send_notification("QuickNote", "Da xoa ghi chu thanh cong!");
    } else {
        remove("temp.dat");
        printf("Khong tim thay ghi chu voi ID da nhap!\n");
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    char choice[16];
    while (1) {
        printf("\n=== QUAN LY GHI CHU ===\n");
        printf("1. Tao ghi chu moi\n");
        printf("2. Xem lai ghi chu (Ban thong bao)\n");
        printf("3. Xoa ghi chu\n");
        printf("4. Thoat\n");
        printf("Chon thao tac (1-4): ");

        if (!fgets(choice, sizeof(choice), stdin)) break;
        trim_newline(choice);

        if (strcmp(choice, "1") == 0) {
            add_note();
        } else if (strcmp(choice, "2") == 0) {
            view_note();
        } else if (strcmp(choice, "3") == 0) {
            delete_note();
        } else if (strcmp(choice, "4") == 0) {
            printf("Tam biet!\n");
            break;
        } else {
            printf("Lua chon khong hop le, vui long thu lai!\n");
        }
    }
    return 0;
}
