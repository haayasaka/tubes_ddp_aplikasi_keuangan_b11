#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "pos.h"

/*
    Function bertujuan untuk memvalidasi format tanggal dd-mm-YYYY.
    Input : tanggal (String tanggal)
    Output : Mengembalikan 1 jika format valid, 0 jika tidak valid.
*/
int validasi_format_tanggal(const char *tanggal);

/*
    Function bertujuan untuk memvalidasi kebenaran tanggal (hari, bulan, tahun).
    Input : d (Hari), m (Bulan), y (Tahun)
    Output : Mengembalikan 1 jika tanggal valid, 0 jika tidak valid.
*/
int validasi_tanggal_sah(int d, int m, int y);

/*
    Function bertujuan untuk memvalidasi tanggal bukan 00-00-0000.
    Input : tanggal (String tanggal)
    Output : Mengembalikan 1 jika bukan tanggal nol, 0 jika tanggal nol atau invalid.
*/
int validasi_bukan_nol(const char *tanggal);

/*
    Function bertujuan untuk memvalidasi tanggal secara lengkap (format dan kevalidan).
    Input : tanggal (String tanggal format dd-mm-YYYY)
    Output : Mengembalikan 1 jika valid, 0 jika tidak valid.
*/
int validasi_tanggal_lengkap(const char *tanggal);

/*
    Function bertujuan untuk memvalidasi bulan dalam range 1-12.
    Input : bulan (Nomor bulan)
    Output : Mengembalikan 1 jika valid, 0 jika tidak valid.
*/
int validasi_bulan(int bulan);

/*
    Function bertujuan untuk memvalidasi tahun (harus > 0 dan wajar).
    Input : tahun (Tahun yang divalidasi)
    Output : Mengembalikan 1 jika valid (1900-2100), 0 jika tidak valid.
*/
int validasi_tahun(int tahun);

/*
    Function bertujuan untuk memvalidasi bulan pada tanggal sesuai dengan bulan aktif.
    Input : tanggal (String tanggal format dd-mm-YYYY), bulan_aktif (Bulan yang sedang dipilih)
    Output : Mengembalikan 1 jika bulan sesuai, 0 jika tidak sesuai.
*/
int validasi_bulan_sesuai(const char *tanggal, int bulan_aktif);

/*
    Function bertujuan untuk memvalidasi nominal harus positif (> 0).
    Input : nominal (Nilai yang akan divalidasi)
    Output : Mengembalikan 1 jika valid (> 0), 0 jika tidak valid.
*/
int validasi_nominal_positif(unsigned long long nominal);

/*
    Function bertujuan untuk memvalidasi nomor pos dalam range yang valid.
    Input : no (Nomor pos), max (Nilai maksimal yang diizinkan)
    Output : Mengembalikan 1 jika valid (1 <= no <= max), 0 jika tidak valid.
*/
int validasi_no_pos(int no, int max);

/*
    Function bertujuan untuk mem-parse string ke unsigned long long dengan validasi.
    Input : str (String yang berisi angka)
    Output : result (Pointer untuk menyimpan hasil parsing). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int urai_nominal(const char *str, unsigned long long *result);

/*
    Function bertujuan untuk memvalidasi pilihan menu dalam range.
    Input : pilihan (Pilihan yang divalidasi), min (Nilai minimum), max (Nilai maksimum)
    Output : Mengembalikan 1 jika valid, 0 jika tidak valid.
*/
int validasi_pilihan_menu(int pilihan, int min, int max);

/*
    Function bertujuan untuk memvalidasi jenis transaksi (0 atau 1).
    Input : jenis (Jenis transaksi)
    Output : Mengembalikan 1 jika valid (0=Pengeluaran atau 1=Pemasukan), 0 jika tidak.
*/
int validasi_jenis_transaksi(int jenis);

