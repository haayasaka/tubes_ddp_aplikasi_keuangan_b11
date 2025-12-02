/**
 * @file file.h
 * @brief Header untuk modul operasi file
 * @author Kelompok B11
 * @date 2025
 */

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "types.h"

/* --- file_analisis.h --- */

/**
 * @brief Load analisis keuangan untuk bulan tertentu
 * @param bulan Bulan (1-12)
 * @param result Pointer untuk menyimpan hasil
 * @return 1 jika berhasil, 0 jika gagal
 */
int load_analisis_bulan(int bulan, AnalisisKeuangan *result);

/**
 * @brief Simpan analisis keuangan untuk bulan tertentu
 * @param bulan Bulan (1-12)
 * @param analisis Data analisis
 * @return 1 jika berhasil, 0 jika gagal
 */
int save_analisis_bulan(int bulan, AnalisisKeuangan *analisis);

/**
 * @brief Cek apakah file analisis untuk bulan tertentu ada
 * @param bulan Bulan (1-12)
 * @return 1 jika ada, 0 jika tidak
 */
int analisis_file_exists(int bulan);

/**
 * @brief Inisialisasi analisis default untuk bulan baru
 * @param bulan Bulan (1-12)
 * @return 1 jika berhasil, 0 jika gagal
 */
int init_analisis_bulan(int bulan);

/* --- file_core.h --- */

/**
 * @brief Cek apakah file ada
 * @param filename Path file yang akan dicek
 * @return 1 jika file ada, 0 jika tidak ada
 */
int file_exists(const char *filename);

/**
 * @brief Buat file baru (kosong)
 * @param filename Path file yang akan dibuat
 * @return 1 jika berhasil, 0 jika gagal
 */
int file_create(const char *filename);

/**
 * @brief Buka file untuk dibaca
 * @param filename Path file yang akan dibuka
 * @return FILE pointer jika berhasil, NULL jika gagal
 * 
 * Caller bertanggung jawab menutup file dengan fclose()
 */
FILE* file_open_read(const char *filename);

/**
 * @brief Buka file untuk ditulis (overwrite)
 * @param filename Path file yang akan dibuka
 * @return FILE pointer jika berhasil, NULL jika gagal
 * 
 * Caller bertanggung jawab menutup file dengan fclose()
 */
FILE* file_open_write(const char *filename);

/**
 * @brief Buka file untuk append (tambah di akhir)
 * @param filename Path file yang akan dibuka
 * @return FILE pointer jika berhasil, NULL jika gagal
 * 
 * Caller bertanggung jawab menutup file dengan fclose()
 */
FILE* file_open_append(const char *filename);

/**
 * @brief Generate nama file dengan suffix bulan
 * @param base Nama dasar file (misal "pos_anggaran")
 * @param bulan Nomor bulan (1-12)
 * @param result Buffer untuk menyimpan hasil
 * @param result_size Ukuran buffer result
 * 
 * Contoh: get_filename_bulan("pos", 11, result, sizeof(result)) 
 *         -> result = "data/pos_11.txt"
 */
void get_filename_bulan(const char *base, int bulan, char *result, int result_size);

/**
 * @brief Generate nama file transaksi (tanpa suffix bulan)
 * @param result Buffer untuk menyimpan hasil
 * @param result_size Ukuran buffer result
 * 
 * Transaksi disimpan dalam satu file untuk semua bulan
 */
void get_filename_transaksi(char *result, int result_size);

/**
 * @brief Memastikan direktori data ada
 * @return 1 jika berhasil (direktori ada/dibuat), 0 jika gagal
 */
int ensure_data_directory(void);

/**
 * @brief Membaca seluruh isi file ke buffer
 * @param filename Path file yang akan dibaca
 * @param buffer Buffer untuk menyimpan isi file
 * @param buffer_size Ukuran buffer
 * @return Jumlah karakter yang dibaca, 0 jika gagal atau file kosong
 */
int file_read_all(const char *filename, char *buffer, int buffer_size);

/**
 * @brief Menulis string ke file (overwrite)
 * @param filename Path file tujuan
 * @param content String yang akan ditulis
 * @return 1 jika berhasil, 0 jika gagal
 */
int file_write_all(const char *filename, const char *content);

