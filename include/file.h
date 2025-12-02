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

/*
    Function bertujuan untuk memastikan direktori data ada.
    Input : -
    Output : Mengembalikan 1 jika berhasil (direktori ada/dibuat), 0 jika gagal.
*/
int pastikan_direktori_data(void);

/*
    Function bertujuan untuk memeriksa apakah file ada.
    Input : filename (Path file yang akan dicek)
    Output : Mengembalikan 1 jika file ada, 0 jika tidak ada.
*/
int cek_file_ada(const char *filename);

/*
    Function bertujuan untuk membuat file baru (kosong).
    Input : filename (Path file yang akan dibuat)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int buat_file(const char *filename);

/*
    Function bertujuan untuk membuka file untuk dibaca.
    Input : filename (Path file yang akan dibuka)
    Output : Mengembalikan FILE pointer jika berhasil, NULL jika gagal.
*/
FILE* buka_file_baca(const char *filename);

/*
    Function bertujuan untuk membuka file untuk ditulis (overwrite).
    Input : filename (Path file yang akan dibuka)
    Output : Mengembalikan FILE pointer jika berhasil, NULL jika gagal.
*/
FILE* buka_file_tulis(const char *filename);

/*
    Function bertujuan untuk membuka file untuk append (tambah di akhir).
    Input : filename (Path file yang akan dibuka)
    Output : Mengembalikan FILE pointer jika berhasil, NULL jika gagal.
*/
FILE* buka_file_tambah(const char *filename);

/*
    Function bertujuan untuk membaca seluruh isi file ke buffer.
    Input : filename (Path file), buffer_size (Ukuran buffer)
    Output : buffer (Buffer untuk menyimpan isi file). Mengembalikan jumlah karakter yang dibaca, 0 jika gagal atau file kosong.
*/
int baca_semua_file(const char *filename, char *buffer, int buffer_size);

/*
    Function bertujuan untuk menulis string ke file (overwrite).
    Input : filename (Path file tujuan), content (String yang akan ditulis)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int tulis_semua_file(const char *filename, const char *content);

/*
    Function bertujuan untuk menambahkan baris ke akhir file.
    Input : filename (Path file tujuan), line (Baris yang akan ditambahkan)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int tambah_baris_file(const char *filename, const char *line);

/*
    Procedure bertujuan untuk menghasilkan nama file dengan suffix bulan.
    Input : base (Nama dasar file), bulan (Nomor bulan 1-12), result_size (Ukuran buffer result)
    Output : result (Buffer hasil path file lengkap)
*/
void dapatkan_nama_file_bulan(const char *base, int bulan, char *result, int result_size);

/*
    Procedure bertujuan untuk menghasilkan nama file transaksi.
    Input : result_size (Ukuran buffer result)
    Output : result (Buffer hasil path file transaksi)
*/
void dapatkan_nama_file_transaksi(char *result, int result_size);

/*
    Function bertujuan untuk mem-parse baris teks ke struct Transaksi.
    Input : line (Baris teks format pipe-delimited)
    Output : trx (Pointer ke struct Transaksi untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika format invalid.
*/
int urai_baris_transaksi(const char *line, Transaksi *trx);

/*
    Function bertujuan untuk mem-parse baris teks ke struct PosAnggaran.
    Input : line (Baris teks format pipe-delimited)
    Output : pos (Pointer ke struct PosAnggaran untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika format invalid.
*/
int urai_baris_pos(const char *line, PosAnggaran *pos);

/*
    Function bertujuan untuk mem-parse baris teks ke struct AnalisisKeuangan.
    Input : line (Baris teks format pipe-delimited)
    Output : data (Pointer ke struct AnalisisKeuangan untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika format invalid.
*/
int urai_baris_analisis(const char *line, AnalisisKeuangan *data);

/*
    Procedure bertujuan untuk memformat struct Transaksi ke string pipe-delimited.
    Input : trx (Pointer ke struct Transaksi sumber), result_size (Ukuran buffer result)
    Output : result (Buffer hasil string format file)
*/
void format_transaksi_ke_string(const Transaksi *trx, char *result, int result_size);

