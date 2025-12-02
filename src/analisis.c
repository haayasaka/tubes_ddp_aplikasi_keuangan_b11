/**
 * @file analisis.c
 * @brief Implementasi modul analisis keuangan
 * @author Kelompok B11
 * @date 2025
 */

#include <stdio.h>
#include <string.h>
#include "analisis.h"
#include "transaksi.h"
#include "file.h"
#include "tui.h"
#include "pos.h"
#include "utils.h"
#include "constants.h"

/* --- analisis_calc.c --- */

int hitung_analisis_bulan(int bulan, AnalisisKeuangan *result) {
    if (!result || bulan < 1 || bulan > 12) return 0;
    
    memset(result, 0, sizeof(AnalisisKeuangan));
    
    /* Hitung total pemasukan dan pengeluaran */
    result->total_pemasukan = transaksi_total_pemasukan(bulan);
    result->total_pengeluaran = transaksi_total_pengeluaran(bulan);
    
    /* Hitung jumlah transaksi */
    result->total_trx_pemasukan = transaksi_count(bulan, JENIS_PEMASUKAN);
    result->total_trx_pengeluaran = transaksi_count(bulan, JENIS_PENGELUARAN);
    
    /* Hitung saldo akhir */
    result->saldo_akhir = (long long)result->total_pemasukan - 
                          (long long)result->total_pengeluaran;
    
    /* Hitung rata-rata pengeluaran */
    if (result->total_trx_pengeluaran > 0) {
        result->rata_rata_pengeluaran = (double)result->total_pengeluaran / 
                                         result->total_trx_pengeluaran;
    } else {
        result->rata_rata_pengeluaran = 0;
    }
    
    /* Hitung persentase sisa */
    if (result->total_pemasukan > 0) {
        result->persentase_sisa = ((double)result->saldo_akhir / 
                                   (double)result->total_pemasukan) * 100.0;
    } else {
        result->persentase_sisa = 0;
    }
    
    /* Tentukan kondisi keuangan */
    result->kondisi_keuangan = tentukan_kondisi_keuangan(
        result->total_pemasukan, result->total_pengeluaran);
    
    /* Tentukan kesimpulan */
    result->kesimpulan = tentukan_kesimpulan(result->persentase_sisa);
    
    return 1;
}

int tentukan_kondisi_keuangan(unsigned long long pemasukan, 
                               unsigned long long pengeluaran) {
    if (pengeluaran > pemasukan) {
        return KONDISI_DEFISIT;
    } else if (pengeluaran == pemasukan) {
        return KONDISI_SEIMBANG;
    } else {
        return KONDISI_SURPLUS;
    }
}

int tentukan_kesimpulan(double persentase_sisa) {
    if (persentase_sisa > 25.0) {
        return KESIMPULAN_HEMAT;
    } else if (persentase_sisa > 0.0) {
        return KESIMPULAN_SEIMBANG;
    } else if (persentase_sisa >= -10.0) {
        return KESIMPULAN_BOROS;
    } else {
        return KESIMPULAN_TIDAK_SEHAT;
    }
}

const char* get_label_kondisi(int kondisi) {
    switch (kondisi) {
        case KONDISI_DEFISIT:
            return "DEFISIT";
        case KONDISI_SEIMBANG:
            return "SEIMBANG";
        case KONDISI_SURPLUS:
            return "SURPLUS";
        default:
            return "TIDAK DIKETAHUI";
    }
}

const char* get_label_kesimpulan(int kesimpulan) {
    switch (kesimpulan) {
        case KESIMPULAN_HEMAT:
            return "HEMAT";
        case KESIMPULAN_SEIMBANG:
            return "SEIMBANG";
        case KESIMPULAN_BOROS:
            return "BOROS";
        case KESIMPULAN_TIDAK_SEHAT:
            return "TIDAK SEHAT";
        default:
            return "TIDAK DIKETAHUI";
    }
}

