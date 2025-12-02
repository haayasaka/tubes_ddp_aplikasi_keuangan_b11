/**
 * @file validator.h
 * @brief Header untuk modul validator
 * @author Kelompok B11
 * @date 2025
 */

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "types.h"

/* --- validator_date.h --- */

/**
 * @brief Validasi format tanggal dd-mm-YYYY
 * @param tanggal String tanggal yang akan divalidasi
 * @return 1 jika format valid, 0 jika tidak valid
 * 
 * Hanya mengecek format, bukan kevalidan tanggal
 */
int validasi_format_tanggal(const char *tanggal);

/**
 * @brief Validasi tanggal benar-benar valid (termasuk kabisat)
 * @param d Hari (1-31)
 * @param m Bulan (1-12)
 * @param y Tahun
 * @return 1 jika tanggal valid, 0 jika tidak valid
 */
int validasi_tanggal_valid(int d, int m, int y);

/**
 * @brief Validasi tanggal bukan 00-00-0000
 * @param tanggal String tanggal
 * @return 1 jika bukan tanggal nol, 0 jika tanggal nol atau invalid
 */
int validasi_bukan_nol(const char *tanggal);

/**
 * @brief Validasi tanggal lengkap (format dan kevalidan)
 * @param tanggal String tanggal format dd-mm-YYYY
 * @return 1 jika valid, 0 jika tidak valid
 * 
 * Kombinasi validasi format dan validasi tanggal valid
 */
int validasi_tanggal_lengkap(const char *tanggal);

/**
 * @brief Validasi bulan dalam range 1-12
 * @param bulan Nomor bulan
 * @return 1 jika valid, 0 jika tidak valid
 */
int validasi_bulan(int bulan);

/**
 * @brief Validasi tahun (harus > 0 dan reasonable)
 * @param tahun Tahun yang divalidasi
 * @return 1 jika valid (1900-2100), 0 jika tidak valid
 */
int validasi_tahun(int tahun);

/**
 * @brief REV-3: Validasi bulan pada tanggal sesuai dengan bulan aktif
 * @param tanggal String tanggal format dd-mm-YYYY
 * @param bulan_aktif Bulan yang sedang dipilih (1-12)
 * @return 1 jika bulan sesuai, 0 jika tidak sesuai
 */
int validasi_bulan_sesuai(const char *tanggal, int bulan_aktif);

/* --- validator_number.h --- */

/**
 * @brief Validasi nominal harus positif (> 0)
 * @param nominal Nilai yang akan divalidasi
 * @return 1 jika valid (> 0), 0 jika tidak valid
 */
int validasi_nominal_positif(unsigned long long nominal);

/**
 * @brief Validasi nomor pos dalam range yang valid
 * @param no Nomor pos yang akan divalidasi
 * @param max Nilai maksimal yang diizinkan
 * @return 1 jika valid (1 <= no <= max), 0 jika tidak valid
 */
int validasi_no_pos(int no, int max);

/**
 * @brief Parse string ke unsigned long long dengan validasi
 * @param str String yang berisi angka
 * @param result Pointer untuk menyimpan hasil parsing
 * @return 1 jika berhasil, 0 jika gagal atau format invalid
 */
int parse_nominal(const char *str, unsigned long long *result);

/**
 * @brief Validasi pilihan menu dalam range
 * @param pilihan Pilihan yang akan divalidasi
 * @param min Nilai minimum yang diizinkan
 * @param max Nilai maksimum yang diizinkan
 * @return 1 jika valid, 0 jika tidak valid
 */
int validasi_pilihan_menu(int pilihan, int min, int max);

/**
 * @brief Validasi jenis transaksi (0 atau 1)
 * @param jenis Jenis transaksi
 * @return 1 jika valid (0=Pengeluaran atau 1=Pemasukan), 0 jika tidak
 */
int validasi_jenis_transaksi(int jenis);

/* --- validator_pos.h --- */

/**
 * @brief Validasi nama pos unik (belum ada di daftar)
 * @param nama Nama pos yang akan divalidasi
 * @param list Array pos anggaran yang sudah ada
 * @param count Jumlah pos dalam array
 * @return 1 jika nama unik (belum ada), 0 jika sudah ada
 */
