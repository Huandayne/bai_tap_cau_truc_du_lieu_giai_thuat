#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "sqlite3.h"

#define MAX_TITLE 256
#define MAX_CONTENT 1024

// Bắn thông báo ra màn hình dạng hộp thoại Message Box của Windows
void send_notification(const char *title, const char *body) {
    // Chuyển UTF-8 sang UTF-16 (Wide Char) để hiển thị đúng tiếng Việt trong MessageBox
    wchar_t w_title[MAX_TITLE];
    wchar_t w_body[MAX_CONTENT];

    MultiByteToWideChar(CP_UTF8, 0, title, -1, w_title, MAX_TITLE);
    MultiByteToWideChar(CP_UTF8, 0, body, -1, w_body, MAX_CONTENT);

    // MB_ICONINFORMATION | MB_TOPMOST: Hiện icon thông tin và luôn nổi lên trên các cửa sổ khác
    MessageBoxW(NULL, w_body, w_title, MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
}

// Khởi tạo bảng dữ liệu SQLite
sqlite3* init_db() {
    sqlite3 *db;
    if (sqlite3_open("notes.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Lỗi mở cơ sở dữ liệu: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS notes ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "title TEXT NOT NULL, "
                      "content TEXT NOT NULL);";
    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Lỗi tạo bảng: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    return db;
}

void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
    if (len > 1 && str[len - 2] == '\r') str[len - 2] = '\0'; // Xóa ký tự \r của Windows
}

// 1. Tạo ghi chú mới
void add_note(sqlite3 *db) {
    char title[MAX_TITLE];
    char content[MAX_CONTENT];

    printf("\n--- TẠO GHI CHÚ MỚI ---\n");
    printf("Tiêu đề: ");
    if (!fgets(title, sizeof(title), stdin)) return;
    trim_newline(title);

    if (strlen(title) == 0) {
        printf("Tiêu đề không được để trống!\n");
        return;
    }

    printf("Nội dung: ");
    if (!fgets(content, sizeof(content), stdin)) return;
    trim_newline(content);

    const char *sql = "INSERT INTO notes (title, content) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, content, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            printf("✓ Đã lưu thành công: '%s'\n", title);
            send_notification("QuickNote - Thông báo", "Đã tạo ghi chú thành công!");
        } else {
            printf("Lỗi khi thêm ghi chú!\n");
        }
    }
    sqlite3_finalize(stmt);
}

// Liệt kê danh sách ghi chú
int list_notes(sqlite3 *db) {
    const char *sql = "SELECT id, title, content FROM notes ORDER BY id ASC;";
    sqlite3_stmt *stmt;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        printf("\n--- DANH SÁCH GHI CHÚ ---\n");
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            count++;
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *title = sqlite3_column_text(stmt, 1);
            const unsigned char *content = sqlite3_column_text(stmt, 2);

            char preview[45];
            strncpy(preview, (const char*)content, 40);
            preview[40] = '\0';
            if (strlen((const char*)content) > 40) {
                strcat(preview, "...");
            }

            printf("[%d] %s - %s\n", id, title, preview);
        }
        printf("--------------------------\n");
    }
    sqlite3_finalize(stmt);

    if (count == 0) {
        printf("Chưa có ghi chú nào.\n");
    }
    return count;
}

// 2. Xem ghi chú và bắn thông báo hộp thoại
void view_note(sqlite3 *db) {
    if (list_notes(db) == 0) return;

    printf("\nNhập ID ghi chú muốn xem: ");
    char input[32];
    if (!fgets(input, sizeof(input), stdin)) return;
    int target_id = atoi(input);

    const char *sql = "SELECT id, title, content FROM notes WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, target_id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *title = sqlite3_column_text(stmt, 1);
            const unsigned char *content = sqlite3_column_text(stmt, 2);

            printf("\n=== [%d] %s ===\n", target_id, title);
            printf("%s\n", content);
            printf("=======================\n");

            // Bắn hộp thoại Windows nổi lên màn hình
            send_notification((const char*)title, (const char*)content);
        } else {
            printf("Không tìm thấy ghi chú với ID đã nhập!\n");
        }
    }
    sqlite3_finalize(stmt);
}

// 3. Xóa ghi chú
void delete_note(sqlite3 *db) {
    if (list_notes(db) == 0) return;

    printf("\nNhập ID ghi chú muốn xóa: ");
    char input[32];
    if (!fgets(input, sizeof(input), stdin)) return;
    int target_id = atoi(input);

    const char *sql = "DELETE FROM notes WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, target_id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            if (sqlite3_changes(db) > 0) {
                printf("✓ Đã xóa thành công ghi chú ID: %d\n", target_id);
                char notify_msg[64];
                snprintf(notify_msg, sizeof(notify_msg), "Đã xóa thành công ghi chú ID #%d", target_id);
                send_notification("QuickNote - Xóa ghi chú", notify_msg);
            } else {
                printf("Không tìm thấy ghi chú với ID đã nhập!\n");
            }
        }
    }
    sqlite3_finalize(stmt);
}

int main() {
    // Thiết lập hiển thị UTF-8 trên Windows Command Prompt/PowerShell
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    sqlite3 *db = init_db();
    if (!db) return 1;

    char choice[16];
    while (1) {
        printf("\n=== QUẢN LÝ GHI CHÚ (Windows C) ===\n");
        printf("1. Tạo ghi chú mới\n");
        printf("2. Xem lại ghi chú (Bắn thông báo ra màn hình)\n");
        printf("3. Xóa ghi chú\n");
        printf("4. Thoát\n");
        printf("Chọn thao tác (1-4): ");

        if (!fgets(choice, sizeof(choice), stdin)) break;
        trim_newline(choice);

        if (strcmp(choice, "1") == 0) {
            add_note(db);
        } else if (strcmp(choice, "2") == 0) {
            view_note(db);
        } else if (strcmp(choice, "3") == 0) {
            delete_note(db);
        } else if (strcmp(choice, "4") == 0) {
            printf("Tạm biệt!\n");
            break;
        } else {
            printf("Lựa chọn không hợp lệ, vui lòng thử lại!\n");
        }
    }

    sqlite3_close(db);
    return 0;
}
