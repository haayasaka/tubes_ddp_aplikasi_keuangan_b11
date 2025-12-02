/**
 * @file main.c
 * @brief Entry point Aplikasi Keuangan Mahasiswa
 * @author Kelompok B11
 * @date 2025
 * 
 * Program ini adalah aplikasi pencatatan keuangan berbasis TUI (Text User Interface)
 * yang membantu mahasiswa mengelola keuangan bulanan mereka.
 * 
 * Fitur utama:
 * - Pencatatan transaksi pemasukan dan pengeluaran
 * - Pengelolaan pos anggaran dengan tracking realisasi
 * - Analisis kondisi keuangan dengan grafik dan saran
 * 
 * Penggunaan:
 *   ./keuangan
 * 
 * Dependencies:
 *   - ncurses library
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "app_menu.h"
#include "file.h"

/**
 * @brief Main function - entry point aplikasi
 * @return 0 jika sukses, 1 jika error
 */
int main(void) {
    /* Set locale untuk mendukung karakter khusus */
    setlocale(LC_ALL, "");
    
    /* Pastikan direktori data ada */
    if (!ensure_data_directory()) {
        fprintf(stderr, "Error: Tidak dapat membuat direktori data.\n");
        return 1;
    }
    
    /* Jalankan aplikasi */
    run_aplikasi();
    
    return 0;
}