/**
 * @brief Menambahkan baris ke akhir file
 * @param filename Path file tujuan
 * @param line Baris yang akan ditambahkan
 * @return 1 jika berhasil, 0 jika gagal
 */
int file_append_line(const char *filename, const char *line);

/* --- file_parser.h --- */

/**
 * @brief Parse baris teks ke struct Transaksi
 * @param line Baris teks format: id|tanggal|jenis|pos|nominal|deskripsi
 * @param trx Pointer ke struct Transaksi untuk menyimpan hasil
 * @return 1 jika berhasil, 0 jika format invalid
 */
int parse_line_transaksi(const char *line, Transaksi *trx);

/**
 * @brief Parse baris teks ke struct PosAnggaran
 * @param line Baris teks format: no|nama|nominal|realisasi|sisa|jumlah_trx|status
 * @param pos Pointer ke struct PosAnggaran untuk menyimpan hasil
 * @return 1 jika berhasil, 0 jika format invalid
 */
int parse_line_pos(const char *line, PosAnggaran *pos);

/**
 * @brief Parse baris teks ke struct AnalisisKeuangan
 * @param line Baris teks format: pemasukan|pengeluaran|rata|persen|saldo|kondisi|kesimpulan|trx_out|trx_in
 * @param data Pointer ke struct AnalisisKeuangan untuk menyimpan hasil
 * @return 1 jika berhasil, 0 jika format invalid
 */
int parse_line_analisis(const char *line, AnalisisKeuangan *data);

/**
 * @brief Format struct Transaksi ke string pipe-delimited
 * @param trx Pointer ke struct Transaksi sumber
 * @param result Buffer untuk menyimpan hasil
 * @param result_size Ukuran buffer result
 */
void format_transaksi(const Transaksi *trx, char *result, int result_size);

/**
 * @brief Format struct PosAnggaran ke string pipe-delimited
 * @param pos Pointer ke struct PosAnggaran sumber
 * @param result Buffer untuk menyimpan hasil
 * @param result_size Ukuran buffer result
 */
void format_pos(const PosAnggaran *pos, char *result, int result_size);

/**
 * @brief Format struct AnalisisKeuangan ke string pipe-delimited
 * @param data Pointer ke struct AnalisisKeuangan sumber
 * @param result Buffer untuk menyimpan hasil
 * @param result_size Ukuran buffer result
 */
void format_analisis(const AnalisisKeuangan *data, char *result, int result_size);

/**
 * @brief Mendapatkan label jenis transaksi
 * @param jenis Kode jenis transaksi (0 atau 1)
 * @return "Pengeluaran" atau "Pemasukan"
 */
const char* get_label_jenis(int jenis);

/**
 * @brief Konversi string jenis ke kode
 * @param jenis_str String "Pemasukan" atau "Pengeluaran"
 * @return 0 untuk Pengeluaran, 1 untuk Pemasukan, -1 jika invalid
 */
int parse_jenis_transaksi(const char *jenis_str);

/* --- file_pos.h --- */

/**
 * @brief Load pos anggaran untuk bulan tertentu
 * @param list Array untuk menyimpan pos
 * @param max_count Kapasitas maksimal array
 * @param bulan Bulan (1-12)
 * @return Jumlah pos yang berhasil di-load
 */
int load_pos_bulan(PosAnggaran *list, int max_count, int bulan);

/**
 * @brief Simpan pos anggaran untuk bulan tertentu
 * @param list Array pos anggaran
 * @param count Jumlah pos
 * @param bulan Bulan (1-12)
 * @return 1 jika berhasil, 0 jika gagal
 */
int save_pos_bulan(PosAnggaran *list, int count, int bulan);

/**
 * @brief Tambah pos anggaran baru untuk bulan tertentu
 * @param pos Pointer ke pos baru
 * @param bulan Bulan (1-12)
 * @return 1 jika berhasil, 0 jika gagal
 */
int add_pos(PosAnggaran *pos, int bulan);

/**
 * @brief Update pos anggaran berdasarkan nomor
 * @param no Nomor pos yang akan diupdate
 * @param pos Data pos baru
 * @param bulan Bulan (1-12)
 * @return 1 jika berhasil, 0 jika tidak ditemukan
 */
