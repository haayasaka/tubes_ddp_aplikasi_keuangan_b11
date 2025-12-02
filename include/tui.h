/**
 * @file tui.h
 * @brief Header untuk modul TUI
 * @author Kelompok B11
 * @date 2025
 */

#ifndef TUI_H
#define TUI_H

#include <ncurses.h>
#include "pos.h"
#include "transaksi.h"

/* ===== KONFIGURASI WARNA (ncurses color pairs) ===== */
#define COLOR_PAIR_CYAN         1       /* Pair warna cyan untuk data */
#define COLOR_PAIR_RED          2       /* Pair warna merah untuk error */
#define COLOR_PAIR_HIGHLIGHT    3       /* Pair warna highlight menu */
#define COLOR_PAIR_GREEN        4       /* Pair warna hijau untuk sukses */
#define COLOR_PAIR_YELLOW       5       /* Pair warna kuning untuk warning */
#define COLOR_PAIR_WHITE        6       /* Pair warna putih untuk text biasa */

/* ===== KONFIGURASI MENU ===== */
#define MENU_HIGHLIGHT_CHAR     '>'     /* Karakter penanda item terpilih */
#define MENU_CANCEL_KEY         'X'     /* Tombol untuk cancel */

/* --- tui_color.h --- */

/*
    Procedure bertujuan untuk menginisialisasi pasangan warna ncurses.
    I. S. : ncurses mode telah dimulai (start_color dipanggil)
    F. S. : Pasangan warna (init_pair) telah didefinisikan.
*/
void tui_init_colors(void);

/*
    Procedure bertujuan untuk mengaktifkan atribut warna tertentu.
    I. S. : pair_id valid
    F. S. : Atribut warna pair_id aktif di window standar.
*/
void tui_color_on(int pair_id);

/*
    Procedure bertujuan untuk menonaktifkan atribut warna tertentu.
    I. S. : Atribut warna pair_id mungkin aktif
    F. S. : Atribut warna pair_id nonaktif.
*/
void tui_color_off(int pair_id);

/*
    Procedure bertujuan untuk mengaktifkan atribut bold (tebal).
    I. S. : Sembarang
    F. S. : Atribut A_BOLD aktif.
*/
void tui_bold_on(void);

/*
    Procedure bertujuan untuk menonaktifkan atribut bold (tebal).
    I. S. : Sembarang
    F. S. : Atribut A_BOLD nonaktif.
*/
void tui_bold_off(void);

/*
    Procedure bertujuan untuk mengaktifkan atribut reverse (warna terbalik).
    I. S. : Sembarang
    F. S. : Atribut A_REVERSE aktif.
*/
void tui_reverse_on(void);

/*
    Procedure bertujuan untuk menonaktifkan atribut reverse (warna terbalik).
    I. S. : Sembarang
    F. S. : Atribut A_REVERSE nonaktif.
*/
void tui_reverse_off(void);

/*
    Procedure bertujuan untuk mengaktifkan atribut underline (garis bawah).
    I. S. : Sembarang
    F. S. : Atribut A_UNDERLINE aktif.
*/
void tui_underline_on(void);

/*
    Procedure bertujuan untuk menonaktifkan atribut underline (garis bawah).
    I. S. : Sembarang
    F. S. : Atribut A_UNDERLINE nonaktif.
*/
void tui_underline_off(void);

/*
    Procedure bertujuan untuk menampilkan teks berwarna pada posisi tertentu.
    I. S. : Layar siap
    F. S. : Teks ditampilkan di (y,x) dengan warna pair_id.
*/
void tui_print_colored(int y, int x, int pair_id, const char *text);

/*
    Procedure bertujuan untuk menampilkan teks berwarna di tengah baris.
    I. S. : Layar siap
    F. S. : Teks ditampilkan di tengah baris y dengan warna pair_id.
*/
void tui_print_center_colored(int y, int pair_id, const char *text);

/*
    Procedure bertujuan untuk me-reset semua atribut tampilan ke normal.
    I. S. : Berbagai atribut mungkin aktif
    F. S. : Semua atribut dimatikan (A_NORMAL).
*/
void tui_attr_reset(void);

