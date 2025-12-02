#include <stdio.h>
#include <string.h>
#include "analisis.h"
#include "transaksi.h"
#include "file.h"
#include "tui.h"
#include "pos.h"
#include "utils.h"

/* ===== KONSTANTA LOKAL ===== */
/* Aksi Menu */
#define ACT_LIHAT       1
#define ACT_REFRESH     2
#define ACT_GANTI_BULAN 3
#define ACT_KEMBALI     0

/* ===== DEKLARASI FUNGSI LOKAL ===== */
static void tampilkan_transaksi_filter(int filter_jenis);
static int display_laporan_keuangan(int bulan, AnalisisKeuangan *analisis);

/* ===== IMPLEMENTASI PERHITUNGAN ANALISIS ===== */

/**
 * Menghitung analisis keuangan lengkap untuk bulan tertentu
 */
int hitung_analisis_bulan(int bulan, AnalisisKeuangan *result) {
    if (!result || bulan < 1 || bulan > 12) return 0;

    memset(result, 0, sizeof(AnalisisKeuangan));

    /* Hitung total pemasukan dan pengeluaran */
    result->total_pemasukan = hitung_total_pemasukan_transaksi(bulan);
    result->total_pengeluaran = hitung_total_pengeluaran_transaksi(bulan);

    /* Hitung jumlah transaksi */
    result->total_trx_pemasukan = hitung_jumlah_transaksi(bulan, JENIS_PEMASUKAN);
    result->total_trx_pengeluaran = hitung_jumlah_transaksi(bulan, JENIS_PENGELUARAN);

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

/**
 * Menentukan kondisi keuangan berdasarkan saldo
 */
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

/**
 * Menentukan kesimpulan berdasarkan persentase sisa
 */
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

/**
 * Mendapatkan label kondisi keuangan
 */
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

/**
 * Mendapatkan label kesimpulan
 */
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

/**
 * Mendapatkan deskripsi kesimpulan yang lebih lengkap
 */
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

/**
 * Me-refresh analisis untuk bulan (hitung ulang dan simpan)
 */
int refresh_analisis_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    AnalisisKeuangan analisis;
    if (!hitung_analisis_bulan(bulan, &analisis)) {
        return 0;
    }

    return simpan_analisis_bulan(bulan, &analisis);
}

/* ===== IMPLEMENTASI TAMPILAN ANALISIS ===== */

/**
 * Tampilkan analisis keuangan lengkap untuk bulan
 * I.S.: bulan valid
 * F.S.: analisis ditampilkan di layar
 */