/*
    Function bertujuan untuk memvalidasi nama pos unik (belum ada di daftar).
    Input : nama (Nama pos), list (Array pos anggaran), count (Jumlah pos)
    Output : Mengembalikan 1 jika nama unik (belum ada), 0 jika sudah ada.
*/
int validasi_pos_unik(const char *nama, PosAnggaran *list, int count);

/*
    Function bertujuan untuk memvalidasi pos dengan nomor tertentu ada dalam daftar.
    Input : no (Nomor pos), list (Array pos anggaran), count (Jumlah pos)
    Output : Mengembalikan 1 jika pos ada, 0 jika tidak ada.
*/
int validasi_pos_ada(int no, PosAnggaran *list, int count);

/*
    Function bertujuan untuk mendapatkan nama pos berdasarkan nomor.
    Input : no (Nomor pos), list (Array pos anggaran), count (Jumlah pos)
    Output : result (Buffer nama pos via pointer). Mengembalikan 1 jika berhasil, 0 jika tidak ditemukan.
*/
int dapatkan_nama_pos_berdasarkan_no(int no, PosAnggaran *list, int count, char *result);

/*
    Function bertujuan untuk mendapatkan index pos berdasarkan nama.
    Input : nama (Nama pos), list (Array pos anggaran), count (Jumlah pos)
    Output : Mengembalikan index pos (0-based) jika ditemukan, -1 jika tidak ditemukan.
*/
int dapatkan_index_pos_berdasarkan_nama(const char *nama, PosAnggaran *list, int count);

/*
    Function bertujuan untuk memvalidasi nama pos untuk edit (unik kecuali nama yang sedang diedit).
    Input : nama (Nama pos baru), no_edit (Nomor pos yang diedit), list (Array pos anggaran), count (Jumlah pos)
    Output : Mengembalikan 1 jika valid (unik atau sama dengan pos yang diedit), 0 jika duplikat.
*/
int validasi_pos_unik_edit(const char *nama, int no_edit, PosAnggaran *list, int count);

/*
    Function bertujuan untuk memvalidasi apakah pos bisa dihapus (tidak ada transaksi terkait).
    Input : no (Nomor pos), pos_list (Array pos anggaran), pos_count (Jumlah pos)
    Output : Mengembalikan 1 jika bisa dihapus (jumlah_transaksi == 0), 0 jika tidak.
*/
int validasi_pos_bisa_hapus(int no, PosAnggaran *pos_list, int pos_count);

/*
    Function bertujuan untuk memvalidasi panjang nama pos anggaran.
    Input : nama (String nama pos)
    Output : Mengembalikan 1 jika valid (<= 20 karakter), 0 jika tidak valid.
*/
int validasi_panjang_pos(const char *nama);

/*
    Function bertujuan untuk memvalidasi panjang deskripsi transaksi.
    Input : deskripsi (String deskripsi)
    Output : Mengembalikan 1 jika valid (<= 50 karakter), 0 jika tidak valid.
*/
int validasi_panjang_deskripsi(const char *deskripsi);

/*
    Function bertujuan untuk memvalidasi string tidak kosong.
    Input : str (String yang divalidasi)
    Output : Mengembalikan 1 jika tidak kosong, 0 jika kosong atau NULL.
*/
int validasi_tidak_kosong(const char *str);

/*
    Function bertujuan untuk memvalidasi format ID transaksi (T0000).
    Input : id (String ID)
    Output : Mengembalikan 1 jika valid, 0 jika tidak valid.
*/
int validasi_format_id(const char *id);

/*
    Function bertujuan untuk memvalidasi string tidak mengandung karakter pipe (|).
    Input : str (String yang divalidasi)
    Output : Mengembalikan 1 jika tidak mengandung pipe, 0 jika mengandung.
*/
int validasi_tidak_ada_pipe(const char *str);

/*
    Function bertujuan untuk memvalidasi string hanya mengandung karakter yang diizinkan.
    Input : str (String yang divalidasi)
    Output : Mengembalikan 1 jika valid, 0 jika mengandung karakter tidak valid.
*/
int validasi_karakter_sah(const char *str);

#endif