/* --- tui_core.h --- */

/*
    Function bertujuan untuk menginisialisasi ncurses dan settingan TUI.
    Input : -
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int tui_init(void);

/*
    Procedure bertujuan untuk membersihkan resources TUI (akhir program).
    I. S. : TUI mode aktif
    F. S. : TUI mode diakhiri, terminal kembali normal.
*/
void tui_cleanup(void);

/*
    Procedure bertujuan untuk membersihkan layar.
    I. S. : Layar mungkin berisi teks
    F. S. : Layar bersih.
*/
void tui_clear(void);

/*
    Procedure bertujuan untuk me-refresh layar untuk menampilkan perubahan.
    I. S. : Ada perubahan di buffer window
    F. S. : Perubahan ditampilkan di layar fisik.
*/
void tui_refresh(void);

/*
    Function bertujuan untuk mendapatkan lebar terminal.
    Input : -
    Output : Mengembalikan jumlah kolom (lebar) terminal.
*/
int tui_get_width(void);

/*
    Function bertujuan untuk mendapatkan tinggi terminal.
    Input : -
    Output : Mengembalikan jumlah baris (tinggi) terminal.
*/
int tui_get_height(void);

/*
    Procedure bertujuan untuk menampilkan teks di posisi tertentu.
    I. S. : Layar siap
    F. S. : Teks muncul di (y,x).
*/
void tui_print(int y, int x, const char *text);

/*
    Procedure bertujuan untuk menampilkan teks terformat (seperti printf) di posisi tertentu.
    I. S. : Layar siap
    F. S. : Teks terformat muncul di (y,x).
*/
void tui_printf(int y, int x, const char *fmt, ...);

/*
    Procedure bertujuan untuk menampilkan teks di tengah baris.
    I. S. : Layar siap
    F. S. : Teks muncul di tengah baris y.
*/
void tui_print_center(int y, const char *text);

/*
    Procedure bertujuan untuk menggambar garis horizontal.
    I. S. : Layar siap
    F. S. : Garis tergambar dari (y,x) sepanjang length.
*/
void tui_draw_hline(int y, int x, int length, char ch);

/*
    Procedure bertujuan untuk menggambar kotak.
    I. S. : Layar siap
    F. S. : Kotak tergambar dengan border karakter.
*/
void tui_draw_box(int y, int x, int height, int width);

/*
    Function bertujuan untuk mendapatkan input satu karakter dari user.
    Input : -
    Output : Mengembalikan kode karakter tombol yang ditekan.
*/
int tui_getch(void);

/*
    Procedure bertujuan untuk mengatur visibilitas kursor.
    I. S. : Status kursor sembarang
    F. S. : Visibilitas kursor diubah sesuai parameter (1=tampil, 0=sembunyi).
*/
void tui_cursor(int visible);

/*
    Procedure bertujuan untuk memindahkan kursor ke posisi tertentu.
    I. S. : Kursor di posisi lama
    F. S. : Kursor pindah ke (y,x).
*/
void tui_move(int y, int x);

/*
    Procedure bertujuan untuk menunggu input sembarang tombol.
    I. S. : Program berjalan
    F. S. : Program berhenti sejenak hingga user menekan tombol.
*/
void tui_pause(int y);

/* --- tui_input.h --- */

/*
    Function bertujuan untuk mendapatkan input string dari user.
    Input : prompt (pesan), max_len (panjang maksimal)
    Output : buffer (menyimpan string input). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int input_string(int y, int x, const char *prompt, char *buffer, int max_len);

/*
    Function bertujuan untuk mendapatkan input string yang wajib diisi.
    Input : prompt (pesan), max_len (panjang maksimal)
    Output : buffer (menyimpan string input). Mengembalikan 1 jika berhasil.
*/
int input_string_required(int y, int x, const char *prompt, char *buffer, int max_len);