const char* get_deskripsi_kesimpulan(int kesimpulan) {
    switch (kesimpulan) {
        case KESIMPULAN_HEMAT:
            return "Keuangan Anda sangat baik! Sisa > 25% dari pemasukan.";
        case KESIMPULAN_SEIMBANG:
            return "Keuangan Anda cukup baik. Sisa 0-25% dari pemasukan.";
        case KESIMPULAN_BOROS:
            return "Perlu lebih hemat! Pengeluaran melebihi pemasukan hingga 10%.";
        case KESIMPULAN_TIDAK_SEHAT:
            return "Keuangan tidak sehat! Defisit lebih dari 10% pemasukan.";
        default:
            return "Status keuangan tidak dapat ditentukan.";
    }
}

int refresh_analisis_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    AnalisisKeuangan analisis;
    if (!hitung_analisis_bulan(bulan, &analisis)) {
        return 0;
    }
    
    return save_analisis_bulan(bulan, &analisis);
}

/* --- analisis_display.c --- */

/* Forward declarations for new functions */
static void tampilkan_transaksi_filter(int filter_jenis);
static int display_laporan_keuangan(int bulan, AnalisisKeuangan *analisis);

void tampilkan_analisis_bulan(int bulan) {
    AnalisisKeuangan analisis;

    /* Hitung analisis */
    if (!hitung_analisis_bulan(bulan, &analisis)) {
        return;
    }

    while (1) {
        int next_y = display_laporan_keuangan(bulan, &analisis);
        (void)next_y; /* unused */

        /* Navigation menu */
        tui_draw_hline(tui_get_height() - 7, 2, 60, '-');
        tui_bold_on();
        tui_print(tui_get_height() - 6, 2, "MENU NAVIGASI:");
        tui_bold_off();
        tui_print(tui_get_height() - 5, 4, "1. Tampilkan Semua Transaksi");
        tui_print(tui_get_height() - 4, 4, "2. Tampilkan Transaksi Pengeluaran");
        tui_print(tui_get_height() - 3, 4, "3. Tampilkan Transaksi Pemasukan");

        display_footer("1-3: Pilih Menu | ESC: Kembali");
        tui_refresh();

        int ch = tui_getch();
        switch (ch) {
            case '1':
                tampilkan_transaksi_filter(-1); /* All transactions */
                break;
            case '2':
                tampilkan_transaksi_filter(JENIS_PENGELUARAN);
                break;
            case '3':
                tampilkan_transaksi_filter(JENIS_PEMASUKAN);
                break;
            case 27: /* ESC */
                return;
        }
    }
}

