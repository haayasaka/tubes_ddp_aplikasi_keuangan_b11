#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "file.h"
#include "tui.h"
#include "pos.h"
#include "transaksi.h"
#include "analisis.h"
#include "utils.h"

/* ===== KONSTANTA LOKAL ===== */
/* Aksi Menu */
#define ACT_TRANSAKSI   1
#define ACT_POS         2
#define ACT_ANALISIS    3
#define ACT_BULAN       4
#define ACT_BANTUAN     5
#define ACT_TENTANG     6
#define ACT_KELUAR      0

/* ===== VARIABEL GLOBAL LOKAL ===== */
static int bulan_aktif = 0;

/* ===== DEKLARASI FUNGSI LOKAL ===== */
static void tampilkan_splash_screen(void);
static int menu_utama(void);
static int pilih_bulan_global(int bulan_saat_ini);
static void tampilkan_tentang(void);
static void tampilkan_bantuan(void);

/* ===== FUNGSI UTAMA ===== */

/**
 * @brief Fungsi utama - titik masuk aplikasi
 * @return 0 jika sukses, 1 jika error
 */
int main(void) {
    /* Atur locale untuk mendukung karakter khusus */
    setlocale(LC_ALL, "");

    /* Pastikan direktori data ada */
    if (!pastikan_direktori_data()) {
        fprintf(stderr, "Error: Tidak dapat membuat direktori data.\n");
        return 1;
    }

    /* Inisialisasi TUI */
    tui_inisialisasi();
    tui_inisialisasi_warna();

    /* Atur bulan aktif ke bulan saat ini */
    bulan_aktif = dapatkan_bulan_saat_ini();

    /* Tampilkan splash screen */
    tampilkan_splash_screen();

    /* Loop Utama */
    int berjalan = 1;
    while (berjalan) {
        int aksi = menu_utama();

        switch (aksi) {
            case ACT_TRANSAKSI:
                jalankan_modul_transaksi(bulan_aktif);
                break;

            case ACT_POS:
                jalankan_modul_pos(bulan_aktif);
                break;

            case ACT_ANALISIS:
                jalankan_modul_analisis(bulan_aktif);
                break;

            case ACT_BULAN:
                bulan_aktif = pilih_bulan_global(bulan_aktif);
                break;

            case ACT_BANTUAN:
                tampilkan_bantuan();
                break;

            case ACT_TENTANG:
                tampilkan_tentang();
                break;

            case ACT_KELUAR:
            case CANCEL:
                if (tampilkan_konfirmasi("Keluar dari aplikasi?")) {
                    berjalan = 0;
                }
                break;
        }
    }

    /* Pembersihan */
    tui_bersihkan();

    return 0;
}

/* ===== IMPLEMENTASI FUNGSI LOKAL ===== */

/**
 * Tampilkan splash screen aplikasi
 * I.S.: Layar tui terinisialisasi
 * F.S.: Splash screen ditampilkan, menunggu input user
 */
static void tampilkan_splash_screen(void) {
    tui_hapus_layar();

    int center_y = tui_ambil_tinggi() / 2 - 5;
    int center_x = tui_ambil_lebar() / 2;

    /* Logo/Spanduk */
    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_aktifkan_tebal();

    const char *banner[] = {
        "╔═══════════════════════════════════════════════════╗",
        "║                                                   ║",
        "║           APLIKASI KEUANGAN MAHASISWA             ║",
        "║                                                   ║",
        "║           Kelola Keuangan dengan Mudah            ║",
        "║                                                   ║",
        "╚═══════════════════════════════════════════════════╝"
    };

    int banner_width = 53;
    int start_x = center_x - banner_width / 2;

    for (int i = 0; i < 7; i++) {
        tui_cetak(center_y + i, start_x, banner[i]);
    }

    tui_nonaktifkan_tebal();
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);

    /* Subjudul */
    tui_aktifkan_warna(COLOR_PAIR_YELLOW);
    tui_cetak_tengah(center_y + 8, "- Dibuat oleh Kelompok B11 -");
    tui_nonaktifkan_warna(COLOR_PAIR_YELLOW);

    /* Versi */
    tui_cetak_tengah(center_y + 10, "Versi 2.0");

    /* Prompt */
    tui_aktifkan_warna(COLOR_PAIR_GREEN);
    tui_cetak_tengah(center_y + 13, "Tekan sembarang tombol untuk melanjutkan...");
    tui_nonaktifkan_warna(COLOR_PAIR_GREEN);

    tui_segarkan();
    tui_ambil_karakter();
}

static int menu_utama(void) {
    char judul[80];
    snprintf(judul, sizeof(judul), "MENU UTAMA - Bulan: %s", dapatkan_nama_bulan(bulan_aktif));

    Menu menu;
    menu_inisialisasi(&menu, judul);

    menu_tambah_item(&menu, "Kelola Transaksi", ACT_TRANSAKSI);
    menu_tambah_item(&menu, "Kelola Pos Anggaran", ACT_POS);
    menu_tambah_item(&menu, "Analisis Keuangan", ACT_ANALISIS);
    menu_tambah_item(&menu, "Ganti Bulan", ACT_BULAN);
    menu_tambah_item(&menu, "Bantuan", ACT_BANTUAN);
    menu_tambah_item(&menu, "Tentang", ACT_TENTANG);
    menu_tambah_item(&menu, "Keluar", ACT_KELUAR);

    return menu_navigasi(&menu);
}