int validasi_pos_unik(const char *nama, PosAnggaran *list, int count);

/**
 * @brief Validasi pos dengan nomor tertentu ada dalam daftar
 * @param no Nomor pos yang dicari
 * @param list Array pos anggaran
 * @param count Jumlah pos dalam array
 * @return 1 jika pos ada, 0 jika tidak ada
 */
int validasi_pos_exists(int no, PosAnggaran *list, int count);

/**
 * @brief Mendapatkan nama pos berdasarkan nomor
 * @param no Nomor pos yang dicari
 * @param list Array pos anggaran
 * @param count Jumlah pos dalam array
 * @param result Buffer untuk menyimpan nama pos (minimal 21 karakter)
 * @return 1 jika berhasil, 0 jika tidak ditemukan
 */
int get_nama_pos_by_no(int no, PosAnggaran *list, int count, char *result);

/**
 * @brief Mendapatkan index pos berdasarkan nama
 * @param nama Nama pos yang dicari
 * @param list Array pos anggaran
 * @param count Jumlah pos dalam array
 * @return Index pos (0-based) jika ditemukan, -1 jika tidak ditemukan
 */
int get_index_pos_by_nama(const char *nama, PosAnggaran *list, int count);

/**
 * @brief Validasi nama pos untuk edit (unik kecuali nama yang sedang diedit)
 * @param nama Nama pos baru
 * @param no_edit Nomor pos yang sedang diedit
 * @param list Array pos anggaran
 * @param count Jumlah pos dalam array
 * @return 1 jika valid (unik atau sama dengan pos yang diedit), 0 jika duplikat
 */
int validasi_pos_unik_edit(const char *nama, int no_edit, PosAnggaran *list, int count);

/**
 * @brief Validasi pos bisa dihapus (tidak ada transaksi terkait)
 * @param no Nomor pos yang akan dihapus
 * @param pos_list Array pos anggaran
 * @param pos_count Jumlah pos
 * @return 1 jika bisa dihapus (jumlah_transaksi == 0), 0 jika tidak
 * 
 * Pos dengan transaksi tidak bisa dihapus untuk menjaga integritas data
 */
int validasi_pos_bisa_hapus(int no, PosAnggaran *pos_list, int pos_count);

/* --- validator_string.h --- */

/**
 * @brief Validasi panjang nama pos anggaran
 * @param nama String nama pos yang akan divalidasi
 * @return 1 jika valid (<= 20 karakter), 0 jika tidak valid
 */
int validasi_panjang_pos(const char *nama);

/**
 * @brief Validasi panjang deskripsi transaksi
 * @param deskripsi String deskripsi yang akan divalidasi
 * @return 1 jika valid (<= 50 karakter), 0 jika tidak valid
 */
int validasi_panjang_deskripsi(const char *deskripsi);

/**
 * @brief Validasi string tidak kosong
 * @param str String yang akan divalidasi
 * @return 1 jika tidak kosong, 0 jika kosong atau NULL
 */
int validasi_tidak_kosong(const char *str);

/**
 * @brief Validasi panjang ID transaksi
 * @param id String ID yang akan divalidasi
 * @return 1 jika valid (format T0000), 0 jika tidak valid
 */
int validasi_format_id(const char *id);

/**
 * @brief Validasi string tidak mengandung karakter pipe (|)
 * @param str String yang akan divalidasi
 * @return 1 jika tidak mengandung pipe, 0 jika mengandung
 * 
 * Karakter pipe digunakan sebagai delimiter file, tidak boleh ada di data
 */
int validasi_tidak_ada_pipe(const char *str);

/**
 * @brief Validasi string hanya mengandung karakter yang diizinkan
 * @param str String yang akan divalidasi
 * @return 1 jika valid, 0 jika mengandung karakter tidak valid
 * 
 * Karakter yang diizinkan: alfanumerik, spasi, dan beberapa simbol umum
 */
int validasi_karakter_valid(const char *str);

#endif /* VALIDATOR_H */
