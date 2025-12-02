/**
 * @file tui.c
 * @brief Implementasi modul TUI
 * @author Kelompok B11
 * @date 2025
 */

#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tui.h"
#include "utils.h"
#include "validator.h"
#include "file.h"
#include "constants.h"

/* --- tui_color.c --- */

void tui_init_colors(void) {
    if (!has_colors()) return;
    init_pair(COLOR_PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_RED, COLOR_RED, COLOR_BLACK);
    /* REV-4: Improved highlight visibility - cyan background with black text */
    init_pair(COLOR_PAIR_HIGHLIGHT, COLOR_BLACK, COLOR_CYAN);
    init_pair(COLOR_PAIR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
}

void tui_color_on(int pair_id) { if (has_colors()) attron(COLOR_PAIR(pair_id)); }
void tui_color_off(int pair_id) { if (has_colors()) attroff(COLOR_PAIR(pair_id)); }
void tui_bold_on(void) { attron(A_BOLD); }
void tui_bold_off(void) { attroff(A_BOLD); }
void tui_reverse_on(void) { attron(A_REVERSE); }
void tui_reverse_off(void) { attroff(A_REVERSE); }
void tui_underline_on(void) { attron(A_UNDERLINE); }
void tui_underline_off(void) { attroff(A_UNDERLINE); }

void tui_print_colored(int y, int x, int pair_id, const char *text) {
    if (!text) return;
    tui_color_on(pair_id);
    mvprintw(y, x, "%s", text);
    tui_color_off(pair_id);
}

void tui_print_center_colored(int y, int pair_id, const char *text) {
    if (!text) return;
    int len = strlen(text);
    int x = (COLS - len) / 2;
    if (x < 0) x = 0;
    tui_color_on(pair_id);
    mvprintw(y, x, "%s", text);
    tui_color_off(pair_id);
}

void tui_attr_reset(void) { attrset(A_NORMAL); }

/* --- tui_core.c --- */

int tui_init(void) {
    if (initscr() == NULL) return 0;
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    if (has_colors()) {
        start_color();
        tui_init_colors();
    }
    return 1;
}

void tui_cleanup(void) { curs_set(1); endwin(); }
void tui_clear(void) { clear(); }
void tui_refresh(void) { refresh(); }
int tui_get_width(void) { return COLS; }
int tui_get_height(void) { return LINES; }

void tui_print(int y, int x, const char *text) {
    if (text) mvprintw(y, x, "%s", text);
}

void tui_printf(int y, int x, const char *fmt, ...) {
    if (!fmt) return;
    va_list args;
    va_start(args, fmt);
    move(y, x);
    vw_printw(stdscr, fmt, args);
    va_end(args);
}

void tui_print_center(int y, const char *text) {
    if (!text) return;
    int x = (COLS - (int)strlen(text)) / 2;
    if (x < 0) x = 0;
    mvprintw(y, x, "%s", text);
}

void tui_draw_hline(int y, int x, int length, char ch) {
    if (ch == 0) ch = '-';
    move(y, x);
    for (int i = 0; i < length; i++) addch(ch);
}

void tui_draw_box(int y, int x, int height, int width) {
    mvaddch(y, x, ACS_ULCORNER);
    for (int i = 1; i < width - 1; i++) addch(ACS_HLINE);
    addch(ACS_URCORNER);
    for (int i = 1; i < height - 1; i++) {
        mvaddch(y + i, x, ACS_VLINE);
        mvaddch(y + i, x + width - 1, ACS_VLINE);
    }
    mvaddch(y + height - 1, x, ACS_LLCORNER);
    for (int i = 1; i < width - 1; i++) addch(ACS_HLINE);
    addch(ACS_LRCORNER);
}

int tui_getch(void) { return getch(); }
void tui_cursor(int visible) { curs_set(visible ? 1 : 0); }
void tui_move(int y, int x) { move(y, x); }

void tui_pause(int y) {
    tui_print_center(y, "Tekan sembarang tombol untuk melanjutkan...");
    tui_refresh();
    tui_getch();
}

/* --- tui_input.c --- */

void clear_input_line(int y) {
    move(y, 0);
    clrtoeol();
}

int input_string(int y, int x, const char *prompt, char *buffer, int max_len) {
    if (!buffer || max_len <= 0) return 0;
    tui_print(y, x, prompt);
    tui_cursor(1);
    
    int input_x = x + strlen(prompt);
    move(y, input_x);
    
    char temp[256];
    memset(temp, 0, sizeof(temp));
    int pos = 0, ch;
    
    while (1) {
        ch = getch();
        if (ch == 27) { tui_cursor(0); buffer[0] = '\0'; return 0; }
        if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) break;
        if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (pos > 0) { pos--; temp[pos] = '\0'; mvaddch(y, input_x + pos, ' '); move(y, input_x + pos); }
        } else if (isprint(ch) && pos < max_len - 1 && pos < 254) {
            temp[pos] = ch; mvaddch(y, input_x + pos, ch); pos++; temp[pos] = '\0';
        }
        refresh();
    }
    tui_cursor(0);
    
    strncpy(buffer, temp, max_len - 1);
    buffer[max_len - 1] = '\0';
    
    /* Trim */
    char *start = buffer;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != buffer) memmove(buffer, start, strlen(start) + 1);
    char *end = buffer + strlen(buffer) - 1;
    while (end > buffer && isspace((unsigned char)*end)) { *end = '\0'; end--; }
    
    return 1;
}

