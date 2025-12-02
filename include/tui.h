/**
 * @file tui.h
 * @brief Header untuk modul TUI
 * @author Kelompok B11
 * @date 2025
 */

#ifndef TUI_H
#define TUI_H

#include <ncurses.h>
#include "types.h"

/* --- tui_color.h --- */

void tui_init_colors(void);
void tui_color_on(int pair_id);
void tui_color_off(int pair_id);
void tui_bold_on(void);
void tui_bold_off(void);
void tui_reverse_on(void);
void tui_reverse_off(void);
void tui_underline_on(void);
void tui_underline_off(void);
void tui_print_colored(int y, int x, int pair_id, const char *text);
void tui_print_center_colored(int y, int pair_id, const char *text);
void tui_attr_reset(void);

/* --- tui_core.h --- */

int tui_init(void);
void tui_cleanup(void);
void tui_clear(void);
void tui_refresh(void);
int tui_get_width(void);
int tui_get_height(void);
void tui_print(int y, int x, const char *text);
void tui_printf(int y, int x, const char *fmt, ...);
void tui_print_center(int y, const char *text);
void tui_draw_hline(int y, int x, int length, char ch);
void tui_draw_box(int y, int x, int height, int width);
int tui_getch(void);
void tui_cursor(int visible);
void tui_move(int y, int x);
void tui_pause(int y);

/* --- tui_input.h --- */

int input_string(int y, int x, const char *prompt, char *buffer, int max_len);
int input_string_required(int y, int x, const char *prompt, char *buffer, int max_len);
int input_number(int y, int x, const char *prompt, unsigned long long *result);
int input_number_min(int y, int x, const char *prompt, unsigned long long *result, unsigned long long min);
int input_tanggal(int y, int x, const char *prompt, char *buffer);
int input_konfirmasi(int y, int x, const char *prompt);
int input_pilihan(int y, int x, const char *prompt, int max);
int input_bulan(int y, int x, const char *prompt, int *result);
int input_jenis_transaksi(int y, int x, int *result);
void clear_input_line(int y);

/* --- tui_menu.h --- */

#define MAX_MENU_ITEMS 20
#define MAX_MENU_TEXT 50

typedef struct {
    char text[MAX_MENU_TEXT];
    int value;
} MenuItem;

typedef struct {
    char title[MAX_MENU_TEXT];
    MenuItem items[MAX_MENU_ITEMS];
    int item_count;
    int selected;
    int start_y;
    int start_x;
} Menu;

void menu_init(Menu *menu, const char *title);
int menu_add_item(Menu *menu, const char *text, int value);
void menu_set_position(Menu *menu, int y, int x);
void menu_display(Menu *menu);
void menu_display_center(Menu *menu);
int menu_navigate(Menu *menu);
int menu_get_selected_value(Menu *menu);
void display_header(const char *title);
void display_footer(const char *instructions);

/* --- tui_message.h --- */

void show_info(const char *fmt, ...);
void show_success(const char *fmt, ...);
void show_error(const char *fmt, ...);
void show_warning(const char *fmt, ...);
int show_confirm(const char *message);
void show_message_box(const char *title, const char *message, int color_pair);
void show_status(const char *message);
void show_loading(const char *message);
void clear_message_area(void);

/* --- tui_table.h --- */

#define MAX_TABLE_COLS 10
#define MAX_COL_WIDTH 30

#define ALIGN_LEFT   0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT  2

typedef struct {
    char header[MAX_COL_WIDTH];
    int width;
    int align;
} TableColumn;

typedef struct {
    TableColumn columns[MAX_TABLE_COLS];
    int col_count;
    int start_y;
    int start_x;
} Table;

void table_init(Table *table);
int table_add_column(Table *table, const char *header, int width, int align);
void table_set_position(Table *table, int y, int x);
int table_draw_header(Table *table);
void table_draw_row(Table *table, int row, int selected, const char **values);
void table_draw_separator(Table *table, int y);
int display_transaksi_table(Transaksi *list, int count, int start_y, int selected);
int display_pos_table(PosAnggaran *list, int count, int start_y, int selected);

#endif /* TUI_H */
