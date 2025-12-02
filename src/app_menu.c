/**
 * @file app_menu.c
 * @brief Implementasi menu utama aplikasi
 */

#include <stdio.h>
#include <string.h>
#include "app_menu.h"
#include "tui.h"
#include "pos.h"
#include "transaksi.h"
#include "analisis.h"
#include "utils.h"
#include "constants.h"

/* Menu actions */
#define ACT_TRANSAKSI   1
#define ACT_POS         2
#define ACT_ANALISIS    3
#define ACT_BULAN       4
#define ACT_BANTUAN     5
#define ACT_TENTANG     6
#define ACT_KELUAR      0

static int bulan_aktif = 0;

void tampilkan_splash_screen(void) {
    tui_clear();

    int center_y = tui_get_height() / 2 - 5;
    int center_x = tui_get_width() / 2;

    /* Logo/Banner */
    tui_color_on(COLOR_PAIR_CYAN);
    tui_bold_on();

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
        tui_print(center_y + i, start_x, banner[i]);
    }

    tui_bold_off();
    tui_color_off(COLOR_PAIR_CYAN);

    /* Subtitle */
    tui_color_on(COLOR_PAIR_YELLOW);
    tui_print_center(center_y + 8, "- Created by Kelompok B11 -");
    tui_color_off(COLOR_PAIR_YELLOW);

    /* Version */
    tui_print_center(center_y + 10, "Versi 2.0");

    /* Prompt */
    tui_color_on(COLOR_PAIR_GREEN);
    tui_print_center(center_y + 13, "Tekan sembarang tombol untuk melanjutkan...");
    tui_color_off(COLOR_PAIR_GREEN);

    tui_refresh();
    tui_getch();
}

int menu_utama(void) {
    char title[80];
    snprintf(title, sizeof(title), "MENU UTAMA - Bulan: %s", get_nama_bulan(bulan_aktif));

    Menu menu;
    menu_init(&menu, title);

    menu_add_item(&menu, "Kelola Transaksi", ACT_TRANSAKSI);
    menu_add_item(&menu, "Kelola Pos Anggaran", ACT_POS);
    menu_add_item(&menu, "Analisis Keuangan", ACT_ANALISIS);
    menu_add_item(&menu, "Ganti Bulan", ACT_BULAN);
    menu_add_item(&menu, "Bantuan", ACT_BANTUAN);
    menu_add_item(&menu, "Tentang", ACT_TENTANG);
    menu_add_item(&menu, "Keluar", ACT_KELUAR);

    return menu_navigate(&menu);
}

int pilih_bulan_global(int bulan_saat_ini) {
    return menu_pilih_bulan(bulan_saat_ini);
}

void run_aplikasi(void) {
    /* Inisialisasi TUI */
    tui_init();
    tui_init_colors();

    /* Set bulan aktif ke bulan saat ini */
    bulan_aktif = get_current_month();

    /* Tampilkan splash screen */
    tampilkan_splash_screen();

    /* Main loop */
    int running = 1;
    while (running) {
        int action = menu_utama();

        switch (action) {
            case ACT_TRANSAKSI:
                run_transaksi_module(bulan_aktif);
                break;

            case ACT_POS:
                run_pos_module(bulan_aktif);
                break;

            case ACT_ANALISIS:
                run_analisis_module(bulan_aktif);
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
                if (show_confirm("Keluar dari aplikasi?")) {
                    running = 0;
                }
                break;
        }
    }

    /* Cleanup */
    tui_cleanup();
}

void tampilkan_tentang(void) {
    tui_clear();
    display_header("TENTANG APLIKASI");

    int y = 5;

    tui_color_on(COLOR_PAIR_CYAN);
    tui_bold_on();
    tui_print(y++, 2, "APLIKASI KEUANGAN MAHASISWA");
    tui_bold_off();
    tui_color_off(COLOR_PAIR_CYAN);

    tui_print(y++, 2, "Versi 1.0");
    y++;

    tui_draw_hline(y++, 2, 50, '-');

    tui_print(y++, 2, "Dibuat oleh: Kelompok B11");
    tui_print(y++, 2, "Mata Kuliah: Dasar Pemrograman");
    tui_print(y++, 2, "Tahun: 2025");
    y++;

    tui_draw_hline(y++, 2, 50, '-');

    tui_bold_on();
    tui_print(y++, 2, "Fitur Utama:");
    tui_bold_off();

    tui_print(y++, 4, "* Pencatatan transaksi pemasukan & pengeluaran");
    tui_print(y++, 4, "* Pengelolaan pos anggaran bulanan");
    tui_print(y++, 4, "* Analisis kondisi keuangan otomatis");
    tui_print(y++, 4, "* Grafik perbandingan pemasukan/pengeluaran");
    tui_print(y++, 4, "* Saran pengelolaan keuangan");
    y++;

    tui_draw_hline(y++, 2, 50, '-');

    tui_color_on(COLOR_PAIR_YELLOW);
    tui_print(y++, 2, "Terima kasih telah menggunakan aplikasi ini!");
    tui_color_off(COLOR_PAIR_YELLOW);

    display_footer("Tekan sembarang tombol untuk kembali");
    tui_refresh();
    tui_getch();
}

void tampilkan_bantuan(void) {
    tui_clear();
    display_header("BANTUAN PENGGUNAAN");

    int y = 5;

    tui_bold_on();
    tui_color_on(COLOR_PAIR_CYAN);
    tui_print(y++, 2, "NAVIGASI:");
    tui_color_off(COLOR_PAIR_CYAN);
    tui_bold_off();

    tui_print(y++, 4, "UP/DOWN atau k/j  : Pindah pilihan menu");
    tui_print(y++, 4, "ENTER             : Pilih/Konfirmasi");
    tui_print(y++, 4, "ESC               : Kembali/Batal");
    tui_print(y++, 4, "1-9               : Pilih langsung item menu");
    y++;

    tui_bold_on();
    tui_color_on(COLOR_PAIR_CYAN);
    tui_print(y++, 2, "MENU TRANSAKSI:");
    tui_color_off(COLOR_PAIR_CYAN);
    tui_bold_off();

    tui_print(y++, 4, "* Tambah transaksi pemasukan atau pengeluaran");
    tui_print(y++, 4, "* Edit data transaksi yang sudah ada");
    tui_print(y++, 4, "* Hapus transaksi");
    tui_print(y++, 4, "* Lihat daftar dan detail transaksi");
    y++;

    tui_bold_on();
    tui_color_on(COLOR_PAIR_CYAN);
    tui_print(y++, 2, "MENU POS ANGGARAN:");
    tui_color_off(COLOR_PAIR_CYAN);
    tui_bold_off();

    tui_print(y++, 4, "* Buat pos anggaran baru dengan batas nominal");
    tui_print(y++, 4, "* Edit nama atau nominal pos");
    tui_print(y++, 4, "* Hapus pos (hanya jika tidak ada transaksi)");
    tui_print(y++, 4, "* Lihat realisasi dan sisa anggaran");
    y++;

    tui_bold_on();
    tui_color_on(COLOR_PAIR_CYAN);
    tui_print(y++, 2, "MENU ANALISIS:");
    tui_color_off(COLOR_PAIR_CYAN);
    tui_bold_off();

    tui_print(y++, 4, "* Lihat ringkasan keuangan bulanan");
    tui_print(y++, 4, "* Grafik perbandingan pemasukan/pengeluaran");
    tui_print(y++, 4, "* Kesimpulan dan saran otomatis");

    display_footer("Tekan sembarang tombol untuk kembali");
    tui_refresh();
    tui_getch();
}