/*
    Function bertujuan untuk mendapatkan input angka dari user.
    Input : prompt (pesan)
    Output : result (menyimpan nilai angka via pointer). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int input_number(int y, int x, const char *prompt, unsigned long long *result);

/*
    Function bertujuan untuk mendapatkan input angka dengan nilai minimum.
    Input : prompt (pesan), min (nilai minimum)
    Output : result (menyimpan nilai angka via pointer). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int input_number_min(int y, int x, const char *prompt, unsigned long long *result, unsigned long long min);

/*
    Function bertujuan untuk mendapatkan input tanggal yang valid.
    Input : prompt (pesan)
    Output : buffer (menyimpan string tanggal "dd-mm-YYYY"). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int input_tanggal(int y, int x, const char *prompt, char *buffer);

/*
    Function bertujuan untuk mendapatkan konfirmasi ya/tidak dari user.
    Input : prompt (pertanyaan)
    Output : Mengembalikan 1 jika Ya, 0 jika Tidak.
*/
int input_konfirmasi(int y, int x, const char *prompt);

/*
    Function bertujuan untuk mendapatkan input pilihan angka dalam range.
    Input : prompt (pesan), max (nilai maksimal)
    Output : Mengembalikan pilihan (1-max), atau 0 jika batal.
*/
int input_pilihan(int y, int x, const char *prompt, int max);

/*
    Function bertujuan untuk mendapatkan input bulan (1-12).
    Input : prompt (pesan)
    Output : result (menyimpan bulan via pointer). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int input_bulan(int y, int x, const char *prompt, int *result);

/*
    Function bertujuan untuk mendapatkan input jenis transaksi (Pemasukan/Pengeluaran).
    Input : -
    Output : result (menyimpan jenis transaksi via pointer). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int input_jenis_transaksi(int y, int x, int *result);

/*
    Procedure bertujuan untuk membersihkan baris input.
    I. S. : Baris y mungkin berisi teks sisa
    F. S. : Baris y kosong dari kolom 0 sampai akhir.
*/
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

/*
    Procedure bertujuan untuk menginisialisasi struktur menu.
    I. S. : menu sembarang
    F. S. : menu terinisialisasi dengan judul dan item kosong.
*/
void menu_init(Menu *menu, const char *title);

/*
    Function bertujuan untuk menambahkan item ke menu.
    Input : menu (pointer ke struct Menu), text (teks item), value (nilai item)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal (penuh).
*/
int menu_add_item(Menu *menu, const char *text, int value);

/*
    Procedure bertujuan untuk meng-set posisi awal menu.
    I. S. : menu terdefinisi
    F. S. : Posisi start_y dan start_x menu diupdate.
*/
void menu_set_position(Menu *menu, int y, int x);

/*
    Procedure bertujuan untuk menampilkan menu di posisi yang diset.
    I. S. : menu terisi item
    F. S. : Menu ditampilkan di layar pada posisi (start_y, start_x).
*/
void menu_display(Menu *menu);

/*
    Procedure bertujuan untuk menampilkan menu di tengah layar.
    I. S. : menu terisi item
    F. S. : Posisi menu dihitung agar tengah, lalu ditampilkan.
*/
void menu_display_center(Menu *menu);

/*
    Function bertujuan untuk meng-handle navigasi menu (looping hingga pilih/batal).
    Input : menu (pointer ke struct Menu)
    Output : Mengembalikan value dari item yang dipilih, atau CANCEL jika batal.
*/
int menu_navigate(Menu *menu);

/*
    Function bertujuan untuk mendapatkan value dari item yang sedang tersorot.
    Input : menu (pointer ke struct Menu)
    Output : Mengembalikan value item terpilih.
*/
int menu_get_selected_value(Menu *menu);

/*
    Procedure bertujuan untuk menampilkan header aplikasi standar.
    I. S. : Layar siap
    F. S. : Header ditampilkan di baris atas layar dengan garis pemisah.
*/
void display_header(const char *title);

/*
    Procedure bertujuan untuk menampilkan footer aplikasi standar.
    I. S. : Layar siap
    F. S. : Footer ditampilkan di baris bawah layar dengan garis pemisah.
*/
void display_footer(const char *instructions);