int input_string_required(int y, int x, const char *prompt, char *buffer, int max_len) {
    while (1) {
        if (!input_string(y, x, prompt, buffer, max_len)) return 0;
        if (validasi_tidak_kosong(buffer)) return 1;
        show_error("Input tidak boleh kosong!");
        clear_input_line(y);
    }
}

int input_number(int y, int x, const char *prompt, unsigned long long *result) {
    char buffer[32];
    while (1) {
        if (!input_string(y, x, prompt, buffer, sizeof(buffer))) return 0;
        if (parse_nominal(buffer, result)) return 1;
        show_error("Input harus berupa angka positif!");
        clear_input_line(y);
    }
}

int input_number_min(int y, int x, const char *prompt, unsigned long long *result, unsigned long long min) {
    while (1) {
        if (!input_number(y, x, prompt, result)) return 0;
        if (*result >= min) return 1;
        show_error("Nilai harus minimal %llu!", min);
        clear_input_line(y);
    }
}

int input_tanggal(int y, int x, const char *prompt, char *buffer) {
    char full_prompt[128];
    snprintf(full_prompt, sizeof(full_prompt), "%s (dd-mm-YYYY): ", prompt);
    while (1) {
        if (!input_string(y, x, full_prompt, buffer, 11)) return 0;
        if (validasi_tanggal_lengkap(buffer)) return 1;
        show_error("Format tanggal tidak valid! Gunakan dd-mm-YYYY");
        clear_input_line(y);
    }
}

int input_konfirmasi(int y, int x, const char *prompt) {
    char full_prompt[128];
    snprintf(full_prompt, sizeof(full_prompt), "%s (y/n): ", prompt);
    tui_print(y, x, full_prompt);
    tui_refresh();
    while (1) {
        int ch = tui_getch();
        if (ch == 'y' || ch == 'Y') return 1;
        if (ch == 'n' || ch == 'N' || ch == 27) return 0;
    }
}

int input_pilihan(int y, int x, const char *prompt, int max) {
    char buffer[16], full_prompt[128];
    snprintf(full_prompt, sizeof(full_prompt), "%s (1-%d): ", prompt, max);
    while (1) {
        if (!input_string(y, x, full_prompt, buffer, sizeof(buffer))) return 0;
        int pilihan = atoi(buffer);
        if (validasi_pilihan_menu(pilihan, 1, max)) return pilihan;
        show_error("Pilihan tidak valid! Masukkan 1-%d", max);
        clear_input_line(y);
    }
}

int input_bulan(int y, int x, const char *prompt, int *result) {
    char buffer[16], full_prompt[128];
    snprintf(full_prompt, sizeof(full_prompt), "%s (1-12): ", prompt);
    while (1) {
        if (!input_string(y, x, full_prompt, buffer, sizeof(buffer))) return 0;
        int bulan = atoi(buffer);
        if (bulan >= BULAN_MIN && bulan <= BULAN_MAX) { *result = bulan; return 1; }
        show_error("Bulan tidak valid! Masukkan 1-12");
        clear_input_line(y);
    }
}

int input_jenis_transaksi(int y, int x, int *result) {
    tui_print(y, x, "Jenis: 1=Pengeluaran, 2=Pemasukan: ");
    tui_refresh();
    while (1) {
        int ch = tui_getch();
        if (ch == '1') { *result = JENIS_PENGELUARAN; return 1; }
        if (ch == '2') { *result = JENIS_PEMASUKAN; return 1; }
        if (ch == 27) return 0;
    }
}