void tampilkan_analisis_bulan(int bulan) {
    AnalisisKeuangan analisis;

    /* Hitung analisis */
    if (!hitung_analisis_bulan(bulan, &analisis)) {
        return;
    }

    while (1) {
        int next_y = display_laporan_keuangan(bulan, &analisis);
        (void)next_y; /* unused */

        /* Menu Navigasi */
        tui_gambar_garis_horizontal(tui_ambil_tinggi() - 7, 2, 60, '-');
        tui_aktifkan_tebal();
        tui_cetak(tui_ambil_tinggi() - 6, 2, "MENU NAVIGASI:");
        tui_nonaktifkan_tebal();
        tui_cetak(tui_ambil_tinggi() - 5, 4, "1. Tampilkan Semua Transaksi");
        tui_cetak(tui_ambil_tinggi() - 4, 4, "2. Tampilkan Transaksi Pengeluaran");
        tui_cetak(tui_ambil_tinggi() - 3, 4, "3. Tampilkan Transaksi Pemasukan");

        tampilkan_footer("1-3: Pilih Menu | ESC: Kembali");
        tui_segarkan();

        int ch = tui_ambil_karakter();
        switch (ch) {
            case '1':
                tampilkan_transaksi_filter(-1); /* Semua transaksi */
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

/**
 * Helper: Menampilkan laporan keuangan di layar
 */
static int display_laporan_keuangan(int bulan, AnalisisKeuangan *analisis) {
    tui_hapus_layar();

    int y = 1;
    char title[64];

    /* HEADER */
    tui_aktifkan_tebal();
    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    snprintf(title, sizeof(title), "# LAPORAN KEUANGAN BULAN %s", dapatkan_nama_bulan(bulan));
    tui_cetak_tengah(y++, title);
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);
    tui_nonaktifkan_tebal();

    tui_gambar_garis_horizontal(y++, 2, 76, '=');
    y++;

    /* BAGIAN RINGKASAN */
    tui_aktifkan_tebal();
    tui_cetak(y++, 2, "RINGKASAN KEUANGAN:");
    tui_nonaktifkan_tebal();

    char buffer[64];

    /* Total Pemasukan */
    format_rupiah(analisis->total_pemasukan, buffer);
    tui_cetakf(y, 4, "Total Pemasukan (%d Transaksi)  : ", analisis->total_trx_pemasukan);
    tui_aktifkan_warna(COLOR_PAIR_GREEN);
    tui_cetak(y++, 42, buffer);
    tui_nonaktifkan_warna(COLOR_PAIR_GREEN);

    /* Total Pengeluaran */
    format_rupiah(analisis->total_pengeluaran, buffer);
    tui_cetakf(y, 4, "Total Pengeluaran (%d Transaksi): ", analisis->total_trx_pengeluaran);
    tui_aktifkan_warna(COLOR_PAIR_RED);
    tui_cetak(y++, 42, buffer);
    tui_nonaktifkan_warna(COLOR_PAIR_RED);

    /* Saldo Akhir */
    format_saldo(analisis->saldo_akhir, buffer);
    tui_cetak(y, 4, "Saldo Akhir                     : ");
    if (analisis->saldo_akhir < 0) {
        tui_aktifkan_warna(COLOR_PAIR_RED);
    } else {
        tui_aktifkan_warna(COLOR_PAIR_GREEN);
    }
    tui_aktifkan_tebal();
    tui_cetak(y++, 42, buffer);
    tui_nonaktifkan_tebal();
    tui_reset_atribut();

    /* Rata-rata Pengeluaran */
    format_rupiah((unsigned long long)analisis->rata_rata_pengeluaran, buffer);
    tui_cetakf(y++, 4, "Rata-rata Pengeluaran           : %s", buffer);

    y++;
    tui_gambar_garis_horizontal(y++, 2, 76, '-');

    /* TABEL POS ANGGARAN */
    tui_aktifkan_tebal();
    tui_cetak(y++, 2, "TABEL POS ANGGARAN:");
    tui_nonaktifkan_tebal();

    PosAnggaran pos_list[MAX_POS];
    int pos_count = muat_pos_bulan(pos_list, MAX_POS, bulan);

    if (pos_count > 0) {
        /* Header tabel */
        tui_aktifkan_tebal();
        tui_aktifkan_warna(COLOR_PAIR_CYAN);
        tui_cetakf(y++, 4, "%-15s %12s %12s %12s %5s %8s",
                   "Kategori", "Anggaran", "Realisasi", "Sisa", "Trx", "Status");
        tui_nonaktifkan_warna(COLOR_PAIR_CYAN);
        tui_nonaktifkan_tebal();
        tui_gambar_garis_horizontal(y++, 4, 70, '-');

        char anggaran_str[20], realisasi_str[20], sisa_str[20];
        for (int i = 0; i < pos_count && y < tui_ambil_tinggi() - 10; i++) {
            format_rupiah(pos_list[i].nominal, anggaran_str);
            format_rupiah(pos_list[i].realisasi, realisasi_str);
            format_saldo(pos_list[i].sisa, sisa_str);

            const char *status_str = (pos_list[i].status == STATUS_AMAN) ? "AMAN" : "TIDAK AMAN";
            int status_color = (pos_list[i].status == STATUS_AMAN) ? COLOR_PAIR_GREEN : COLOR_PAIR_RED;

            tui_cetakf(y, 4, "%-15s %12s %12s %12s %5d ",
                       pos_list[i].nama, anggaran_str, realisasi_str, sisa_str,
                       pos_list[i].jumlah_transaksi);
            tui_aktifkan_warna(status_color);
            tui_cetak(y++, 68, status_str);
            tui_nonaktifkan_warna(status_color);
        }
    } else {
        tui_cetak(y++, 4, "(Tidak ada pos anggaran)");
    }

    y++;
    tui_gambar_garis_horizontal(y++, 2, 76, '-');

    /* KONDISI KEUANGAN */
    tui_aktifkan_tebal();
    tui_cetak(y, 2, "KONDISI KEUANGAN: ");
    tui_nonaktifkan_tebal();

    const char *kondisi_str;
    int kondisi_color;
    switch (analisis->kondisi_keuangan) {
        case KONDISI_DEFISIT:
            kondisi_str = "DEFISIT";
            kondisi_color = COLOR_PAIR_RED;
            break;
        case KONDISI_SEIMBANG:
            kondisi_str = "SEIMBANG";
            kondisi_color = COLOR_PAIR_YELLOW;
            break;
        case KONDISI_SURPLUS:
            kondisi_str = "SURPLUS";
            kondisi_color = COLOR_PAIR_GREEN;
            break;
        default:
            kondisi_str = "TIDAK DIKETAHUI";
            kondisi_color = COLOR_PAIR_CYAN;
    }

    tui_aktifkan_warna(kondisi_color);
    tui_aktifkan_tebal();
    tui_cetakf(y, 20, "[%s]", kondisi_str);
    tui_nonaktifkan_tebal();
    tui_nonaktifkan_warna(kondisi_color);
    tui_cetakf(y++, 32, " (%.2f%% sisa dari total pemasukan)", analisis->persentase_sisa);

    y++;

    /* KESIMPULAN */
    tui_aktifkan_tebal();
    tui_cetak(y, 2, "KESIMPULAN: ");
    tui_nonaktifkan_tebal();

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

    tui_aktifkan_warna(kesimpulan_color);
    tui_cetak(y++, 14, kesimpulan_str);
    tui_nonaktifkan_warna(kesimpulan_color);

    return y;
}

/**
 * Helper: Tampilkan transaksi dari semua bulan dengan filter opsional
 */
static void tampilkan_transaksi_filter(int filter_jenis) {
    tui_hapus_layar();

    const char *title;
    if (filter_jenis == JENIS_PENGELUARAN) {
        title = "SEMUA TRANSAKSI PENGELUARAN (SEMUA BULAN)";
    } else if (filter_jenis == JENIS_PEMASUKAN) {
        title = "SEMUA TRANSAKSI PEMASUKAN (SEMUA BULAN)";
    } else {
        title = "SEMUA TRANSAKSI (SEMUA BULAN)";
    }

    tampilkan_header(title);

    /* Muat semua transaksi */
    Transaksi all_trx[MAX_TRANSAKSI];
    int total = muat_transaksi(all_trx, MAX_TRANSAKSI);

    /* Filter jika diperlukan */
    Transaksi filtered[MAX_TRANSAKSI];
    int count = 0;

    for (int i = 0; i < total; i++) {
        if (filter_jenis < 0 || all_trx[i].jenis == filter_jenis) {
            filtered[count++] = all_trx[i];
        }
    }

    /* Tampilkan tabel */
    int y = 4;
    if (count == 0) {
        tui_cetak(y, 4, "(Tidak ada transaksi)");
    } else {
        tampilkan_tabel_transaksi(filtered, count, y, -1);
    }

    tampilkan_footer("Tekan sembarang tombol untuk kembali");
    tui_segarkan();
    tui_ambil_karakter();
}

/**
 * Tampilkan ringkasan kondisi keuangan
 */
int tampilkan_ringkasan_kondisi(AnalisisKeuangan *analisis, int start_y) {
    if (!analisis) return start_y;

    int y = start_y;
    char buffer[64];

    /* Total Pemasukan */
    format_rupiah(analisis->total_pemasukan, buffer);
    tui_cetak(y, 2, "Total Pemasukan   : ");
    tui_aktifkan_warna(COLOR_PAIR_GREEN);
    tui_cetak(y++, 22, buffer);
    tui_nonaktifkan_warna(COLOR_PAIR_GREEN);
    tui_cetakf(y++, 4, "(%d transaksi)", analisis->total_trx_pemasukan);

    /* Total Pengeluaran */
    format_rupiah(analisis->total_pengeluaran, buffer);
    tui_cetak(y, 2, "Total Pengeluaran : ");
    tui_aktifkan_warna(COLOR_PAIR_RED);
    tui_cetak(y++, 22, buffer);
    tui_nonaktifkan_warna(COLOR_PAIR_RED);
    tui_cetakf(y++, 4, "(%d transaksi)", analisis->total_trx_pengeluaran);

    /* Rata-rata Pengeluaran */
    if (analisis->total_trx_pengeluaran > 0) {
        format_rupiah((unsigned long long)analisis->rata_rata_pengeluaran, buffer);
        tui_cetakf(y++, 2, "Rata-rata/Transaksi: %s", buffer);
    }

    y++;
    tui_gambar_garis_horizontal(y++, 2, 60, '-');

    /* Saldo Akhir */
    format_saldo(analisis->saldo_akhir, buffer);
    tui_aktifkan_tebal();
    tui_cetak(y, 2, "SALDO AKHIR       : ");
    if (analisis->saldo_akhir < 0) {
        tui_aktifkan_warna(COLOR_PAIR_RED);
    } else {
        tui_aktifkan_warna(COLOR_PAIR_GREEN);
    }
    tui_cetak(y++, 22, buffer);
    tui_reset_atribut();

    /* Persentase Sisa */
    tui_cetakf(y++, 2, "Persentase Sisa   : %.2f%%", analisis->persentase_sisa);

    y++;

    /* Kondisi Keuangan */
    tui_aktifkan_tebal();
    tui_cetak(y, 2, "Kondisi Keuangan  : ");
    int kondisi_color = (analisis->kondisi_keuangan == KONDISI_SURPLUS) ?
                         COLOR_PAIR_GREEN :
                         (analisis->kondisi_keuangan == KONDISI_DEFISIT) ?
                         COLOR_PAIR_RED : COLOR_PAIR_YELLOW;
    tui_aktifkan_warna(kondisi_color);
    tui_cetak(y++, 22, get_label_kondisi(analisis->kondisi_keuangan));
    tui_reset_atribut();

    /* Kesimpulan */
    tui_aktifkan_tebal();
    tui_cetak(y, 2, "Kesimpulan        : ");
    int kesimpulan_color = (analisis->kesimpulan == KESIMPULAN_HEMAT) ?
                            COLOR_PAIR_GREEN :
                            (analisis->kesimpulan == KESIMPULAN_TIDAK_SEHAT) ?
                            COLOR_PAIR_RED : COLOR_PAIR_YELLOW;
    tui_aktifkan_warna(kesimpulan_color);
    tui_cetak(y++, 22, get_label_kesimpulan(analisis->kesimpulan));
    tui_reset_atribut();

    return y + 1;
}

/**
 * Tampilkan grafik sederhana perbandingan pemasukan/pengeluaran
 */
int tampilkan_grafik_sederhana(AnalisisKeuangan *analisis, int start_y) {
    if (!analisis) return start_y;

    int y = start_y;

    tui_gambar_garis_horizontal(y++, 2, 60, '-');
    tui_aktifkan_tebal();
    tui_cetak(y++, 2, "GRAFIK PERBANDINGAN:");
    tui_nonaktifkan_tebal();
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
    tui_cetak(y, 2, "Pemasukan   [");
    tui_aktifkan_warna(COLOR_PAIR_GREEN);
    for (int i = 0; i < bar_masuk; i++) {
        tui_cetak(y, 15 + i, "#");
    }
    tui_nonaktifkan_warna(COLOR_PAIR_GREEN);
    tui_cetak(y++, 15 + max_bar, "]");

    /* Bar Pengeluaran */
    int bar_keluar = 0;
    if (max_val > 0) {
        bar_keluar = (int)((double)analisis->total_pengeluaran / max_val * max_bar);
    }
    tui_cetak(y, 2, "Pengeluaran [");
    tui_aktifkan_warna(COLOR_PAIR_RED);
    for (int i = 0; i < bar_keluar; i++) {
        tui_cetak(y, 15 + i, "#");
    }
    tui_nonaktifkan_warna(COLOR_PAIR_RED);
    tui_cetak(y++, 15 + max_bar, "]");

    return y + 1;
}

/**
 * Tampilkan saran berdasarkan kesimpulan
 */
int tampilkan_saran(int kesimpulan, int start_y) {
    int y = start_y;

    tui_gambar_garis_horizontal(y++, 2, 60, '-');
    tui_aktifkan_tebal();
    tui_cetak(y++, 2, "SARAN:");
    tui_nonaktifkan_tebal();

    const char *deskripsi = get_deskripsi_kesimpulan(kesimpulan);
    tui_cetak(y++, 2, deskripsi);

    y++;

    /* Saran spesifik */
    switch (kesimpulan) {
        case KESIMPULAN_HEMAT:
            tui_aktifkan_warna(COLOR_PAIR_GREEN);
            tui_cetak(y++, 2, "* Pertahankan pola keuangan Anda!");
            tui_cetak(y++, 2, "* Pertimbangkan untuk menabung atau investasi.");
            tui_nonaktifkan_warna(COLOR_PAIR_GREEN);
            break;

        case KESIMPULAN_SEIMBANG:
            tui_aktifkan_warna(COLOR_PAIR_YELLOW);
            tui_cetak(y++, 2, "* Coba tingkatkan tabungan bulanan.");
            tui_cetak(y++, 2, "* Tinjau kembali pengeluaran yang bisa dikurangi.");
            tui_nonaktifkan_warna(COLOR_PAIR_YELLOW);
            break;

        case KESIMPULAN_BOROS:
            tui_aktifkan_warna(COLOR_PAIR_YELLOW);
            tui_cetak(y++, 2, "* Kurangi pengeluaran tidak penting.");
            tui_cetak(y++, 2, "* Buat prioritas kebutuhan vs keinginan.");
            tui_cetak(y++, 2, "* Evaluasi pos anggaran yang melebihi batas.");
            tui_nonaktifkan_warna(COLOR_PAIR_YELLOW);
            break;

        case KESIMPULAN_TIDAK_SEHAT:
            tui_aktifkan_warna(COLOR_PAIR_RED);
            tui_cetak(y++, 2, "* PERINGATAN: Keuangan perlu perhatian serius!");
            tui_cetak(y++, 2, "* Segera kurangi pengeluaran tidak penting.");
            tui_cetak(y++, 2, "* Cari sumber pemasukan tambahan jika memungkinkan.");
            tui_cetak(y++, 2, "* Hindari utang untuk pengeluaran konsumtif.");
            tui_nonaktifkan_warna(COLOR_PAIR_RED);
            break;
    }

    return y + 1;
}

/* ===== IMPLEMENTASI HANDLER ANALISIS ===== */

/**
 * Menampilkan menu analisis keuangan
 */
int menu_analisis_utama(int bulan) {
    char title[64];
    snprintf(title, sizeof(title), "Menu Analisis - %s", dapatkan_nama_bulan(bulan));

    Menu menu;
    menu_inisialisasi(&menu, title);

    menu_tambah_item(&menu, "Lihat Analisis Keuangan", ACT_LIHAT);
    menu_tambah_item(&menu, "Refresh Analisis", ACT_REFRESH);

    return menu_navigasi(&menu);
}

/**
 * Jalankan modul analisis keuangan
 */
void jalankan_modul_analisis(int bulan_awal) {
    int bulan = bulan_awal;

    while (1) {
        int action = menu_analisis_utama(bulan);

        switch (action) {
            case ACT_LIHAT:
                tampilkan_analisis_bulan(bulan);
                break;

            case ACT_REFRESH:
                if (refresh_analisis_bulan(bulan)) {
                    tampilkan_sukses("Analisis berhasil diperbarui");
                } else {
                    tampilkan_error("Gagal memperbarui analisis");
                }
                break;

            case ACT_KEMBALI:
            case CANCEL:
                return;
        }
    }
}
