/**
 * @file utils.h
 * @brief Header untuk modul utilitas
 * @author Kelompok B11
 * @date 2025
 */

#ifndef UTILS_H
#define UTILS_H

#include "types.h"

/* --- utils_array.h --- */

/**
 * @brief Mengalokasikan array Transaksi
 * @param size Jumlah elemen yang akan dialokasikan
 * @return Pointer ke array Transaksi, atau NULL jika gagal
 * 
 * Caller bertanggung jawab untuk membebaskan memory dengan free_transaksi_array()
 */
Transaksi* alloc_transaksi_array(int size);

/**
 * @brief Mengalokasikan array PosAnggaran
 * @param size Jumlah elemen yang akan dialokasikan
 * @return Pointer ke array PosAnggaran, atau NULL jika gagal
 * 
 * Caller bertanggung jawab untuk membebaskan memory dengan free_pos_array()
 */
PosAnggaran* alloc_pos_array(int size);

/**
 * @brief Membebaskan memory array Transaksi
 * @param arr Pointer ke array yang akan dibebaskan
 */
void free_transaksi_array(Transaksi *arr);

/**
 * @brief Membebaskan memory array PosAnggaran
 * @param arr Pointer ke array yang akan dibebaskan
 */
void free_pos_array(PosAnggaran *arr);

/**
 * @brief Menginisialisasi struct ListTransaksi
 * @param list Pointer ke ListTransaksi yang akan diinisialisasi
 * @param initial_capacity Kapasitas awal
 * @return 1 jika berhasil, 0 jika gagal
 */
int init_list_transaksi(ListTransaksi *list, int initial_capacity);

/**
 * @brief Membebaskan memory ListTransaksi
 * @param list Pointer ke ListTransaksi yang akan dibebaskan
 */
void free_list_transaksi(ListTransaksi *list);

/**
 * @brief Menambahkan transaksi ke ListTransaksi
 * @param list Pointer ke ListTransaksi
 * @param trx Pointer ke Transaksi yang akan ditambahkan
 * @return 1 jika berhasil, 0 jika gagal
 * 
 * Fungsi ini akan memperbesar kapasitas jika diperlukan
 */
int add_to_list_transaksi(ListTransaksi *list, const Transaksi *trx);

/**
 * @brief Menginisialisasi struct ListPos
 * @param list Pointer ke ListPos yang akan diinisialisasi
 * @param initial_capacity Kapasitas awal
 * @return 1 jika berhasil, 0 jika gagal
 */
int init_list_pos(ListPos *list, int initial_capacity);

/**
 * @brief Membebaskan memory ListPos
 * @param list Pointer ke ListPos yang akan dibebaskan
 */
void free_list_pos(ListPos *list);

/**
 * @brief Menambahkan pos anggaran ke ListPos
 * @param list Pointer ke ListPos
 * @param pos Pointer ke PosAnggaran yang akan ditambahkan
 * @return 1 jika berhasil, 0 jika gagal
 * 
 * Fungsi ini akan memperbesar kapasitas jika diperlukan
 */
int add_to_list_pos(ListPos *list, const PosAnggaran *pos);

/**
 * @brief Menginisialisasi struct Transaksi ke nilai default
 * @param trx Pointer ke Transaksi yang akan diinisialisasi
 */
void init_transaksi(Transaksi *trx);

/**
 * @brief Menginisialisasi struct PosAnggaran ke nilai default
 * @param pos Pointer ke PosAnggaran yang akan diinisialisasi
 */
void init_pos_anggaran(PosAnggaran *pos);

/**
 * @brief Menginisialisasi struct AnalisisKeuangan ke nilai default
 * @param analisis Pointer ke AnalisisKeuangan yang akan diinisialisasi
 */
void init_analisis_keuangan(AnalisisKeuangan *analisis);

/* --- utils_bulan.h --- */

/**
 * @brief Mendapatkan nama bulan dalam Bahasa Indonesia
 * @param bulan Nomor bulan (1-12)
 * @return Pointer ke string nama bulan (statis), atau "Invalid" jika di luar range
 * 
 * Contoh: get_nama_bulan(1) -> "Januari"
 */