/* --- tui_menu.c --- */

void menu_init(Menu *menu, const char *title) {
    if (!menu) return;
    memset(menu, 0, sizeof(Menu));
    if (title) strncpy(menu->title, title, MAX_MENU_TEXT - 1);
    menu->selected = 0;
    menu->start_y = 5;
    menu->start_x = 5;
}

int menu_add_item(Menu *menu, const char *text, int value) {
    if (!menu || !text || menu->item_count >= MAX_MENU_ITEMS) return 0;
    strncpy(menu->items[menu->item_count].text, text, MAX_MENU_TEXT - 1);
    menu->items[menu->item_count].value = value;
    menu->item_count++;
    return 1;
}

void menu_set_position(Menu *menu, int y, int x) {
    if (!menu) return;
    menu->start_y = y;
    menu->start_x = x;
}

void menu_display(Menu *menu) {
    if (!menu) return;
    int y = menu->start_y, x = menu->start_x;
    
    if (strlen(menu->title) > 0) {
        tui_bold_on();
        tui_color_on(COLOR_PAIR_CYAN);
        tui_print(y, x, menu->title);
        tui_color_off(COLOR_PAIR_CYAN);
        tui_bold_off();
        y += 2;
    }
    
    for (int i = 0; i < menu->item_count; i++) {
        if (i == menu->selected) {
            /* REV-4: Add arrow indicator + bold + color highlight for better visibility */
            tui_bold_on();
            tui_color_on(COLOR_PAIR_HIGHLIGHT);
            tui_printf(y + i, x, "> %d. %s  ", i + 1, menu->items[i].text);
            tui_color_off(COLOR_PAIR_HIGHLIGHT);
            tui_bold_off();
        } else {
            tui_printf(y + i, x, "  %d. %s  ", i + 1, menu->items[i].text);
        }
    }
}

void menu_display_center(Menu *menu) {
    if (!menu) return;
    int max_width = strlen(menu->title);
    for (int i = 0; i < menu->item_count; i++) {
        int len = strlen(menu->items[i].text) + 8;
        if (len > max_width) max_width = len;
    }
    int x = (tui_get_width() - max_width) / 2;
    int y = (tui_get_height() - menu->item_count - 4) / 2;
    if (x < 2) x = 2;
    if (y < 2) y = 2;
    menu_set_position(menu, y, x);
    menu_display(menu);
}

int menu_navigate(Menu *menu) {
    if (!menu || menu->item_count == 0) return -1;
    
    while (1) {
        tui_clear();
        display_header("APLIKASI KEUANGAN MAHASISWA");
        menu_display_center(menu);
        display_footer("UP/DOWN: Navigasi | ENTER: Pilih | ESC: Kembali");
        tui_refresh();
        
        int ch = tui_getch();
        switch (ch) {
            case KEY_UP: case 'k': case 'K':
                if (menu->selected > 0) menu->selected--;
                break;
            case KEY_DOWN: case 'j': case 'J':
                if (menu->selected < menu->item_count - 1) menu->selected++;
                break;
            case KEY_ENTER: case '\n': case '\r':
                return menu->items[menu->selected].value;
            case 27:
                return CANCEL;
            default:
                if (ch >= '1' && ch <= '9') {
                    int idx = ch - '1';
                    if (idx < menu->item_count) {
                        menu->selected = idx;
                        return menu->items[idx].value;
                    }
                }
                break;
        }
    }
}

int menu_get_selected_value(Menu *menu) {
    if (!menu || menu->item_count == 0) return -1;
    return menu->items[menu->selected].value;
}

void display_header(const char *title) {
    int width = tui_get_width();
    tui_color_on(COLOR_PAIR_CYAN);
    tui_draw_hline(0, 0, width, '=');
    tui_bold_on();
    tui_print_center(1, title);
    tui_bold_off();
    tui_draw_hline(2, 0, width, '=');
    tui_color_off(COLOR_PAIR_CYAN);
}

void display_footer(const char *instructions) {
    int h = tui_get_height(), w = tui_get_width();
    tui_color_on(COLOR_PAIR_CYAN);
    tui_draw_hline(h - 2, 0, w, '-');
    if (instructions) tui_print_center(h - 1, instructions);
    tui_color_off(COLOR_PAIR_CYAN);
}

/* --- tui_message.c --- */

#define MSG_LINE (LINES - 4)

