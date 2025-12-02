/**
 * @file file.h
 * @brief Header untuk modul operasi file
 * @author Kelompok B11
 * @date 2025
 */

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "utils.h"
#include "pos.h"
#include "transaksi.h"
#include "analisis.h"

/* ===== KONSTANTA FILE ===== */
#define DATA_DIR                "data/"             /* Direktori data */
#define FILE_EXTENSION          ".txt"              /* Ekstensi file */
#define MAX_LINE_LENGTH         512                 /* Panjang maksimal baris file */
#define MAX_PATH_LENGTH         256                 /* Panjang maksimal path file */
#define MAX_PATH                MAX_PATH_LENGTH     /* Alias untuk MAX_PATH_LENGTH */

/* --- file_analisis.h --- */

/*
    Function bertujuan untuk memuat data analisis keuangan untuk bulan tertentu dari file.
    Input : bulan (1-12)
    Output : result (pointer untuk menyimpan hasil analisis). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int load_analisis_bulan(int bulan, AnalisisKeuangan *result);

/*
    Function bertujuan untuk menyimpan analisis keuangan untuk bulan tertentu ke file.
    Input : bulan (1-12), analisis (data analisis)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int save_analisis_bulan(int bulan, AnalisisKeuangan *analisis);

/*
    Function bertujuan untuk memeriksa apakah file analisis untuk bulan tertentu ada.
    Input : bulan (1-12)
    Output : Mengembalikan 1 jika ada, 0 jika tidak.
*/
int analisis_file_exists(int bulan);

/*
    Function bertujuan untuk menginisialisasi analisis default untuk bulan baru.
    Input : bulan (1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int init_analisis_bulan(int bulan);

/* --- file_core.h --- */

/*
    Function bertujuan untuk memeriksa apakah file ada.
    Input : filename (path file yang akan dicek)
    Output : Mengembalikan 1 jika file ada, 0 jika tidak ada.
*/
int file_exists(const char *filename);

/*
    Function bertujuan untuk membuat file baru (kosong).
    Input : filename (path file yang akan dibuat)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int file_create(const char *filename);

/*
    Function bertujuan untuk membuka file untuk dibaca.
    Input : filename (path file yang akan dibuka)
    Output : Mengembalikan FILE pointer jika berhasil, NULL jika gagal.
*/
FILE* file_open_read(const char *filename);

/*
    Function bertujuan untuk membuka file untuk ditulis (overwrite).
    Input : filename (path file yang akan dibuka)
    Output : Mengembalikan FILE pointer jika berhasil, NULL jika gagal.
*/
FILE* file_open_write(const char *filename);

/*
    Function bertujuan untuk membuka file untuk append (tambah di akhir).
    Input : filename (path file yang akan dibuka)
    Output : Mengembalikan FILE pointer jika berhasil, NULL jika gagal.
*/
FILE* file_open_append(const char *filename);

/*
    Procedure bertujuan untuk menghasilkan nama file dengan suffix bulan.
    Input : base (nama dasar file), bulan (nomor bulan 1-12), result_size (ukuran buffer result)
    Output : result (buffer menyimpan path file lengkap, misal "data/pos_11.txt")
*/
void get_filename_bulan(const char *base, int bulan, char *result, int result_size);

/*
    Procedure bertujuan untuk menghasilkan nama file transaksi.
    Input : result_size (ukuran buffer result)
    Output : result (buffer menyimpan path file transaksi)
*/
void get_filename_transaksi(char *result, int result_size);

/*
    Function bertujuan untuk memastikan direktori data ada.
    Input : -
    Output : Mengembalikan 1 jika berhasil (direktori ada/dibuat), 0 jika gagal.
*/
int ensure_data_directory(void);

/*
    Function bertujuan untuk membaca seluruh isi file ke buffer.
    Input : filename (path file), buffer_size (ukuran buffer)
    Output : buffer (menyimpan isi file). Mengembalikan jumlah karakter yang dibaca, 0 jika gagal atau file kosong.
*/
int file_read_all(const char *filename, char *buffer, int buffer_size);

