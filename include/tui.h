#ifndef TUI_H
#define TUI_H

#include <ncurses.h>
#include "pos.h"
#include "transaksi.h"

/* ===== KONSTANTA ===== */

/* Konfigurasi Warna (ncurses color pairs) */
#define COLOR_PAIR_CYAN         1       /* Pasangan warna cyan untuk data */
#define COLOR_PAIR_RED          2       /* Pasangan warna merah untuk kesalahan */
#define COLOR_PAIR_HIGHLIGHT    3       /* Pasangan warna sorotan menu */
#define COLOR_PAIR_GREEN        4       /* Pasangan warna hijau untuk sukses */
#define COLOR_PAIR_YELLOW       5       /* Pasangan warna kuning untuk peringatan */
#define COLOR_PAIR_WHITE        6       /* Pasangan warna putih untuk teks biasa */

/* Konfigurasi Menu */
#define MENU_HIGHLIGHT_CHAR     '>'     /* Karakter penanda item terpilih */
#define MENU_CANCEL_KEY         'X'     /* Tombol untuk membatalkan */

#define MAX_MENU_ITEMS          20      /* Jumlah maksimal item dalam satu menu */
#define MAX_MENU_TEXT           50      /* Panjang maksimal teks item menu */

/* Konfigurasi Tabel */
#define MAX_TABLE_COLS          10      /* Jumlah maksimal kolom tabel */
#define MAX_COL_WIDTH           30      /* Lebar maksimal satu kolom */

/* Perataan Teks Tabel */
#define ALIGN_LEFT              0       /* Rata kiri */
#define ALIGN_CENTER            1       /* Rata tengah */
#define ALIGN_RIGHT             2       /* Rata kanan */

/* ===== STRUKTUR DATA ===== */

/**
 * Struct untuk representasi satu item menu
 */
typedef struct {
    char text[MAX_MENU_TEXT];   /* Teks yang ditampilkan untuk item */
    int value;                  /* Nilai yang dikembalikan jika item dipilih */
} MenuItem;

/**
 * Struct untuk mengelola menu navigasi
 */
typedef struct {
    char title[MAX_MENU_TEXT];      /* Judul menu */
    MenuItem items[MAX_MENU_ITEMS]; /* Array item menu */
    int item_count;                 /* Jumlah item saat ini */
    int selected;                   /* Index item yang sedang dipilih */
    int start_y;                    /* Posisi Y (baris) awal menu */
    int start_x;                    /* Posisi X (kolom) awal menu */
} Menu;

/**
 * Struct untuk konfigurasi satu kolom tabel
 */
typedef struct {
    char header[MAX_COL_WIDTH]; /* Judul header kolom */
    int width;                  /* Lebar kolom dalam karakter */
    int align;                  /* Perataan teks (ALIGN_*) */
} TableColumn;

/**
 * Struct untuk mengelola tampilan tabel
 */
typedef struct {
    TableColumn columns[MAX_TABLE_COLS]; /* Array definisi kolom */
    int col_count;                       /* Jumlah kolom saat ini */
    int start_y;                         /* Posisi Y (baris) awal tabel */
    int start_x;                         /* Posisi X (kolom) awal tabel */
} Table;

/* ===== DEKLARASI FUNGSI ===== */

/* --- Tampilan & Warna Dasar --- */

/*
    Function bertujuan untuk menginisialisasi ncurses dan pengaturan TUI.
    Input : -
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int tui_inisialisasi(void);

/*
    Procedure bertujuan untuk membersihkan sumber daya TUI (akhir program).
    I. S. : Mode TUI aktif
    F. S. : Mode TUI diakhiri, terminal kembali normal.
*/
void tui_bersihkan(void);

/*
    Procedure bertujuan untuk membersihkan layar.
    I. S. : Layar mungkin berisi teks
    F. S. : Layar bersih.
*/
void tui_hapus_layar(void);