/*
    Procedure bertujuan untuk memformat struct PosAnggaran ke string pipe-delimited.
    Input : pos (Pointer ke struct PosAnggaran sumber), result_size (Ukuran buffer result)
    Output : result (Buffer hasil string format file)
*/
void format_pos_ke_string(const PosAnggaran *pos, char *result, int result_size);

/*
    Procedure bertujuan untuk memformat struct AnalisisKeuangan ke string pipe-delimited.
    Input : data (Pointer ke struct AnalisisKeuangan sumber), result_size (Ukuran buffer result)
    Output : result (Buffer hasil string format file)
*/
void format_analisis_ke_string(const AnalisisKeuangan *data, char *result, int result_size);

/*
    Function bertujuan untuk mendapatkan label jenis transaksi.
    Input : jenis (Kode jenis transaksi 0 atau 1)
    Output : Mengembalikan string "Pengeluaran" atau "Pemasukan".
*/
const char* dapatkan_label_jenis(int jenis);

/*
    Function bertujuan untuk mengkonversi string jenis ke kode.
    Input : jenis_str (String "Pemasukan" atau "Pengeluaran")
    Output : Mengembalikan 0 untuk Pengeluaran, 1 untuk Pemasukan, -1 jika invalid.
*/
int urai_jenis_transaksi(const char *jenis_str);

/*
    Function bertujuan untuk memuat data analisis keuangan untuk bulan tertentu dari file.
    Input : bulan (Nomor bulan 1-12)
    Output : result (Pointer untuk menyimpan hasil analisis). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int muat_analisis_bulan(int bulan, AnalisisKeuangan *result);

/*
    Function bertujuan untuk menyimpan analisis keuangan untuk bulan tertentu ke file.
    Input : bulan (Nomor bulan 1-12), analisis (Pointer data analisis)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int simpan_analisis_bulan(int bulan, AnalisisKeuangan *analisis);

/*
    Function bertujuan untuk memeriksa apakah file analisis untuk bulan tertentu ada.
    Input : bulan (Nomor bulan 1-12)
    Output : Mengembalikan 1 jika ada, 0 jika tidak.
*/
int cek_file_analisis_ada(int bulan);

/*
    Function bertujuan untuk menginisialisasi analisis default untuk bulan baru.
    Input : bulan (Nomor bulan 1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int inisialisasi_analisis_bulan(int bulan);

/*
    Function bertujuan untuk memuat pos anggaran untuk bulan tertentu dari file.
    Input : max_count (Kapasitas maksimal array), bulan (Nomor bulan 1-12)
    Output : list (Array untuk menyimpan pos). Mengembalikan jumlah pos yang berhasil di-load.
*/
int muat_pos_bulan(PosAnggaran *list, int max_count, int bulan);

/*
    Function bertujuan untuk menyimpan daftar pos anggaran untuk bulan tertentu ke file.
    Input : list (Array pos anggaran), count (Jumlah pos), bulan (Nomor bulan 1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int simpan_pos_bulan(PosAnggaran *list, int count, int bulan);

/*
    Function bertujuan untuk menambahkan pos anggaran baru untuk bulan tertentu.
    Input : pos (Pointer ke pos baru), bulan (Nomor bulan 1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int tambah_pos_ke_file(PosAnggaran *pos, int bulan);

/*
    Function bertujuan untuk mengupdate pos anggaran berdasarkan nomor.
    Input : no (Nomor pos yang diupdate), pos (Data pos baru), bulan (Nomor bulan 1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika tidak ditemukan.
*/
int ubah_pos_di_file(int no, PosAnggaran *pos, int bulan);

/*
    Function bertujuan untuk menghapus pos anggaran berdasarkan nomor.
    Input : no (Nomor pos yang dihapus), bulan (Nomor bulan 1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika tidak ditemukan.
*/
int hapus_pos_dari_file(int no, int bulan);

