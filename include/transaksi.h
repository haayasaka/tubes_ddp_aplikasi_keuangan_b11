/**
 * @file transaksi.h
 * @brief Header untuk modul transaksi
 * @author Kelompok B11
 * @date 2025
 */

#ifndef TRANSAKSI_H
#define TRANSAKSI_H

#include "types.h"

/* --- transaksi_crud.h --- */

/**
 * @brief Tambah transaksi baru dengan validasi
 * @param tanggal Tanggal transaksi (dd-mm-YYYY)
 * @param jenis Jenis transaksi (0=Pengeluaran, 1=Pemasukan)
 * @param pos Nama pos anggaran
 * @param nominal Nilai transaksi
 * @param deskripsi Keterangan transaksi
 * @param error_msg Buffer untuk pesan error (bisa NULL)
 * @return 1 jika berhasil, 0 jika gagal
 */
int transaksi_tambah(const char *tanggal, int jenis, const char *pos,
                     unsigned long long nominal, const char *deskripsi,
                     char *error_msg);

/**
 * @brief Edit transaksi dengan validasi
 * @param id ID transaksi yang diedit
 * @param tanggal Tanggal baru (NULL jika tidak diubah)
 * @param jenis Jenis baru (-1 jika tidak diubah)
 * @param pos Pos baru (NULL jika tidak diubah)
 * @param nominal Nominal baru (0 jika tidak diubah)
 * @param deskripsi Deskripsi baru (NULL jika tidak diubah)
 * @param error_msg Buffer untuk pesan error
 * @return 1 jika berhasil, 0 jika gagal
 */
int transaksi_edit(const char *id, const char *tanggal, int jenis,
                   const char *pos, unsigned long long nominal,
                   const char *deskripsi, char *error_msg);

/**
 * @brief Hapus transaksi dengan validasi
 * @param id ID transaksi yang dihapus
 * @param error_msg Buffer untuk pesan error
 * @return 1 jika berhasil, 0 jika gagal
 */
int transaksi_hapus(const char *id, char *error_msg);

/**
 * @brief Ambil daftar transaksi untuk bulan tertentu
 * @param list Array untuk menyimpan hasil
 * @param max_count Kapasitas maksimal
 * @param bulan Bulan (1-12), 0 untuk semua bulan
 * @return Jumlah transaksi
 */
int transaksi_get_list(Transaksi *list, int max_count, int bulan);

/**
 * @brief Ambil transaksi berdasarkan ID
 * @param id ID transaksi
 * @param result Pointer untuk menyimpan hasil
 * @return 1 jika ditemukan, 0 jika tidak
 */
int transaksi_get_by_id(const char *id, Transaksi *result);

/**
 * @brief Hitung total pemasukan untuk bulan tertentu
 * @param bulan Bulan (1-12)
 * @return Total pemasukan
 */
unsigned long long transaksi_total_pemasukan(int bulan);

/**
 * @brief Hitung total pengeluaran untuk bulan tertentu
 * @param bulan Bulan (1-12)
 * @return Total pengeluaran
 */
unsigned long long transaksi_total_pengeluaran(int bulan);

/**
 * @brief Hitung jumlah transaksi untuk bulan tertentu
 * @param bulan Bulan (1-12)
 * @param jenis Jenis transaksi (-1 untuk semua)
 * @return Jumlah transaksi
 */
int transaksi_count(int bulan, int jenis);

/* --- transaksi_display.h --- */

/**
 * @brief Tampilkan daftar transaksi bulan
 * @param bulan Bulan yang ditampilkan
 * @param selected Index transaksi terpilih (-1 jika tidak ada)
 * @return Baris berikutnya setelah tabel
 */
int tampilkan_daftar_transaksi(int bulan, int selected);

/**
 * @brief Tampilkan detail satu transaksi
 * @param trx Pointer ke transaksi
 * @param start_y Baris awal
 * @return Baris berikutnya setelah detail
 */
int tampilkan_detail_transaksi(Transaksi *trx, int start_y);

/**
 * @brief Tampilkan ringkasan transaksi bulan
 * @param bulan Bulan yang ditampilkan
 * @param start_y Baris awal
 * @return Baris berikutnya setelah ringkasan
 */
int tampilkan_ringkasan_transaksi(int bulan, int start_y);

/**
 * @brief Tampilkan form tambah transaksi
 * @param bulan Bulan default untuk transaksi
 * @return 1 jika berhasil tambah, 0 jika dibatalkan
 */
int tampilkan_form_tambah_transaksi(int bulan);

/**
 * @brief Tampilkan form edit transaksi
 * @param id ID transaksi yang diedit
 * @return 1 jika berhasil edit, 0 jika dibatalkan
 */
int tampilkan_form_edit_transaksi(const char *id);

/**
 * @brief Tampilkan konfirmasi hapus transaksi
 * @param id ID transaksi yang akan dihapus
 * @return 1 jika berhasil hapus, 0 jika dibatalkan
 */
int tampilkan_konfirmasi_hapus_transaksi(const char *id);

/* --- transaksi_handler.h --- */

/**
 * @brief Jalankan modul transaksi
 * @param bulan_awal Bulan awal yang ditampilkan
 */
void run_transaksi_module(int bulan_awal);

/* --- transaksi_menu.h --- */

/**
 * @brief Tampilkan menu utama transaksi
 * @param bulan Bulan yang aktif
 * @return Kode aksi yang dipilih
 */
int menu_transaksi_utama(int bulan);

/**
 * @brief Handler untuk view daftar transaksi
 * @param bulan Bulan yang ditampilkan
 */
void handler_view_transaksi(int bulan);

/**
 * @brief Handler untuk tambah transaksi
 * @param bulan Bulan untuk transaksi baru
 */
void handler_tambah_transaksi(int bulan);

/**
 * @brief Handler untuk edit transaksi
 * @param bulan Bulan transaksi
 */
void handler_edit_transaksi(int bulan);

/**
 * @brief Handler untuk hapus transaksi
 * @param bulan Bulan transaksi
 */
void handler_hapus_transaksi(int bulan);

#endif /* TRANSAKSI_H */