/*
    Procedure bertujuan untuk menyegarkan layar untuk menampilkan perubahan.
    I. S. : Ada perubahan di buffer window
    F. S. : Perubahan ditampilkan di layar fisik.
*/
void tui_segarkan(void);

/*
    Function bertujuan untuk mendapatkan lebar terminal.
    Input : -
    Output : Mengembalikan jumlah kolom (lebar) terminal.
*/
int tui_ambil_lebar(void);

/*
    Function bertujuan untuk mendapatkan tinggi terminal.
    Input : -
    Output : Mengembalikan jumlah baris (tinggi) terminal.
*/
int tui_ambil_tinggi(void);

/*
    Procedure bertujuan untuk menginisialisasi pasangan warna ncurses.
    I. S. : Mode ncurses telah dimulai (start_color dipanggil)
    F. S. : Pasangan warna (init_pair) telah didefinisikan.
*/
void tui_inisialisasi_warna(void);

/*
    Procedure bertujuan untuk mengaktifkan atribut warna tertentu.
    I. S. : pair_id valid
    F. S. : Atribut warna pair_id aktif di window standar.
*/
void tui_aktifkan_warna(int pair_id);

/*
    Procedure bertujuan untuk menonaktifkan atribut warna tertentu.
    I. S. : Atribut warna pair_id mungkin aktif
    F. S. : Atribut warna pair_id nonaktif.
*/
void tui_nonaktifkan_warna(int pair_id);

/*
    Procedure bertujuan untuk mengaktifkan atribut bold (tebal).
    I. S. : Sembarang
    F. S. : Atribut A_BOLD aktif.
*/
void tui_aktifkan_tebal(void);

/*
    Procedure bertujuan untuk menonaktifkan atribut bold (tebal).
    I. S. : Sembarang
    F. S. : Atribut A_BOLD nonaktif.
*/
void tui_nonaktifkan_tebal(void);

/*
    Procedure bertujuan untuk mengaktifkan atribut reverse (warna terbalik).
    I. S. : Sembarang
    F. S. : Atribut A_REVERSE aktif.
*/
void tui_aktifkan_terbalik(void);

/*
    Procedure bertujuan untuk menonaktifkan atribut reverse (warna terbalik).
    I. S. : Sembarang
    F. S. : Atribut A_REVERSE nonaktif.
*/
void tui_nonaktifkan_terbalik(void);

/*
    Procedure bertujuan untuk mengaktifkan atribut underline (garis bawah).
    I. S. : Sembarang
    F. S. : Atribut A_UNDERLINE aktif.
*/
void tui_aktifkan_garis_bawah(void);

/*
    Procedure bertujuan untuk menonaktifkan atribut underline (garis bawah).
    I. S. : Sembarang
    F. S. : Atribut A_UNDERLINE nonaktif.
*/
void tui_nonaktifkan_garis_bawah(void);

/*
    Procedure bertujuan untuk mengatur ulang semua atribut tampilan ke normal.
    I. S. : Berbagai atribut mungkin aktif
    F. S. : Semua atribut dimatikan (A_NORMAL).
*/
void tui_reset_atribut(void);

/* --- Pencetakan & Gambar --- */

/*
    Procedure bertujuan untuk menampilkan teks di posisi tertentu.
    I. S. : Layar siap
    F. S. : Teks muncul di (y,x).
*/
void tui_cetak(int y, int x, const char *text);

/*
    Procedure bertujuan untuk menampilkan teks terformat (seperti printf) di posisi tertentu.
    I. S. : Layar siap
    F. S. : Teks terformat muncul di (y,x).
*/
void tui_cetakf(int y, int x, const char *fmt, ...);

/*
    Procedure bertujuan untuk menampilkan teks berwarna pada posisi tertentu.
    I. S. : Layar siap
    F. S. : Teks ditampilkan di (y,x) dengan warna pair_id.
*/
void tui_cetak_berwarna(int y, int x, int pair_id, const char *text);