const char* get_nama_bulan(int bulan);

/**
 * @brief Mendapatkan kode bulan 2 digit
 * @param bulan Nomor bulan (1-12)
 * @return Pointer ke string kode bulan (statis), atau "00" jika di luar range
 * 
 * Contoh: get_kode_bulan(1) -> "01", get_kode_bulan(12) -> "12"
 */
const char* get_kode_bulan(int bulan);

/**
 * @brief Mendapatkan nama bulan singkat (3 huruf)
 * @param bulan Nomor bulan (1-12)
 * @return Pointer ke string nama bulan singkat (statis)
 * 
 * Contoh: get_nama_bulan_singkat(1) -> "Jan"
 */
const char* get_nama_bulan_singkat(int bulan);

/**
 * @brief Mengkonversi nama bulan ke nomor bulan
 * @param nama Nama bulan (case-insensitive)
 * @return Nomor bulan (1-12), atau 0 jika tidak valid
 * 
 * Contoh: nama_ke_nomor_bulan("januari") -> 1
 */
int nama_ke_nomor_bulan(const char *nama);

/**
 * @brief Validasi nomor bulan
 * @param bulan Nomor bulan yang akan divalidasi
 * @return 1 jika valid (1-12), 0 jika tidak valid
 */
int is_valid_bulan(int bulan);

/* --- utils_date.h --- */

/**
 * @brief Parse string tanggal ke komponen hari, bulan, tahun
 * @param str String tanggal format dd-mm-YYYY
 * @param d Pointer untuk menyimpan hari
 * @param m Pointer untuk menyimpan bulan
 * @param y Pointer untuk menyimpan tahun
 * @return 1 jika berhasil parse, 0 jika format invalid
 */
int parse_tanggal(const char *str, int *d, int *m, int *y);

/**
 * @brief Parse string tanggal ke struct Tanggal
 * @param str String tanggal format dd-mm-YYYY
 * @param tgl Pointer ke struct Tanggal untuk menyimpan hasil
 * @return 1 jika berhasil parse, 0 jika format invalid
 */
int parse_tanggal_struct(const char *str, Tanggal *tgl);

/**
 * @brief Format komponen tanggal ke string dd-mm-YYYY
 * @param d Hari (1-31)
 * @param m Bulan (1-12)
 * @param y Tahun (misal 2025)
 * @param result Buffer untuk menyimpan hasil (minimal 11 karakter)
 */
void format_tanggal(int d, int m, int y, char *result);

/**
 * @brief Mendapatkan bulan saat ini dari sistem
 * @return Bulan saat ini (1-12)
 */
int get_current_month(void);

/**
 * @brief Mendapatkan tahun saat ini dari sistem
 * @return Tahun saat ini
 */
int get_current_year(void);

/**
 * @brief Mendapatkan hari saat ini dari sistem
 * @return Hari saat ini (1-31)
 */
int get_current_day(void);

/**
 * @brief Memeriksa apakah tahun adalah tahun kabisat
 * @param year Tahun yang akan diperiksa
 * @return 1 jika tahun kabisat, 0 jika bukan
 */
int is_leap_year(int year);

/**
 * @brief Mendapatkan jumlah hari dalam bulan tertentu
 * @param month Bulan (1-12)
 * @param year Tahun (untuk pengecekan kabisat)
 * @return Jumlah hari dalam bulan
 */
int get_days_in_month(int month, int year);

/**
 * @brief Mengekstrak bulan dari string tanggal
 * @param tanggal String tanggal format dd-mm-YYYY
 * @return Nomor bulan (1-12), atau 0 jika gagal
 */
int ekstrak_bulan(const char *tanggal);

/**
 * @brief Mengekstrak tahun dari string tanggal
 * @param tanggal String tanggal format dd-mm-YYYY
 * @return Tahun, atau 0 jika gagal
 */
int ekstrak_tahun(const char *tanggal);

/**
 * @brief Mendapatkan tanggal hari ini dalam format dd-mm-YYYY
 * @param result Buffer untuk menyimpan hasil (minimal 11 karakter)
 */
