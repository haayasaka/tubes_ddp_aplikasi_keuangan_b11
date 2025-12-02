/**
 * @file transaksi.h
 * @brief Header untuk modul transaksi
 * @author Kelompok B11
 * @date 2025
 */

#ifndef TRANSAKSI_H
#define TRANSAKSI_H

/**
 * @brief Struct untuk menyimpan data satu transaksi keuangan
 * 
 * Menyimpan informasi lengkap tentang transaksi baik pemasukan maupun pengeluaran
 */
typedef struct Transaksi {
    char id[6];                 /* ID transaksi (T0001-T9999) */
    char tanggal[11];           /* Format dd-mm-YYYY */
    int jenis;                  /* 0=Pengeluaran, 1=Pemasukan */
    char pos[21];               /* Nama pos anggaran (max 20 char) */
    unsigned long long nominal; /* Nilai transaksi > 0 */
    char deskripsi[51];         /* Keterangan (max 50 char) */
} Transaksi;

/**
 * @brief Struct untuk array dinamis transaksi
 * 
 * Digunakan untuk mengelola koleksi transaksi dengan ukuran dinamis
 */
typedef struct ListTransaksi {
    Transaksi *data;            /* Pointer ke array transaksi */
    int count;                  /* Jumlah transaksi saat ini */
    int capacity;               /* Kapasitas maksimal array */
} ListTransaksi;

/* ===== KONSTANTA TRANSAKSI ===== */
#define MAX_DESKRIPSI_LENGTH    50      /* Panjang maksimal deskripsi */
#define MAX_ID_LENGTH           5       /* Panjang ID transaksi (T0001) */
#define MAX_TANGGAL_LENGTH      10      /* Format dd-mm-YYYY */
#define MAX_TRANSAKSI           1000    /* Maksimal transaksi per bulan */
#define FILE_TRANSAKSI          "transaksi"         /* Nama dasar file transaksi */
#define FILE_TRANSAKSI_NAME     "transaksi.txt"     /* Nama file transaksi */

/* ===== JENIS TRANSAKSI ===== */
#define JENIS_PENGELUARAN       0       /* Kode untuk pengeluaran */
#define JENIS_PEMASUKAN         1       /* Kode untuk pemasukan */

/* --- transaksi_crud.h --- */

/*
    Function bertujuan untuk menambahkan transaksi baru dengan validasi.
    Input : tanggal, jenis, pos, nominal, deskripsi
    Output : error_msg (menyimpan pesan error via pointer). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int transaksi_tambah(const char *tanggal, int jenis, const char *pos,
                     unsigned long long nominal, const char *deskripsi,
                     char *error_msg);

/*
    Function bertujuan untuk mengedit transaksi dengan validasi.
    Input : id, tanggal, jenis, pos, nominal, deskripsi
    Output : error_msg (menyimpan pesan error via pointer). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int transaksi_edit(const char *id, const char *tanggal, int jenis,
                   const char *pos, unsigned long long nominal,
                   const char *deskripsi, char *error_msg);

/*
    Function bertujuan untuk menghapus transaksi dengan validasi.
    Input : id
    Output : error_msg (menyimpan pesan error via pointer). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int transaksi_hapus(const char *id, char *error_msg);

/*
    Function bertujuan untuk mengambil daftar transaksi untuk bulan tertentu.
    Input : max_count (kapasitas maksimal), bulan (1-12, 0 untuk semua)
    Output : list (array menyimpan hasil). Mengembalikan jumlah transaksi.
*/
int transaksi_get_list(Transaksi *list, int max_count, int bulan);

/*
    Function bertujuan untuk mengambil transaksi berdasarkan ID.
    Input : id (ID transaksi)
    Output : result (pointer menyimpan hasil). Mengembalikan 1 jika ditemukan, 0 jika tidak.
*/
int transaksi_get_by_id(const char *id, Transaksi *result);