/*
    Procedure bertujuan untuk menampilkan teks di tengah baris.
    I. S. : Layar siap
    F. S. : Teks muncul di tengah baris y.
*/
void tui_cetak_tengah(int y, const char *text);

/*
    Procedure bertujuan untuk menampilkan teks berwarna di tengah baris.
    I. S. : Layar siap
    F. S. : Teks ditampilkan di tengah baris y dengan warna pair_id.
*/
void tui_cetak_tengah_berwarna(int y, int pair_id, const char *text);

/*
    Procedure bertujuan untuk menggambar garis horizontal.
    I. S. : Layar siap
    F. S. : Garis tergambar dari (y,x) sepanjang length.
*/
void tui_gambar_garis_horizontal(int y, int x, int length, char ch);

/*
    Procedure bertujuan untuk menggambar kotak.
    I. S. : Layar siap
    F. S. : Kotak tergambar dengan border karakter.
*/
void tui_gambar_kotak(int y, int x, int height, int width);

/* --- Input & Kursor --- */

/*
    Function bertujuan untuk mendapatkan input satu karakter dari pengguna.
    Input : -
    Output : Mengembalikan kode karakter tombol yang ditekan.
*/
int tui_ambil_karakter(void);

/*
    Procedure bertujuan untuk mengatur visibilitas kursor.
    I. S. : Status kursor sembarang
    F. S. : Visibilitas kursor diubah sesuai parameter (1=tampil, 0=sembunyi).
*/
void tui_atur_kursor(int visible);

/*
    Procedure bertujuan untuk memindahkan kursor ke posisi tertentu.
    I. S. : Kursor di posisi lama
    F. S. : Kursor pindah ke (y,x).
*/
void tui_pindah(int y, int x);

/*
    Procedure bertujuan untuk menunggu input sembarang tombol (Jeda).
    I. S. : Program berjalan
    F. S. : Program berhenti sejenak hingga user menekan tombol.
*/
void tui_jeda(int y);

/*
    Procedure bertujuan untuk membersihkan baris input.
    I. S. : Baris y mungkin berisi teks sisa
    F. S. : Baris y kosong dari kolom 0 sampai akhir.
*/
void bersihkan_baris_input(int y);

/*
    Function bertujuan untuk mendapatkan input string dari pengguna.
    Input : y (Baris input), x (Kolom input), prompt (Pesan prompt), max_len (Panjang maksimal input)
    Output : buffer (Buffer untuk menyimpan input). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int masukan_string(int y, int x, const char *prompt, char *buffer, int max_len);

/*
    Function bertujuan untuk mendapatkan input string yang wajib diisi.
    Input : y (Baris input), x (Kolom input), prompt (Pesan prompt), max_len (Panjang maksimal input)
    Output : buffer (Buffer untuk menyimpan input). Mengembalikan 1 jika berhasil.
*/
int masukan_string_wajib(int y, int x, const char *prompt, char *buffer, int max_len);

/*
    Function bertujuan untuk mendapatkan input angka dari pengguna.
    Input : y (Baris input), x (Kolom input), prompt (Pesan prompt)
    Output : result (Pointer untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int masukan_angka(int y, int x, const char *prompt, unsigned long long *result);

/*
    Function bertujuan untuk mendapatkan input angka dengan nilai minimum.
    Input : y (Baris input), x (Kolom input), prompt (Pesan prompt), min (Nilai minimum)
    Output : result (Pointer untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int masukan_angka_min(int y, int x, const char *prompt, unsigned long long *result, unsigned long long min);

/*
    Function bertujuan untuk mendapatkan input tanggal yang valid.
    Input : y (Baris input), x (Kolom input), prompt (Pesan prompt)
    Output : buffer (Buffer untuk menyimpan tanggal dd-mm-YYYY). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int masukan_tanggal(int y, int x, const char *prompt, char *buffer);

/*
    Function bertujuan untuk mendapatkan konfirmasi ya/tidak dari pengguna.
    Input : y (Baris input), x (Kolom input), prompt (Pertanyaan konfirmasi)
    Output : Mengembalikan 1 jika Ya, 0 jika Tidak.
*/
int masukan_konfirmasi(int y, int x, const char *prompt);