/*
    Function bertujuan untuk mencari pos anggaran berdasarkan nama.
    Input : nama (Nama pos yang dicari), bulan (Nomor bulan 1-12)
    Output : result (Pointer untuk menyimpan hasil pencarian). Mengembalikan 1 jika ditemukan, 0 jika tidak.
*/
int cari_pos_berdasarkan_nama(const char *nama, int bulan, PosAnggaran *result);

/*
    Function bertujuan untuk mencari pos anggaran berdasarkan nomor.
    Input : no (Nomor pos yang dicari), bulan (Nomor bulan 1-12)
    Output : result (Pointer untuk menyimpan hasil pencarian). Mengembalikan 1 jika ditemukan, 0 jika tidak.
*/
int cari_pos_berdasarkan_nomor(int no, int bulan, PosAnggaran *result);

/*
    Function bertujuan untuk memeriksa apakah file pos untuk bulan tertentu ada.
    Input : bulan (Nomor bulan 1-12)
    Output : Mengembalikan 1 jika ada, 0 jika tidak.
*/
int cek_file_pos_ada(int bulan);

/*
    Function bertujuan untuk menginisialisasi pos default untuk bulan baru.
    Input : bulan (Nomor bulan 1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int inisialisasi_pos_bulan(int bulan);

/*
    Function bertujuan untuk menyalin pos dari bulan sebelumnya.
    Input : bulan_tujuan (Bulan target), bulan_sumber (Bulan sumber)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int salin_pos_dari_bulan(int bulan_tujuan, int bulan_sumber);

/*
    Function bertujuan untuk memuat semua transaksi dari file.
    Input : max_count (Kapasitas maksimal array)
    Output : list (Array untuk menyimpan transaksi). Mengembalikan jumlah transaksi yang berhasil di-load.
*/
int muat_transaksi(Transaksi *list, int max_count);

/*
    Function bertujuan untuk memuat transaksi yang difilter berdasarkan bulan.
    Input : max_count (Kapasitas maksimal array), bulan (Bulan yang difilter 1-12)
    Output : list (Array untuk menyimpan transaksi). Mengembalikan jumlah transaksi yang berhasil di-load.
*/
int muat_transaksi_bulan(Transaksi *list, int max_count, int bulan);

/*
    Function bertujuan untuk menyimpan semua transaksi ke file.
    Input : list (Array transaksi), count (Jumlah transaksi)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int simpan_transaksi(Transaksi *list, int count);

/*
    Function bertujuan untuk menambahkan transaksi baru ke file.
    Input : trx (Pointer ke transaksi baru)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int tambah_transaksi_ke_file(Transaksi *trx);

/*
    Function bertujuan untuk mengupdate transaksi berdasarkan ID.
    Input : id (ID transaksi yang diupdate), trx (Data transaksi baru)
    Output : Mengembalikan 1 jika berhasil, 0 jika tidak ditemukan.
*/
int ubah_transaksi_di_file(const char *id, Transaksi *trx);

/*
    Function bertujuan untuk menghapus transaksi berdasarkan ID.
    Input : id (ID transaksi yang dihapus)
    Output : Mengembalikan 1 jika berhasil, 0 jika tidak ditemukan.
*/
int hapus_transaksi_dari_file(const char *id);

/*
    Function bertujuan untuk mencari transaksi berdasarkan ID.
    Input : id (ID transaksi yang dicari)
    Output : result (Pointer untuk menyimpan hasil pencarian). Mengembalikan 1 jika ditemukan, 0 jika tidak.
*/
int cari_transaksi_berdasarkan_id(const char *id, Transaksi *result);

/*
    Procedure bertujuan untuk menghasilkan ID transaksi baru yang unik.
    Input : -
    Output : buffer (Buffer untuk menyimpan ID baru (format Txxxx))
*/
void buat_id_transaksi(char *buffer);

/*
    Function bertujuan untuk menghitung jumlah transaksi untuk pos tertentu.
    Input : nama_pos (Nama pos yang dihitung)
    Output : Mengembalikan jumlah transaksi.
*/
int hitung_transaksi_berdasarkan_pos(const char *nama_pos);

#endif