static int display_laporan_keuangan(int bulan, AnalisisKeuangan *analisis) {
    tui_clear();

    int y = 1;
    char title[64];

    /* HEADER */
    tui_bold_on();
    tui_color_on(COLOR_PAIR_CYAN);
    snprintf(title, sizeof(title), "# LAPORAN KEUANGAN BULAN %s", get_nama_bulan(bulan));
    tui_print_center(y++, title);
    tui_color_off(COLOR_PAIR_CYAN);
    tui_bold_off();

    tui_draw_hline(y++, 2, 76, '=');
    y++;

    /* SUMMARY SECTION */
    tui_bold_on();
    tui_print(y++, 2, "RINGKASAN KEUANGAN:");
    tui_bold_off();

    char buffer[64];

    /* Total Pemasukan */
    format_rupiah(analisis->total_pemasukan, buffer);
    tui_printf(y, 4, "Total Pemasukan (%d Transaksi)  : ", analisis->total_trx_pemasukan);
    tui_color_on(COLOR_PAIR_GREEN);
    tui_print(y++, 42, buffer);
    tui_color_off(COLOR_PAIR_GREEN);

    /* Total Pengeluaran */
    format_rupiah(analisis->total_pengeluaran, buffer);
    tui_printf(y, 4, "Total Pengeluaran (%d Transaksi): ", analisis->total_trx_pengeluaran);
    tui_color_on(COLOR_PAIR_RED);
    tui_print(y++, 42, buffer);
    tui_color_off(COLOR_PAIR_RED);

    /* Saldo Akhir */
    format_saldo(analisis->saldo_akhir, buffer);
    tui_print(y, 4, "Saldo Akhir                     : ");
    if (analisis->saldo_akhir < 0) {
        tui_color_on(COLOR_PAIR_RED);
    } else {
        tui_color_on(COLOR_PAIR_GREEN);
    }
    tui_bold_on();
    tui_print(y++, 42, buffer);
    tui_bold_off();
    tui_attr_reset();

    /* Rata-rata Pengeluaran */
    format_rupiah((unsigned long long)analisis->rata_rata_pengeluaran, buffer);
    tui_printf(y++, 4, "Rata-rata Pengeluaran           : %s", buffer);

    y++;
    tui_draw_hline(y++, 2, 76, '-');

    /* BUDGET CATEGORY TABLE */
    tui_bold_on();
    tui_print(y++, 2, "TABEL POS ANGGARAN:");
    tui_bold_off();

    PosAnggaran pos_list[MAX_POS];
    int pos_count = load_pos_bulan(pos_list, MAX_POS, bulan);

    if (pos_count > 0) {
        /* Table header */
        tui_bold_on();
        tui_color_on(COLOR_PAIR_CYAN);
        tui_printf(y++, 4, "%-15s %12s %12s %12s %5s %8s",
                   "Kategori", "Anggaran", "Realisasi", "Sisa", "Trx", "Status");
        tui_color_off(COLOR_PAIR_CYAN);
        tui_bold_off();
        tui_draw_hline(y++, 4, 70, '-');

        char anggaran_str[20], realisasi_str[20], sisa_str[20];
        for (int i = 0; i < pos_count && y < tui_get_height() - 10; i++) {
            format_rupiah(pos_list[i].nominal, anggaran_str);
            format_rupiah(pos_list[i].realisasi, realisasi_str);
            format_saldo(pos_list[i].sisa, sisa_str);

            const char *status_str = (pos_list[i].status == STATUS_AMAN) ? "AMAN" : "TIDAK AMAN";
            int status_color = (pos_list[i].status == STATUS_AMAN) ? COLOR_PAIR_GREEN : COLOR_PAIR_RED;

            tui_printf(y, 4, "%-15s %12s %12s %12s %5d ",
                       pos_list[i].nama, anggaran_str, realisasi_str, sisa_str,
                       pos_list[i].jumlah_transaksi);
            tui_color_on(status_color);
            tui_print(y++, 68, status_str);
            tui_color_off(status_color);
        }
    } else {
        tui_print(y++, 4, "(Tidak ada pos anggaran)");
    }

    y++;
    tui_draw_hline(y++, 2, 76, '-');

    /* FINANCIAL CONDITION */
    tui_bold_on();
    tui_print(y, 2, "KONDISI KEUANGAN: ");
    tui_bold_off();

    const char *kondisi_str;
    int kondisi_color;
    switch (analisis->kondisi_keuangan) {
        case KONDISI_DEFISIT:
            kondisi_str = "DEFICIT";
            kondisi_color = COLOR_PAIR_RED;
            break;
        case KONDISI_SEIMBANG:
            kondisi_str = "BALANCED";
            kondisi_color = COLOR_PAIR_YELLOW;
            break;
        case KONDISI_SURPLUS:
            kondisi_str = "SURPLUS";
            kondisi_color = COLOR_PAIR_GREEN;
            break;
        default:
            kondisi_str = "UNKNOWN";
            kondisi_color = COLOR_PAIR_CYAN;
    }

    tui_color_on(kondisi_color);
    tui_bold_on();
    tui_printf(y, 20, "[%s]", kondisi_str);
    tui_bold_off();
    tui_color_off(kondisi_color);
    tui_printf(y++, 32, " (%.2f%% sisa dari total pemasukan)", analisis->persentase_sisa);

    y++;

    /* CONCLUSION */
    tui_bold_on();
    tui_print(y, 2, "KESIMPULAN: ");
    tui_bold_off();

    const char *kesimpulan_str;
    int kesimpulan_color;
    switch (analisis->kesimpulan) {
        case KESIMPULAN_HEMAT:
            kesimpulan_str = "Anda adalah mahasiswa yang hemat dan produktif.";
            kesimpulan_color = COLOR_PAIR_GREEN;
            break;
        case KESIMPULAN_SEIMBANG:
            kesimpulan_str = "Keuangan Anda seimbang, tetap waspada terhadap pengeluaran tidak perlu.";
            kesimpulan_color = COLOR_PAIR_YELLOW;
            break;
        case KESIMPULAN_BOROS:
            kesimpulan_str = "Anda mulai boros, perhatikan pengeluaran harian Anda.";
            kesimpulan_color = COLOR_PAIR_YELLOW;
            break;
        case KESIMPULAN_TIDAK_SEHAT:
            kesimpulan_str = "Kondisi keuangan tidak sehat. Kurangi pengeluaran dan cari pemasukan tambahan.";
            kesimpulan_color = COLOR_PAIR_RED;
            break;
        default:
            kesimpulan_str = "Status tidak diketahui.";
            kesimpulan_color = COLOR_PAIR_CYAN;
    }

    tui_color_on(kesimpulan_color);
    tui_print(y++, 14, kesimpulan_str);
    tui_color_off(kesimpulan_color);

    return y;
}