int update_pos(int no, PosAnggaran *pos, int bulan);

/**
 * @brief Hapus pos anggaran berdasarkan nomor
 * @param no Nomor pos yang akan dihapus
 * @param bulan Bulan (1-12)
 * @return 1 jika berhasil, 0 jika tidak ditemukan
 */
int delete_pos(int no, int bulan);

/**
 * @brief Cari pos anggaran berdasarkan nama
 * @param nama Nama pos yang dicari
 * @param bulan Bulan (1-12)
 * @param result Pointer untuk menyimpan hasil
 * @return 1 jika ditemukan, 0 jika tidak
 */
int find_pos_by_nama(const char *nama, int bulan, PosAnggaran *result);

/**
 * @brief Cari pos anggaran berdasarkan nomor
 * @param no Nomor pos yang dicari
 * @param bulan Bulan (1-12)
 * @param result Pointer untuk menyimpan hasil
 * @return 1 jika ditemukan, 0 jika tidak
 */
int find_pos_by_no(int no, int bulan, PosAnggaran *result);

/**
 * @brief Cek apakah file pos untuk bulan tertentu ada
 * @param bulan Bulan (1-12)
 * @return 1 jika ada, 0 jika tidak
 */
int pos_file_exists(int bulan);

/**
 * @brief Inisialisasi pos default untuk bulan baru
 * @param bulan Bulan (1-12)
 * @return 1 jika berhasil, 0 jika gagal
 */
int init_pos_bulan(int bulan);

/**
 * @brief Copy pos dari bulan sebelumnya
 * @param bulan_tujuan Bulan tujuan (1-12)
 * @param bulan_sumber Bulan sumber (1-12)
 * @return 1 jika berhasil, 0 jika gagal
 */
int copy_pos_from_bulan(int bulan_tujuan, int bulan_sumber);

/* --- file_transaksi.h --- */

/**
 * @brief Load semua transaksi dari file
 * @param list Array untuk menyimpan transaksi
 * @param max_count Kapasitas maksimal array
 * @return Jumlah transaksi yang berhasil di-load
 */
int load_transaksi(Transaksi *list, int max_count);

/**
 * @brief Load transaksi berdasarkan bulan
 * @param list Array untuk menyimpan transaksi
 * @param max_count Kapasitas maksimal array
 * @param bulan Bulan yang difilter (1-12)
 * @return Jumlah transaksi yang berhasil di-load
 */
int load_transaksi_bulan(Transaksi *list, int max_count, int bulan);

/**
 * @brief Simpan semua transaksi ke file
 * @param list Array transaksi
 * @param count Jumlah transaksi
 * @return 1 jika berhasil, 0 jika gagal
 */
int save_transaksi(Transaksi *list, int count);

/**
 * @brief Tambah transaksi baru
 * @param trx Pointer ke transaksi baru
 * @return 1 jika berhasil, 0 jika gagal
 */
int add_transaksi(Transaksi *trx);

/**
 * @brief Update transaksi berdasarkan ID
 * @param id ID transaksi yang akan diupdate
 * @param trx Data transaksi baru
 * @return 1 jika berhasil, 0 jika tidak ditemukan
 */
int update_transaksi(const char *id, Transaksi *trx);

/**
 * @brief Hapus transaksi berdasarkan ID
 * @param id ID transaksi yang akan dihapus
 * @return 1 jika berhasil, 0 jika tidak ditemukan
 */
int delete_transaksi(const char *id);

/**
 * @brief Cari transaksi berdasarkan ID
 * @param id ID transaksi yang dicari
 * @param result Pointer untuk menyimpan hasil
 * @return 1 jika ditemukan, 0 jika tidak
 */
int find_transaksi_by_id(const char *id, Transaksi *result);

/**
 * @brief Generate ID transaksi baru
 * @param buffer Buffer untuk menyimpan ID (minimal 6 karakter)
 */
void generate_transaksi_id(char *buffer);

/**
 * @brief Hitung jumlah transaksi untuk pos tertentu
 * @param nama_pos Nama pos yang dihitung
 * @return Jumlah transaksi
 */
int count_transaksi_by_pos(const char *nama_pos);

#endif /* FILE_H */