/*
    Function bertujuan untuk menulis string ke file (overwrite).
    Input : filename (path file tujuan), content (string yang akan ditulis)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int file_write_all(const char *filename, const char *content);

/*
    Function bertujuan untuk menambahkan baris ke akhir file.
    Input : filename (path file tujuan), line (baris yang akan ditambahkan)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int file_append_line(const char *filename, const char *line);

/* --- file_parser.h --- */

/*
    Function bertujuan untuk mem-parse baris teks ke struct Transaksi.
    Input : line (baris teks format pipe-delimited)
    Output : trx (pointer ke struct Transaksi untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika format invalid.
*/
int parse_line_transaksi(const char *line, Transaksi *trx);

/*
    Function bertujuan untuk mem-parse baris teks ke struct PosAnggaran.
    Input : line (baris teks format pipe-delimited)
    Output : pos (pointer ke struct PosAnggaran untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika format invalid.
*/
int parse_line_pos(const char *line, PosAnggaran *pos);

/*
    Function bertujuan untuk mem-parse baris teks ke struct AnalisisKeuangan.
    Input : line (baris teks format pipe-delimited)
    Output : data (pointer ke struct AnalisisKeuangan untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika format invalid.
*/
int parse_line_analisis(const char *line, AnalisisKeuangan *data);

/*
    Procedure bertujuan untuk memformat struct Transaksi ke string pipe-delimited.
    Input : trx (pointer ke struct Transaksi sumber), result_size (ukuran buffer result)
    Output : result (buffer menyimpan string format file untuk transaksi)
*/
void format_transaksi(const Transaksi *trx, char *result, int result_size);

/*
    Procedure bertujuan untuk memformat struct PosAnggaran ke string pipe-delimited.
    Input : pos (pointer ke struct PosAnggaran sumber), result_size (ukuran buffer result)
    Output : result (buffer menyimpan string format file untuk pos anggaran)
*/
void format_pos(const PosAnggaran *pos, char *result, int result_size);

/*
    Procedure bertujuan untuk memformat struct AnalisisKeuangan ke string pipe-delimited.
    Input : data (pointer ke struct AnalisisKeuangan sumber), result_size (ukuran buffer result)
    Output : result (buffer menyimpan string format file untuk analisis)
*/
void format_analisis(const AnalisisKeuangan *data, char *result, int result_size);

/*
    Function bertujuan untuk mendapatkan label jenis transaksi.
    Input : jenis (kode jenis transaksi 0 atau 1)
    Output : Mengembalikan string "Pengeluaran" atau "Pemasukan".
*/
const char* get_label_jenis(int jenis);

/*
    Function bertujuan untuk mengkonversi string jenis ke kode.
    Input : jenis_str (string "Pemasukan" atau "Pengeluaran")
    Output : Mengembalikan 0 untuk Pengeluaran, 1 untuk Pemasukan, -1 jika invalid.
*/
int parse_jenis_transaksi(const char *jenis_str);

/* --- file_pos.h --- */

/*
    Function bertujuan untuk memuat pos anggaran untuk bulan tertentu dari file.
    Input : max_count (kapasitas maksimal array), bulan (1-12)
    Output : list (array menyimpan pos). Mengembalikan jumlah pos yang berhasil di-load.
*/
int load_pos_bulan(PosAnggaran *list, int max_count, int bulan);

/*
    Function bertujuan untuk menyimpan pos anggaran untuk bulan tertentu ke file.
    Input : list (array pos anggaran), count (jumlah pos), bulan (1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int save_pos_bulan(PosAnggaran *list, int count, int bulan);

/*
    Function bertujuan untuk menambahkan pos anggaran baru untuk bulan tertentu.
    Input : pos (pointer ke pos baru), bulan (1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int add_pos(PosAnggaran *pos, int bulan);

/*
    Function bertujuan untuk mengupdate pos anggaran berdasarkan nomor.
    Input : no (nomor pos), pos (data pos baru), bulan (1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika tidak ditemukan.
*/
int update_pos(int no, PosAnggaran *pos, int bulan);