/* Display transactions from all months with optional filter */
static void tampilkan_transaksi_filter(int filter_jenis) {
    tui_clear();

    const char *title;
    if (filter_jenis == JENIS_PENGELUARAN) {
        title = "SEMUA TRANSAKSI PENGELUARAN (SEMUA BULAN)";
    } else if (filter_jenis == JENIS_PEMASUKAN) {
        title = "SEMUA TRANSAKSI PEMASUKAN (SEMUA BULAN)";
    } else {
        title = "SEMUA TRANSAKSI (SEMUA BULAN)";
    }

    display_header(title);

    /* Load all transactions */
    Transaksi all_trx[MAX_TRANSAKSI];
    int total = load_transaksi(all_trx, MAX_TRANSAKSI);

    /* Filter if needed */
    Transaksi filtered[MAX_TRANSAKSI];
    int count = 0;

    for (int i = 0; i < total; i++) {
        if (filter_jenis < 0 || all_trx[i].jenis == filter_jenis) {
            filtered[count++] = all_trx[i];
        }
    }

    /* Display table */
    int y = 4;
    if (count == 0) {
        tui_print(y, 4, "(Tidak ada transaksi)");
    } else {
        display_transaksi_table(filtered, count, y, -1);
    }

    display_footer("Tekan sembarang tombol untuk kembali");
    tui_refresh();
    tui_getch();
}