static int pilih_bulan_global(int bulan_saat_ini) {
    return menu_pilih_bulan(bulan_saat_ini);
}

/**
 * Tampilkan tentang aplikasi
 * I.S.: Layar menu
 * F.S.: Halaman tentang ditampilkan, menunggu input user
 */
static void tampilkan_tentang(void) {
    tui_hapus_layar();
    tampilkan_header("TENTANG APLIKASI");

    int y = 5;

    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_aktifkan_tebal();
    tui_cetak(y++, 2, "APLIKASI KEUANGAN MAHASISWA");
    tui_nonaktifkan_tebal();
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);

    tui_cetak(y++, 2, "Versi 1.0");
    y++;

    tui_gambar_garis_horizontal(y++, 2, 50, '-');

    tui_cetak(y++, 2, "Dibuat oleh: Kelompok B11");
    tui_cetak(y++, 2, "Mata Kuliah: Dasar Pemrograman");
    tui_cetak(y++, 2, "Tahun: 2025");
    y++;

    tui_gambar_garis_horizontal(y++, 2, 50, '-');

    tui_aktifkan_tebal();
    tui_cetak(y++, 2, "Fitur Utama:");
    tui_nonaktifkan_tebal();

    tui_cetak(y++, 4, "* Pencatatan transaksi pemasukan & pengeluaran");
    tui_cetak(y++, 4, "* Pengelolaan pos anggaran bulanan");
    tui_cetak(y++, 4, "* Analisis kondisi keuangan otomatis");
    tui_cetak(y++, 4, "* Grafik perbandingan pemasukan/pengeluaran");
    tui_cetak(y++, 4, "* Saran pengelolaan keuangan");
    y++;

    tui_gambar_garis_horizontal(y++, 2, 50, '-');

    tui_aktifkan_warna(COLOR_PAIR_YELLOW);
    tui_cetak(y++, 2, "Terima kasih telah menggunakan aplikasi ini!");
    tui_nonaktifkan_warna(COLOR_PAIR_YELLOW);

    tampilkan_footer("Tekan sembarang tombol untuk kembali");
    tui_segarkan();
    tui_ambil_karakter();
}

/**
 * Tampilkan bantuan penggunaan
 * I.S.: Layar menu
 * F.S.: Halaman bantuan ditampilkan, menunggu input user
 */
static void tampilkan_bantuan(void) {
    tui_hapus_layar();
    tampilkan_header("BANTUAN PENGGUNAAN");

    int y = 5;

    tui_aktifkan_tebal();
    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_cetak(y++, 2, "NAVIGASI:");
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);
    tui_nonaktifkan_tebal();

    tui_cetak(y++, 4, "ATAS/BAWAH atau k/j  : Pindah pilihan menu");
    tui_cetak(y++, 4, "ENTER                : Pilih/Konfirmasi");
    tui_cetak(y++, 4, "ESC                  : Kembali/Batal");
    tui_cetak(y++, 4, "1-9                  : Pilih langsung item menu");
    y++;

    tui_aktifkan_tebal();
    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_cetak(y++, 2, "MENU TRANSAKSI:");
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);
    tui_nonaktifkan_tebal();

    tui_cetak(y++, 4, "* Tambah transaksi pemasukan atau pengeluaran");
    tui_cetak(y++, 4, "* Edit data transaksi yang sudah ada");
    tui_cetak(y++, 4, "* Hapus transaksi");
    tui_cetak(y++, 4, "* Lihat daftar dan detail transaksi");
    y++;

    tui_aktifkan_tebal();
    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_cetak(y++, 2, "MENU POS ANGGARAN:");
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);
    tui_nonaktifkan_tebal();

    tui_cetak(y++, 4, "* Buat pos anggaran baru dengan batas nominal");
    tui_cetak(y++, 4, "* Edit nama atau nominal pos");
    tui_cetak(y++, 4, "* Hapus pos (hanya jika tidak ada transaksi)");
    tui_cetak(y++, 4, "* Lihat realisasi dan sisa anggaran");
    y++;

    tui_aktifkan_tebal();
    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_cetak(y++, 2, "MENU ANALISIS:");
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);
    tui_nonaktifkan_tebal();

    tui_cetak(y++, 4, "* Lihat ringkasan keuangan bulanan");
    tui_cetak(y++, 4, "* Grafik perbandingan pemasukan/pengeluaran");
    tui_cetak(y++, 4, "* Kesimpulan dan saran otomatis");

    tampilkan_footer("Tekan sembarang tombol untuk kembali");
    tui_segarkan();
    tui_ambil_karakter();
}