static void show_message_colored(int color_pair, const char *prefix, const char *fmt, va_list args) {
    char buffer[256], full_msg[300];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    snprintf(full_msg, sizeof(full_msg), "%s%s", prefix, buffer);
    move(MSG_LINE, 0);
    clrtoeol();
    tui_color_on(color_pair);
    tui_print_center(MSG_LINE, full_msg);
    tui_color_off(color_pair);
    tui_refresh();
    tui_getch();
    move(MSG_LINE, 0);
    clrtoeol();
}

void show_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    show_message_colored(COLOR_PAIR_CYAN, "[INFO] ", fmt, args);
    va_end(args);
}

void show_success(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    show_message_colored(COLOR_PAIR_GREEN, "[OK] ", fmt, args);
    va_end(args);
}

void show_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    show_message_colored(COLOR_PAIR_RED, "[ERROR] ", fmt, args);
    va_end(args);
}

void show_warning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    show_message_colored(COLOR_PAIR_YELLOW, "[WARNING] ", fmt, args);
    va_end(args);
}

int show_confirm(const char *message) {
    char full_msg[256];
    snprintf(full_msg, sizeof(full_msg), "%s (y/n)", message);
    move(MSG_LINE, 0);
    clrtoeol();
    tui_color_on(COLOR_PAIR_YELLOW);
    tui_print_center(MSG_LINE, full_msg);
    tui_color_off(COLOR_PAIR_YELLOW);
    tui_refresh();
    while (1) {
        int ch = tui_getch();
        if (ch == 'y' || ch == 'Y') { move(MSG_LINE, 0); clrtoeol(); return 1; }
        if (ch == 'n' || ch == 'N' || ch == 27) { move(MSG_LINE, 0); clrtoeol(); return 0; }
    }
}

void show_message_box(const char *title, const char *message, int color_pair) {
    int msg_len = strlen(message);
    int title_len = title ? (int)strlen(title) : 0;
    int width = (msg_len > title_len ? msg_len : title_len) + 6;
    int height = 5;
    int x = (COLS - width) / 2, y = (LINES - height) / 2;
    if (x < 1) x = 1;
    if (y < 1) y = 1;
    tui_color_on(color_pair);
    tui_draw_box(y, x, height, width);
    if (title) { tui_bold_on(); tui_print(y, x + 2, title); tui_bold_off(); }
    tui_print(y + 2, x + 3, message);
    tui_color_off(color_pair);
    tui_refresh();
    tui_getch();
}

void show_status(const char *message) {
    if (!message) return;
    move(LINES - 3, 0);
    clrtoeol();
    tui_color_on(COLOR_PAIR_CYAN);
    mvprintw(LINES - 3, 2, "%s", message);
    tui_color_off(COLOR_PAIR_CYAN);
    tui_refresh();
}

void show_loading(const char *message) {
    if (!message) return;
    move(MSG_LINE, 0);
    clrtoeol();
    tui_color_on(COLOR_PAIR_YELLOW);
    tui_print_center(MSG_LINE, message);
    tui_color_off(COLOR_PAIR_YELLOW);
    tui_refresh();
}

void clear_message_area(void) {
    move(MSG_LINE, 0);
    clrtoeol();
    tui_refresh();
}

/* --- tui_table.c --- */

void table_init(Table *table) {
    if (!table) return;
    memset(table, 0, sizeof(Table));
    table->start_y = 4;
    table->start_x = 2;
}

int table_add_column(Table *table, const char *header, int width, int align) {
    if (!table || !header || table->col_count >= MAX_TABLE_COLS) return 0;
    strncpy(table->columns[table->col_count].header, header, MAX_COL_WIDTH - 1);
    table->columns[table->col_count].width = width;
    table->columns[table->col_count].align = align;
    table->col_count++;
    return 1;
}

void table_set_position(Table *table, int y, int x) {
    if (!table) return;
    table->start_y = y;
    table->start_x = x;
}

int table_draw_header(Table *table) {
    if (!table) return 0;
    int x = table->start_x;
    int y = table->start_y;
    
    tui_bold_on();
    tui_color_on(COLOR_PAIR_CYAN);
    for (int i = 0; i < table->col_count; i++) {
        mvprintw(y, x, "%*s", table->columns[i].width, table->columns[i].header);
        x += table->columns[i].width + 1;
    }
    tui_color_off(COLOR_PAIR_CYAN);
    tui_bold_off();
    
    y++;
    table_draw_separator(table, y);
    
    return y + 1;
}