/* --- tui_message.h --- */

/*
    Procedure bertujuan untuk menampilkan pesan info (warna cyan).
    I. S. : Layar siap
    F. S. : Pesan info ditampilkan di area pesan.
*/
void show_info(const char *fmt, ...);

/*
    Procedure bertujuan untuk menampilkan pesan sukses (warna hijau).
    I. S. : Layar siap
    F. S. : Pesan sukses ditampilkan di area pesan.
*/
void show_success(const char *fmt, ...);

/*
    Procedure bertujuan untuk menampilkan pesan error (warna merah).
    I. S. : Layar siap
    F. S. : Pesan error ditampilkan di area pesan.
*/
void show_error(const char *fmt, ...);

/*
    Procedure bertujuan untuk menampilkan pesan warning (warna kuning).
    I. S. : Layar siap
    F. S. : Pesan warning ditampilkan di area pesan.
*/
void show_warning(const char *fmt, ...);

/*
    Function bertujuan untuk menampilkan pesan konfirmasi dan menunggu input user.
    Input : message (pesan konfirmasi)
    Output : Mengembalikan 1 jika user memilih Ya, 0 jika Tidak.
*/
int show_confirm(const char *message);

/*
    Procedure bertujuan untuk menampilkan kotak pesan (dialog box).
    I. S. : Layar siap
    F. S. : Kotak pesan ditampilkan di tengah layar, menunggu input user.
*/
void show_message_box(const char *title, const char *message, int color_pair);

/*
    Procedure bertujuan untuk menampilkan status di baris status.
    I. S. : Layar siap
    F. S. : Pesan ditampilkan di baris status.
*/
void show_status(const char *message);

/*
    Procedure bertujuan untuk menampilkan indikator loading.
    I. S. : Layar siap
    F. S. : Pesan loading ditampilkan di area pesan.
*/
void show_loading(const char *message);

/*
    Procedure bertujuan untuk membersihkan area pesan.
    I. S. : Area pesan mungkin berisi teks
    F. S. : Area pesan dibersihkan.
*/
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

/*
    Procedure bertujuan untuk menginisialisasi struktur tabel.
    I. S. : table sembarang
    F. S. : table terinisialisasi dengan 0 kolom.
*/
void table_init(Table *table);

/*
    Function bertujuan untuk menambahkan kolom ke tabel.
    Input : table, header (judul kolom), width (lebar), align (perataan)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal (penuh).
*/
int table_add_column(Table *table, const char *header, int width, int align);

/*
    Procedure bertujuan untuk meng-set posisi tabel.
    I. S. : table terdefinisi
    F. S. : Posisi table diupdate.
*/
void table_set_position(Table *table, int y, int x);

/*
    Function bertujuan untuk menggambar header tabel.
    Input : table (pointer ke struct Table)
    Output : Mengembalikan posisi baris (y) setelah header.
*/
int table_draw_header(Table *table);

/*
    Procedure bertujuan untuk menggambar satu baris data tabel.
    I. S. : table terdefinisi, values valid
    F. S. : Baris data ditampilkan di layar sesuai format kolom.
*/
void table_draw_row(Table *table, int row, int selected, const char **values);

/*
    Procedure bertujuan untuk menggambar garis pemisah tabel.
    I. S. : table terdefinisi
    F. S. : Garis horizontal selebar tabel digambar di baris y.
*/
void table_draw_separator(Table *table, int y);

/*
    Function bertujuan untuk menampilkan tabel daftar transaksi.
    Input : list (array transaksi), count (jumlah), start_y (baris awal), selected (index terpilih)
    Output : Mengembalikan posisi baris (y) setelah tabel.
*/
int display_transaksi_table(Transaksi *list, int count, int start_y, int selected);

/*
    Function bertujuan untuk menampilkan tabel daftar pos anggaran.
    Input : list (array pos), count (jumlah), start_y (baris awal), selected (index terpilih)
    Output : Mengembalikan posisi baris (y) setelah tabel.
*/
int display_pos_table(PosAnggaran *list, int count, int start_y, int selected);

#endif /* TUI_H */