/*
    Function bertujuan untuk mendapatkan input pilihan angka dalam rentang tertentu.
    Input : y (Baris input), x (Kolom input), prompt (Pesan prompt), max (Nilai maksimal pilihan)
    Output : Mengembalikan pilihan (1-max), atau 0 jika batal.
*/
int masukan_pilihan(int y, int x, const char *prompt, int max);

/*
    Function bertujuan untuk mendapatkan input bulan (1-12).
    Input : y (Baris input), x (Kolom input), prompt (Pesan prompt)
    Output : result (Pointer untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int masukan_bulan(int y, int x, const char *prompt, int *result);

/*
    Function bertujuan untuk mendapatkan input jenis transaksi (Pemasukan/Pengeluaran).
    Input : y (Baris input), x (Kolom input)
    Output : result (Pointer untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika batal.
*/
int masukan_jenis_transaksi(int y, int x, int *result);

/* --- Menu System --- */

/*
    Procedure bertujuan untuk menginisialisasi struktur menu.
    I. S. : menu sembarang
    F. S. : menu terinisialisasi dengan judul dan item kosong.
*/
void menu_inisialisasi(Menu *menu, const char *title);

/*
    Function bertujuan untuk menambahkan item ke menu.
    Input : menu (Pointer ke struct Menu), text (Teks item), value (Nilai item)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal (penuh).
*/
int menu_tambah_item(Menu *menu, const char *text, int value);

/*
    Procedure bertujuan untuk mengatur posisi awal menu.
    I. S. : menu terdefinisi
    F. S. : Posisi start_y dan start_x menu diupdate.
*/
void menu_atur_posisi(Menu *menu, int y, int x);

/*
    Procedure bertujuan untuk menampilkan menu di posisi yang diatur.
    I. S. : menu terisi item
    F. S. : Menu ditampilkan di layar pada posisi (start_y, start_x).
*/
void menu_tampilkan(Menu *menu);

/*
    Procedure bertujuan untuk menampilkan menu di tengah layar.
    I. S. : menu terisi item
    F. S. : Posisi menu dihitung agar tengah, lalu ditampilkan.
*/
void menu_tampilkan_tengah(Menu *menu);

/*
    Function bertujuan untuk menangani navigasi menu (loop hingga pilih/batal).
    Input : menu (Pointer ke struct Menu)
    Output : Mengembalikan nilai item yang dipilih, atau CANCEL jika batal.
*/
int menu_navigasi(Menu *menu);

/*
    Function bertujuan untuk mendapatkan nilai dari item yang sedang tersorot.
    Input : menu (Pointer ke struct Menu)
    Output : Mengembalikan nilai item terpilih.
*/
int menu_ambil_nilai_terpilih(Menu *menu);

/* --- UI Components (Header, Footer, Pesan) --- */

/*
    Procedure bertujuan untuk menampilkan header aplikasi standar.
    I. S. : Layar siap
    F. S. : Header ditampilkan di baris atas layar dengan garis pemisah.
*/
void tampilkan_header(const char *title);

/*
    Procedure bertujuan untuk menampilkan footer aplikasi standar.
    I. S. : Layar siap
    F. S. : Footer ditampilkan di baris bawah layar dengan garis pemisah.
*/
void tampilkan_footer(const char *instructions);

/*
    Procedure bertujuan untuk menampilkan pesan info (warna cyan).
    I. S. : Layar siap
    F. S. : Pesan info ditampilkan di area pesan.
*/
void tampilkan_info(const char *fmt, ...);

/*
    Procedure bertujuan untuk menampilkan pesan sukses (warna hijau).
    I. S. : Layar siap
    F. S. : Pesan sukses ditampilkan di area pesan.
*/
void tampilkan_sukses(const char *fmt, ...);