/*
    Function bertujuan untuk menghitung total pemasukan untuk bulan tertentu.
    Input : bulan (1-12)
    Output : Mengembalikan total pemasukan.
*/
unsigned long long transaksi_total_pemasukan(int bulan);

/*
    Function bertujuan untuk menghitung total pengeluaran untuk bulan tertentu.
    Input : bulan (1-12)
    Output : Mengembalikan total pengeluaran.
*/
unsigned long long transaksi_total_pengeluaran(int bulan);

/*
    Function bertujuan untuk menghitung jumlah transaksi untuk bulan tertentu.
    Input : bulan (1-12), jenis (-1 untuk semua)
    Output : Mengembalikan jumlah transaksi.
*/
int transaksi_count(int bulan, int jenis);

/* --- transaksi_display.h --- */

/*
    Function bertujuan untuk menampilkan daftar transaksi bulan.
    Input : bulan, selected (index terpilih)
    Output : Mengembalikan posisi baris berikutnya setelah tabel.
*/
int tampilkan_daftar_transaksi(int bulan, int selected);

/*
    Function bertujuan untuk menampilkan detail satu transaksi.
    Input : trx (pointer ke transaksi), start_y (baris awal)
    Output : Mengembalikan posisi baris berikutnya setelah detail.
*/
int tampilkan_detail_transaksi(Transaksi *trx, int start_y);

/*
    Function bertujuan untuk menampilkan ringkasan transaksi bulan.
    Input : bulan, start_y (baris awal)
    Output : Mengembalikan posisi baris berikutnya setelah ringkasan.
*/
int tampilkan_ringkasan_transaksi(int bulan, int start_y);

/*
    Function bertujuan untuk menampilkan form tambah transaksi.
    Input : bulan (default)
    Output : Mengembalikan 1 jika berhasil tambah, 0 jika dibatalkan.
*/
int tampilkan_form_tambah_transaksi(int bulan);

/*
    Function bertujuan untuk menampilkan form edit transaksi.
    Input : id (ID transaksi)
    Output : Mengembalikan 1 jika berhasil edit, 0 jika dibatalkan.
*/
int tampilkan_form_edit_transaksi(const char *id);

/*
    Function bertujuan untuk menampilkan konfirmasi hapus transaksi.
    Input : id (ID transaksi)
    Output : Mengembalikan 1 jika berhasil hapus, 0 jika dibatalkan.
*/
int tampilkan_konfirmasi_hapus_transaksi(const char *id);

/* --- transaksi_handler.h --- */

/*
    Procedure bertujuan untuk menjalankan modul transaksi.
    I. S. : bulan_awal valid
    F. S. : Modul transaksi dijalankan hingga user kembali.
*/
void run_transaksi_module(int bulan_awal);

/* --- transaksi_menu.h --- */

/*
    Function bertujuan untuk menampilkan menu utama transaksi.
    Input : bulan (aktif)
    Output : Mengembalikan kode aksi yang dipilih.
*/
int menu_transaksi_utama(int bulan);

/*
    Procedure bertujuan untuk menangani view daftar transaksi.
    I. S. : bulan valid
    F. S. : User kembali dari view.
*/
void handler_view_transaksi(int bulan);

/*
    Procedure bertujuan untuk menangani tambah transaksi.
    I. S. : bulan valid
    F. S. : Transaksi baru ditambahkan jika user konfirmasi.
*/
void handler_tambah_transaksi(int bulan);

/*
    Procedure bertujuan untuk menangani edit transaksi.
    I. S. : bulan valid
    F. S. : Transaksi diedit jika user konfirmasi.
*/
void handler_edit_transaksi(int bulan);

/*
    Procedure bertujuan untuk menangani hapus transaksi.
    I. S. : bulan valid
    F. S. : Transaksi dihapus jika user konfirmasi.
*/
void handler_hapus_transaksi(int bulan);

#endif /* TRANSAKSI_H */