void get_today_string(char *result);

/* --- utils_number.h --- */

/**
 * @brief Memformat nominal ke format Rupiah dengan separator ribuan
 * @param nominal Nilai nominal yang akan diformat
 * @param result Buffer untuk menyimpan hasil (minimal 30 karakter)
 * 
 * Contoh: 1000000 -> "Rp 1.000.000"
 */
void format_rupiah(unsigned long long nominal, char *result);

/**
 * @brief Memformat persentase dengan 2 desimal
 * @param persen Nilai persentase
 * @param result Buffer untuk menyimpan hasil (minimal 15 karakter)
 * 
 * Contoh: 62.14159 -> "62.14%"
 */
void format_persentase(double persen, char *result);

/**
 * @brief Memformat nilai rata-rata dengan 2 desimal
 * @param rata Nilai rata-rata
 * @param result Buffer untuk menyimpan hasil (minimal 20 karakter)
 * 
 * Contoh: 29444.33 -> "29.444,33"
 */
void format_rata_rata(double rata, char *result);

/**
 * @brief Memformat angka dengan separator ribuan (tanpa prefix Rp)
 * @param nominal Nilai yang akan diformat
 * @param result Buffer untuk menyimpan hasil (minimal 25 karakter)
 * 
 * Contoh: 1000000 -> "1.000.000"
 */
void format_number_separator(unsigned long long nominal, char *result);

/**
 * @brief Memformat nilai saldo (bisa negatif) ke format Rupiah
 * @param saldo Nilai saldo (bisa negatif)
 * @param result Buffer untuk menyimpan hasil (minimal 30 karakter)
 * 
 * Contoh: -100000 -> "-Rp 100.000"
 */
void format_saldo(long long saldo, char *result);

/**
 * @brief Mengkonversi string angka ke unsigned long long
 * @param str String yang berisi angka
 * @param result Pointer untuk menyimpan hasil konversi
 * @return 1 jika berhasil, 0 jika gagal
 */
int str_to_ull(const char *str, unsigned long long *result);

/**
 * @brief Memeriksa apakah string hanya berisi digit
 * @param str String yang akan diperiksa
 * @return 1 jika hanya digit, 0 jika tidak
 */
int is_numeric_string(const char *str);

/* --- utils_string.h --- */

/**
 * @brief Menghapus whitespace di awal dan akhir string
 * @param str String yang akan di-trim (dimodifikasi in-place)
 */
void str_trim(char *str);

/**
 * @brief Mengkonversi string ke uppercase
 * @param str String yang akan dikonversi (dimodifikasi in-place)
 */
void str_to_upper(char *str);

/**
 * @brief Mengkonversi string ke lowercase
 * @param str String yang akan dikonversi (dimodifikasi in-place)
 */
void str_to_lower(char *str);

/**
 * @brief Menyalin string dengan batasan ukuran (safe copy)
 * @param dst Buffer tujuan
 * @param src String sumber
 * @param max Ukuran maksimal buffer tujuan
 */
void str_copy_safe(char *dst, const char *src, int max);

/**
 * @brief Memisahkan string berdasarkan delimiter
 * @param str String yang akan dipisahkan
 * @param delim Karakter delimiter
 * @param result Array pointer untuk menyimpan hasil split
 * @param max_parts Jumlah maksimal bagian yang dihasilkan
 * @return Jumlah bagian yang dihasilkan
 */
int str_split(char *str, char delim, char **result, int max_parts);

/**
 * @brief Memeriksa apakah string kosong (hanya whitespace atau NULL)
 * @param str String yang diperiksa
 * @return 1 jika kosong, 0 jika tidak
 */
int str_is_empty(const char *str);

/**
 * @brief Membandingkan dua string secara case-insensitive
 * @param s1 String pertama
 * @param s2 String kedua
 * @return 0 jika sama, non-zero jika berbeda
 */
int str_compare_nocase(const char *s1, const char *s2);

/**
 * @brief Menghapus karakter newline dari akhir string
 * @param str String yang akan diproses (dimodifikasi in-place)
 */
void str_remove_newline(char *str);

#endif /* UTILS_H */