/*
    Procedure bertujuan untuk menampilkan pesan error (warna merah).
    I. S. : Layar siap
    F. S. : Pesan error ditampilkan di area pesan.
*/
void tampilkan_error(const char *fmt, ...);

/*
    Procedure bertujuan untuk menampilkan pesan peringatan (warna kuning).
    I. S. : Layar siap
    F. S. : Pesan warning ditampilkan di area pesan.
*/
void tampilkan_peringatan(const char *fmt, ...);

/*
    Function bertujuan untuk menampilkan pesan konfirmasi dan menunggu input pengguna.
    Input : message (Pesan konfirmasi)
    Output : Mengembalikan 1 jika pengguna memilih Ya, 0 jika Tidak.
*/
int tampilkan_konfirmasi(const char *message);

/*
    Procedure bertujuan untuk menampilkan kotak pesan (dialog box).
    I. S. : Layar siap
    F. S. : Kotak pesan ditampilkan di tengah layar, menunggu input pengguna.
*/
void tampilkan_kotak_pesan(const char *title, const char *message, int color_pair);

/*
    Procedure bertujuan untuk menampilkan status di baris status.
    I. S. : Layar siap
    F. S. : Pesan ditampilkan di baris status.
*/
void tampilkan_status(const char *message);

/*
    Procedure bertujuan untuk menampilkan indikator memuat.
    I. S. : Layar siap
    F. S. : Pesan memuat ditampilkan di area pesan.
*/
void tampilkan_memuat(const char *message);

/*
    Procedure bertujuan untuk membersihkan area pesan.
    I. S. : Area pesan mungkin berisi teks
    F. S. : Area pesan dibersihkan.
*/
void bersihkan_area_pesan(void);

/* --- Sistem Tabel --- */

/*
    Procedure bertujuan untuk menginisialisasi struktur tabel.
    I. S. : table sembarang
    F. S. : table terinisialisasi dengan 0 kolom.
*/
void tabel_inisialisasi(Table *table);

/*
    Function bertujuan untuk menambahkan kolom ke tabel.
    Input : table (Pointer ke struct Table), header (Judul kolom), width (Lebar kolom), align (Perataan kolom)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal (penuh).
*/
int tabel_tambah_kolom(Table *table, const char *header, int width, int align);

/*
    Procedure bertujuan untuk mengatur posisi tabel.
    I. S. : table terdefinisi
    F. S. : Posisi table diupdate.
*/
void tabel_atur_posisi(Table *table, int y, int x);

/*
    Function bertujuan untuk menggambar header tabel.
    Input : table (Pointer ke struct Table)
    Output : Mengembalikan posisi baris (y) setelah header.
*/
int tabel_gambar_header(Table *table);

/*
    Procedure bertujuan untuk menggambar satu baris data tabel.
    I. S. : table terdefinisi, values valid
    F. S. : Baris data ditampilkan di layar sesuai format kolom.
*/
void tabel_gambar_baris(Table *table, int row, int selected, const char **values);

/*
    Procedure bertujuan untuk menggambar garis pemisah tabel.
    I. S. : table terdefinisi
    F. S. : Garis horizontal selebar tabel digambar di baris y.
*/
void tabel_gambar_pemisah(Table *table, int y);

/*
    Function bertujuan untuk menampilkan tabel daftar transaksi.
    Input : list (Array transaksi), count (Jumlah transaksi), start_y (Baris awal), selected (Index transaksi terpilih)
    Output : Mengembalikan posisi baris (y) setelah tabel.
*/
int tampilkan_tabel_transaksi(Transaksi *list, int count, int start_y, int selected);

/*
    Function bertujuan untuk menampilkan tabel daftar pos anggaran.
    Input : list (Array pos anggaran), count (Jumlah pos), start_y (Baris awal), selected (Index pos terpilih)
    Output : Mengembalikan posisi baris (y) setelah tabel.
*/
int tampilkan_tabel_pos(PosAnggaran *list, int count, int start_y, int selected);

#endif
