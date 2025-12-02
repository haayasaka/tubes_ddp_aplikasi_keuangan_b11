/**
 * @file analisis.h
 * @brief Header untuk modul analisis keuangan
 * @author Kelompok B11
 * @date 2025
 */

#ifndef ANALISIS_H
#define ANALISIS_H

#include "types.h"

/* --- analisis_calc.h --- */

/**
 * @brief Hitung analisis keuangan lengkap untuk bulan tertentu
 * @param bulan Bulan (1-12)
 * @param result Pointer untuk menyimpan hasil analisis
 * @return 1 jika berhasil, 0 jika gagal
 */
int hitung_analisis_bulan(int bulan, AnalisisKeuangan *result);

/**
 * @brief Tentukan kondisi keuangan berdasarkan saldo
 * @param pemasukan Total pemasukan
 * @param pengeluaran Total pengeluaran
 * @return KONDISI_DEFISIT, KONDISI_SEIMBANG, atau KONDISI_SURPLUS
 */
int tentukan_kondisi_keuangan(unsigned long long pemasukan, 
                               unsigned long long pengeluaran);

/**
 * @brief Tentukan kesimpulan berdasarkan persentase sisa
 * @param persentase_sisa Persentase sisa dari pemasukan
 * @return KESIMPULAN_HEMAT, KESIMPULAN_SEIMBANG, KESIMPULAN_BOROS, 
 *         atau KESIMPULAN_TIDAK_SEHAT
 */
int tentukan_kesimpulan(double persentase_sisa);

/**
 * @brief Dapatkan label kondisi keuangan
 * @param kondisi Kode kondisi
 * @return String label kondisi
 */
const char* get_label_kondisi(int kondisi);

/**
 * @brief Dapatkan label kesimpulan
 * @param kesimpulan Kode kesimpulan
 * @return String label kesimpulan
 */
const char* get_label_kesimpulan(int kesimpulan);

/**
 * @brief Dapatkan deskripsi kesimpulan yang lebih lengkap
 * @param kesimpulan Kode kesimpulan
 * @return String deskripsi kesimpulan
 */
const char* get_deskripsi_kesimpulan(int kesimpulan);

/**
 * @brief Refresh analisis untuk bulan (hitung ulang dan simpan)
 * @param bulan Bulan (1-12)
 * @return 1 jika berhasil, 0 jika gagal
 */
int refresh_analisis_bulan(int bulan);

/* --- analisis_display.h --- */

/**
 * @brief Tampilkan analisis keuangan lengkap untuk bulan
 * @param bulan Bulan (1-12)
 */
void tampilkan_analisis_bulan(int bulan);

/**
 * @brief Tampilkan ringkasan kondisi keuangan
 * @param analisis Pointer ke data analisis
 * @param start_y Baris awal
 * @return Baris berikutnya
 */
int tampilkan_ringkasan_kondisi(AnalisisKeuangan *analisis, int start_y);

/**
 * @brief Tampilkan grafik sederhana perbandingan pemasukan/pengeluaran
 * @param analisis Pointer ke data analisis
 * @param start_y Baris awal
 * @return Baris berikutnya
 */
int tampilkan_grafik_sederhana(AnalisisKeuangan *analisis, int start_y);

/**
 * @brief Tampilkan saran berdasarkan kesimpulan
 * @param kesimpulan Kode kesimpulan
 * @param start_y Baris awal
 * @return Baris berikutnya
 */
int tampilkan_saran(int kesimpulan, int start_y);

/* --- analisis_handler.h --- */

/**
 * @brief Jalankan modul analisis keuangan
 * @param bulan_awal Bulan awal yang dianalisis
 */
void run_analisis_module(int bulan_awal);

/**
 * @brief Menu analisis keuangan
 * @param bulan Bulan yang aktif
 * @return Kode aksi yang dipilih
 */
int menu_analisis_utama(int bulan);

#endif /* ANALISIS_H */
