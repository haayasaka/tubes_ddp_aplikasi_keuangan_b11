/**
 * @file analisis.h
 * @brief Header untuk modul analisis keuangan
 * @author Kelompok B11
 * @date 2025
 */

#ifndef ANALISIS_H
#define ANALISIS_H


/**
 * @brief Struct untuk menyimpan hasil analisis keuangan bulanan
 * 
 * Berisi semua perhitungan dan kesimpulan kondisi keuangan
 */
typedef struct AnalisisKeuangan {
    unsigned long long total_pemasukan;    /* Jumlah seluruh pemasukan */
    unsigned long long total_pengeluaran;  /* Jumlah seluruh pengeluaran */
    double rata_rata_pengeluaran;          /* Rata-rata per transaksi pengeluaran */
    double persentase_sisa;                /* Sisa / total pemasukan * 100 */
    long long saldo_akhir;                 /* Pemasukan - Pengeluaran */
    int kondisi_keuangan;                  /* 0=Defisit, 1=Seimbang, 2=Surplus */
    int kesimpulan;                        /* 0-3 (lihat ketentuan) */
    int total_trx_pengeluaran;             /* Jumlah transaksi pengeluaran */
    int total_trx_pemasukan;               /* Jumlah transaksi pemasukan */
} AnalisisKeuangan;

/* ===== KONSTANTA ANALISIS ===== */
#define FILE_ANALISIS           "analisis_keuangan" /* Nama dasar file analisis */
#define FILE_ANALISIS_PREFIX    "analisis"          /* Prefix file analisis */

/* ===== KONDISI KEUANGAN ===== */
#define KONDISI_DEFISIT         0       /* Pengeluaran > Pemasukan */
#define KONDISI_SEIMBANG        1       /* Pengeluaran = Pemasukan */
#define KONDISI_SURPLUS         2       /* Pemasukan > Pengeluaran */
#define KONDISI_NORMAL          1       /* Kondisi normal/default */

/* ===== KESIMPULAN KEUANGAN ===== */
#define KESIMPULAN_HEMAT        0       /* Sisa > 25% pemasukan */
#define KESIMPULAN_SEIMBANG     1       /* Sisa 0% < x <= 25% */
#define KESIMPULAN_BOROS        2       /* Sisa -10% <= x <= 0% */
#define KESIMPULAN_TIDAK_SEHAT  3       /* Sisa < -10% */

/* --- analisis_calc.h --- */

/*
    Function bertujuan untuk menghitung analisis keuangan lengkap untuk bulan tertentu.
    Input : bulan (1-12)
    Output : result (pointer menyimpan hasil analisis). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int hitung_analisis_bulan(int bulan, AnalisisKeuangan *result);

/*
    Function bertujuan untuk menentukan kondisi keuangan berdasarkan saldo.
    Input : pemasukan, pengeluaran
    Output : Mengembalikan KONDISI_DEFISIT, KONDISI_SEIMBANG, atau KONDISI_SURPLUS.
*/
int tentukan_kondisi_keuangan(unsigned long long pemasukan, 
                               unsigned long long pengeluaran);

/*
    Function bertujuan untuk menentukan kesimpulan berdasarkan persentase sisa.
    Input : persentase_sisa
    Output : Mengembalikan KESIMPULAN_HEMAT, KESIMPULAN_SEIMBANG, KESIMPULAN_BOROS, atau KESIMPULAN_TIDAK_SEHAT.
*/
int tentukan_kesimpulan(double persentase_sisa);

/*
    Function bertujuan untuk mendapatkan label kondisi keuangan.
    Input : kondisi (kode kondisi)
    Output : Mengembalikan string label kondisi.
*/
const char* get_label_kondisi(int kondisi);

/*
    Function bertujuan untuk mendapatkan label kesimpulan.
    Input : kesimpulan (kode kesimpulan)
    Output : Mengembalikan string label kesimpulan.
*/
const char* get_label_kesimpulan(int kesimpulan);

/*
    Function bertujuan untuk mendapatkan deskripsi kesimpulan yang lebih lengkap.
    Input : kesimpulan (kode kesimpulan)
    Output : Mengembalikan string deskripsi kesimpulan.
*/
const char* get_deskripsi_kesimpulan(int kesimpulan);

/*
    Function bertujuan untuk me-refresh analisis untuk bulan (hitung ulang dan simpan).
    Input : bulan (1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int refresh_analisis_bulan(int bulan);

/* --- analisis_display.h --- */

/*
    Procedure bertujuan untuk menampilkan analisis keuangan lengkap untuk bulan.
    I. S. : bulan valid
    F. S. : Analisis ditampilkan di layar.
*/
void tampilkan_analisis_bulan(int bulan);

/*
    Function bertujuan untuk menampilkan ringkasan kondisi keuangan.
    Input : analisis (pointer ke data), start_y (baris awal)
    Output : Mengembalikan posisi baris berikutnya.
*/
int tampilkan_ringkasan_kondisi(AnalisisKeuangan *analisis, int start_y);

/*
    Function bertujuan untuk menampilkan grafik sederhana perbandingan pemasukan/pengeluaran.
    Input : analisis (pointer ke data), start_y (baris awal)
    Output : Mengembalikan posisi baris berikutnya.
*/
int tampilkan_grafik_sederhana(AnalisisKeuangan *analisis, int start_y);

/*
    Function bertujuan untuk menampilkan saran berdasarkan kesimpulan.
    Input : kesimpulan (kode), start_y (baris awal)
    Output : Mengembalikan posisi baris berikutnya.
*/
int tampilkan_saran(int kesimpulan, int start_y);

/* --- analisis_handler.h --- */

/*
    Procedure bertujuan untuk menjalankan modul analisis keuangan.
    I. S. : bulan_awal valid
    F. S. : Modul analisis dijalankan hingga user kembali.
*/
void run_analisis_module(int bulan_awal);

/*
    Function bertujuan untuk menampilkan menu analisis keuangan.
    Input : bulan (aktif)
    Output : Mengembalikan kode aksi yang dipilih.
*/
int menu_analisis_utama(int bulan);

#endif /* ANALISIS_H */