/*
    Function bertujuan untuk menghapus pos anggaran berdasarkan nomor.
    Input : no (nomor pos), bulan (1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika tidak ditemukan.
*/
int delete_pos(int no, int bulan);

/*
    Function bertujuan untuk mencari pos anggaran berdasarkan nama.
    Input : nama (nama pos), bulan (1-12)
    Output : result (pointer menyimpan hasil pencarian). Mengembalikan 1 jika ditemukan, 0 jika tidak.
*/
int find_pos_by_nama(const char *nama, int bulan, PosAnggaran *result);

/*
    Function bertujuan untuk mencari pos anggaran berdasarkan nomor.
    Input : no (nomor pos), bulan (1-12)
    Output : result (pointer menyimpan hasil pencarian). Mengembalikan 1 jika ditemukan, 0 jika tidak.
*/
int find_pos_by_no(int no, int bulan, PosAnggaran *result);

/*
    Function bertujuan untuk memeriksa apakah file pos untuk bulan tertentu ada.
    Input : bulan (1-12)
    Output : Mengembalikan 1 jika ada, 0 jika tidak.
*/
int pos_file_exists(int bulan);

/*
    Function bertujuan untuk menginisialisasi pos default untuk bulan baru.
    Input : bulan (1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int init_pos_bulan(int bulan);

/*
    Function bertujuan untuk menyalin pos dari bulan sebelumnya.
    Input : bulan_tujuan (bulan target), bulan_sumber (bulan sumber)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int copy_pos_from_bulan(int bulan_tujuan, int bulan_sumber);

/* --- file_transaksi.h --- */

/*
    Function bertujuan untuk memuat semua transaksi dari file.
    Input : max_count (kapasitas maksimal array)
    Output : list (array menyimpan transaksi). Mengembalikan jumlah transaksi yang berhasil di-load.
*/
int load_transaksi(Transaksi *list, int max_count);

/*
    Function bertujuan untuk memuat transaksi berdasarkan bulan.
    Input : max_count (kapasitas maksimal array), bulan (yang difilter 1-12)
    Output : list (array menyimpan transaksi). Mengembalikan jumlah transaksi yang berhasil di-load.
*/
int load_transaksi_bulan(Transaksi *list, int max_count, int bulan);

/*
    Function bertujuan untuk menyimpan semua transaksi ke file.
    Input : list (array transaksi), count (jumlah transaksi)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int save_transaksi(Transaksi *list, int count);

/*
    Function bertujuan untuk menambahkan transaksi baru.
    Input : trx (pointer ke transaksi baru)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int add_transaksi(Transaksi *trx);

/*
    Function bertujuan untuk mengupdate transaksi berdasarkan ID.
    Input : id (ID transaksi), trx (data transaksi baru)
    Output : Mengembalikan 1 jika berhasil, 0 jika tidak ditemukan.
*/
int update_transaksi(const char *id, Transaksi *trx);

/*
    Function bertujuan untuk menghapus transaksi berdasarkan ID.
    Input : id (ID transaksi)
    Output : Mengembalikan 1 jika berhasil, 0 jika tidak ditemukan.
*/
int delete_transaksi(const char *id);

/*
    Function bertujuan untuk mencari transaksi berdasarkan ID.
    Input : id (ID transaksi)
    Output : result (pointer menyimpan hasil pencarian). Mengembalikan 1 jika ditemukan, 0 jika tidak.
*/
int find_transaksi_by_id(const char *id, Transaksi *result);

/*
    Procedure bertujuan untuk menghasilkan ID transaksi baru yang unik.
    Input : buffer (buffer untuk menyimpan ID)
    Output : buffer berisi ID unik baru (format Txxxx).
*/
void generate_transaksi_id(char *buffer);

/*
    Function bertujuan untuk menghitung jumlah transaksi untuk pos tertentu.
    Input : nama_pos (nama pos yang dihitung)
    Output : Mengembalikan jumlah transaksi.
*/
int count_transaksi_by_pos(const char *nama_pos);

#endif /* FILE_H */