void table_draw_row(Table *table, int row, int selected, const char **values) {
    if (!table || !values) return;
    int x = table->start_x;
    int y = table->start_y + 2 + row;
    
    if (selected) {
        tui_reverse_on();
        tui_color_on(COLOR_PAIR_HIGHLIGHT);
    }
    
    for (int i = 0; i < table->col_count; i++) {
        int w = table->columns[i].width;
        const char *val = values[i] ? values[i] : "";
        
        switch (table->columns[i].align) {
            case ALIGN_RIGHT:
                mvprintw(y, x, "%*s", w, val);
                break;
            case ALIGN_CENTER: {
                int pad = (w - (int)strlen(val)) / 2;
                if (pad < 0) pad = 0;
                mvprintw(y, x + pad, "%*s", w - pad, val);
                break;
            }
            default:
                mvprintw(y, x, "%*s", w, val);
                break;
        }
        x += w + 1;
    }
    
    if (selected) {
        tui_color_off(COLOR_PAIR_HIGHLIGHT);
        tui_reverse_off();
    }
}

void table_draw_separator(Table *table, int y) {
    if (!table) return;
    int total_width = 0;
    for (int i = 0; i < table->col_count; i++) {
        total_width += table->columns[i].width + 1;
    }
    tui_draw_hline(y, table->start_x, total_width - 1, '-');
}

int display_transaksi_table(Transaksi *list, int count, int start_y, int selected) {
    Table table;
    table_init(&table);
    table_set_position(&table, start_y, 2);
    
    table_add_column(&table, "No", 4, ALIGN_RIGHT);
    table_add_column(&table, "ID", 6, ALIGN_LEFT);
    table_add_column(&table, "Tanggal", 12, ALIGN_CENTER);
    table_add_column(&table, "Jenis", 12, ALIGN_LEFT);
    table_add_column(&table, "Pos", 15, ALIGN_LEFT);
    table_add_column(&table, "Nominal", 15, ALIGN_RIGHT);
    table_add_column(&table, "Deskripsi", 20, ALIGN_LEFT);
    
    int row_y = table_draw_header(&table);
    
    if (count == 0) {
        tui_print(row_y, 4, "(Tidak ada transaksi)");
        return row_y + 2;
    }
    
    char no_str[16], nominal_str[30];
    const char *values[7];
    
    for (int i = 0; i < count; i++) {
        snprintf(no_str, sizeof(no_str), "%d", i + 1);
        format_rupiah(list[i].nominal, nominal_str);
        
        values[0] = no_str;
        values[1] = list[i].id;
        values[2] = list[i].tanggal;
        values[3] = get_label_jenis(list[i].jenis);
        values[4] = list[i].pos;
        values[5] = nominal_str;
        values[6] = list[i].deskripsi;
        
        table_draw_row(&table, i, (i == selected), values);
    }
    
    return row_y + count + 1;
}

int display_pos_table(PosAnggaran *list, int count, int start_y, int selected) {
    Table table;
    table_init(&table);
    table_set_position(&table, start_y, 2);
    
    table_add_column(&table, "No", 4, ALIGN_RIGHT);
    table_add_column(&table, "Nama Pos", 20, ALIGN_LEFT);
    table_add_column(&table, "Anggaran", 15, ALIGN_RIGHT);
    table_add_column(&table, "Realisasi", 15, ALIGN_RIGHT);
    table_add_column(&table, "Sisa", 15, ALIGN_RIGHT);
    table_add_column(&table, "Status", 12, ALIGN_CENTER);
    
    int row_y = table_draw_header(&table);
    
    if (count == 0) {
        tui_print(row_y, 4, "(Tidak ada pos anggaran)");
        return row_y + 2;
    }
    
    char no_str[16], anggaran_str[30], realisasi_str[30], sisa_str[30];
    const char *values[6];
    
    for (int i = 0; i < count; i++) {
        snprintf(no_str, sizeof(no_str), "%d", list[i].no);
        format_rupiah(list[i].nominal, anggaran_str);
        format_rupiah(list[i].realisasi, realisasi_str);
        format_saldo(list[i].sisa, sisa_str);
        
        values[0] = no_str;
        values[1] = list[i].nama;
        values[2] = anggaran_str;
        values[3] = realisasi_str;
        values[4] = sisa_str;
        values[5] = (list[i].status == STATUS_AMAN) ? "Aman" : "Tidak Aman";
        
        table_draw_row(&table, i, (i == selected), values);
    }
    
    return row_y + count + 1;
}