int tampilkan_ringkasan_kondisi(AnalisisKeuangan *analisis, int start_y) {
    if (!analisis) return start_y;

    int y = start_y;
    char buffer[64];

    /* Total Pemasukan */
    format_rupiah(analisis->total_pemasukan, buffer);
    tui_print(y, 2, "Total Pemasukan   : ");
    tui_color_on(COLOR_PAIR_GREEN);
    tui_print(y++, 22, buffer);
    tui_color_off(COLOR_PAIR_GREEN);
    tui_printf(y++, 4, "(%d transaksi)", analisis->total_trx_pemasukan);

    /* Total Pengeluaran */
    format_rupiah(analisis->total_pengeluaran, buffer);
    tui_print(y, 2, "Total Pengeluaran : ");
    tui_color_on(COLOR_PAIR_RED);
    tui_print(y++, 22, buffer);
    tui_color_off(COLOR_PAIR_RED);
    tui_printf(y++, 4, "(%d transaksi)", analisis->total_trx_pengeluaran);

    /* Rata-rata Pengeluaran */
    if (analisis->total_trx_pengeluaran > 0) {
        format_rupiah((unsigned long long)analisis->rata_rata_pengeluaran, buffer);
        tui_printf(y++, 2, "Rata-rata/Transaksi: %s", buffer);
    }

    y++;
    tui_draw_hline(y++, 2, 60, '-');

    /* Saldo Akhir */
    format_saldo(analisis->saldo_akhir, buffer);
    tui_bold_on();
    tui_print(y, 2, "SALDO AKHIR       : ");
    if (analisis->saldo_akhir < 0) {
        tui_color_on(COLOR_PAIR_RED);
    } else {
        tui_color_on(COLOR_PAIR_GREEN);
    }
    tui_print(y++, 22, buffer);
    tui_attr_reset();

    /* Persentase Sisa */
    tui_printf(y++, 2, "Persentase Sisa   : %.2f%%", analisis->persentase_sisa);

    y++;

    /* Kondisi Keuangan */
    tui_bold_on();
    tui_print(y, 2, "Kondisi Keuangan  : ");
    int kondisi_color = (analisis->kondisi_keuangan == KONDISI_SURPLUS) ?
                         COLOR_PAIR_GREEN :
                         (analisis->kondisi_keuangan == KONDISI_DEFISIT) ?
                         COLOR_PAIR_RED : COLOR_PAIR_YELLOW;
    tui_color_on(kondisi_color);
    tui_print(y++, 22, get_label_kondisi(analisis->kondisi_keuangan));
    tui_attr_reset();

    /* Kesimpulan */
    tui_bold_on();
    tui_print(y, 2, "Kesimpulan        : ");
    int kesimpulan_color = (analisis->kesimpulan == KESIMPULAN_HEMAT) ?
                            COLOR_PAIR_GREEN :
                            (analisis->kesimpulan == KESIMPULAN_TIDAK_SEHAT) ?
                            COLOR_PAIR_RED : COLOR_PAIR_YELLOW;
    tui_color_on(kesimpulan_color);
    tui_print(y++, 22, get_label_kesimpulan(analisis->kesimpulan));
    tui_attr_reset();

    return y + 1;
}

int tampilkan_grafik_sederhana(AnalisisKeuangan *analisis, int start_y) {
    if (!analisis) return start_y;

    int y = start_y;

    tui_draw_hline(y++, 2, 60, '-');
    tui_bold_on();
    tui_print(y++, 2, "GRAFIK PERBANDINGAN:");
    tui_bold_off();
    y++;

    /* Hitung skala */
    unsigned long long max_val = analisis->total_pemasukan;
    if (analisis->total_pengeluaran > max_val) {
        max_val = analisis->total_pengeluaran;
    }

    int max_bar = 40;  /* Panjang maksimal bar */

    /* Bar Pemasukan */
    int bar_masuk = 0;
    if (max_val > 0) {
        bar_masuk = (int)((double)analisis->total_pemasukan / max_val * max_bar);
    }
    tui_print(y, 2, "Pemasukan   [");
    tui_color_on(COLOR_PAIR_GREEN);
    for (int i = 0; i < bar_masuk; i++) {
        tui_print(y, 15 + i, "#");
    }
    tui_color_off(COLOR_PAIR_GREEN);
    tui_print(y++, 15 + max_bar, "]");

    /* Bar Pengeluaran */
    int bar_keluar = 0;
    if (max_val > 0) {
        bar_keluar = (int)((double)analisis->total_pengeluaran / max_val * max_bar);
    }
    tui_print(y, 2, "Pengeluaran [");
    tui_color_on(COLOR_PAIR_RED);
    for (int i = 0; i < bar_keluar; i++) {
        tui_print(y, 15 + i, "#");
    }
    tui_color_off(COLOR_PAIR_RED);
    tui_print(y++, 15 + max_bar, "]");

    return y + 1;
}

int tampilkan_saran(int kesimpulan, int start_y) {
    int y = start_y;

    tui_draw_hline(y++, 2, 60, '-');
    tui_bold_on();
    tui_print(y++, 2, "SARAN:");
    tui_bold_off();

    const char *deskripsi = get_deskripsi_kesimpulan(kesimpulan);
    tui_print(y++, 2, deskripsi);

    y++;

    /* Saran spesifik */
    switch (kesimpulan) {
        case KESIMPULAN_HEMAT:
            tui_color_on(COLOR_PAIR_GREEN);
            tui_print(y++, 2, "* Pertahankan pola keuangan Anda!");
            tui_print(y++, 2, "* Pertimbangkan untuk menabung atau investasi.");
            tui_color_off(COLOR_PAIR_GREEN);
            break;

        case KESIMPULAN_SEIMBANG:
            tui_color_on(COLOR_PAIR_YELLOW);
            tui_print(y++, 2, "* Coba tingkatkan tabungan bulanan.");
            tui_print(y++, 2, "* Review pengeluaran yang bisa dikurangi.");
            tui_color_off(COLOR_PAIR_YELLOW);
            break;

        case KESIMPULAN_BOROS:
            tui_color_on(COLOR_PAIR_YELLOW);
            tui_print(y++, 2, "* Kurangi pengeluaran tidak penting.");
            tui_print(y++, 2, "* Buat prioritas kebutuhan vs keinginan.");
            tui_print(y++, 2, "* Evaluasi pos anggaran yang over budget.");
            tui_color_off(COLOR_PAIR_YELLOW);
            break;

        case KESIMPULAN_TIDAK_SEHAT:
            tui_color_on(COLOR_PAIR_RED);
            tui_print(y++, 2, "* PERINGATAN: Keuangan perlu perhatian serius!");
            tui_print(y++, 2, "* Segera kurangi pengeluaran tidak esensial.");
            tui_print(y++, 2, "* Cari sumber pemasukan tambahan jika memungkinkan.");
            tui_print(y++, 2, "* Hindari utang untuk pengeluaran konsumtif.");
            tui_color_off(COLOR_PAIR_RED);
            break;
    }

    return y + 1;
}

/* --- analisis_handler.c --- */

#define ACT_LIHAT       1
#define ACT_REFRESH     2
#define ACT_GANTI_BULAN 3
#define ACT_KEMBALI     0

int menu_analisis_utama(int bulan) {
    char title[64];
    snprintf(title, sizeof(title), "Menu Analisis - %s", get_nama_bulan(bulan));

    Menu menu;
    menu_init(&menu, title);

    menu_add_item(&menu, "Lihat Analisis Keuangan", ACT_LIHAT);
    menu_add_item(&menu, "Refresh Analisis", ACT_REFRESH);
    /* REV-2 & REV-6: Removed "Ganti Bulan" and "Kembali" - use ESC to go back */

    return menu_navigate(&menu);
}

void run_analisis_module(int bulan_awal) {
    int bulan = bulan_awal;

    while (1) {
        int action = menu_analisis_utama(bulan);

        switch (action) {
            case ACT_LIHAT:
                tampilkan_analisis_bulan(bulan);
                break;

            case ACT_REFRESH:
                if (refresh_analisis_bulan(bulan)) {
                    show_success("Analisis berhasil diperbarui");
                } else {
                    show_error("Gagal memperbarui analisis");
                }
                break;

            /* REV-6: Removed ACT_GANTI_BULAN case */

            case ACT_KEMBALI:
            case CANCEL:
                return;
        }
    }
}
