#include <stdio.h>
#include <string.h>
#include "transaksi.h"
#include "file.h"
#include "pos.h"
#include "tui.h"
#include "utils.h"
#include "validator.h"

/* ===== KONSTANTA LOKAL ===== */
/* Aksi Menu */
#define ACT_VIEW    1
#define ACT_TAMBAH  2
#define ACT_EDIT    3
#define ACT_HAPUS   4
#define ACT_KEMBALI 0

/* ===== FUNGSI HELPER INTERNAL ===== */

/**
 * Helper untuk mengatur pesan error
 * I.S.: error_msg buffer, msg pesan
 * F.S.: error_msg diisi msg
 */
static void atur_pesan_error(char *error_msg, const char *msg) {
    if (error_msg && msg) {
        salin_string_aman(error_msg, msg, 100);
    }
}

/* ===== IMPLEMENTASI CRUD TRANSAKSI ===== */

/**
 * Menambahkan transaksi baru dengan validasi
 */
int tambah_transaksi(const char *tanggal, int jenis, const char *pos,
                     unsigned long long nominal, const char *deskripsi,
                     char *error_msg) {
    /* Validasi tanggal */
    if (!tanggal || !validasi_tanggal_lengkap(tanggal)) {
        atur_pesan_error(error_msg, "Tanggal tidak valid");
        return 0;
    }

    /* Validasi jenis */
    if (!validasi_jenis_transaksi(jenis)) {
        atur_pesan_error(error_msg, "Jenis transaksi tidak valid");
        return 0;
    }

    /* Validasi pos */
    if (!pos || !validasi_tidak_kosong(pos)) {
        atur_pesan_error(error_msg, "Pos anggaran harus dipilih");
        return 0;
    }

    /* Validasi nominal */
    if (!validasi_nominal_positif(nominal)) {
        atur_pesan_error(error_msg, "Nominal harus lebih dari 0");
        return 0;
    }

    /* Validasi deskripsi (opsional tapi tidak boleh ada pipe) */
    if (deskripsi && strlen(deskripsi) > 0) {
        if (!validasi_panjang_deskripsi(deskripsi)) {
            atur_pesan_error(error_msg, "Deskripsi terlalu panjang (max 50 karakter)");
            return 0;
        }
        if (!validasi_tidak_ada_pipe(deskripsi)) {
            atur_pesan_error(error_msg, "Deskripsi tidak boleh mengandung karakter |");
            return 0;
        }
    }

    /* Buat transaksi baru */
    Transaksi trx;
    memset(&trx, 0, sizeof(Transaksi));

    buat_id_transaksi(trx.id);
    salin_string_aman(trx.tanggal, tanggal, sizeof(trx.tanggal));
    trx.jenis = jenis;
    salin_string_aman(trx.pos, pos, sizeof(trx.pos));
    trx.nominal = nominal;
    if (deskripsi) {
        salin_string_aman(trx.deskripsi, deskripsi, sizeof(trx.deskripsi));
    }

    if (!tambah_transaksi_ke_file(&trx)) {
        atur_pesan_error(error_msg, "Gagal menyimpan transaksi");
        return 0;
    }

    /* Update kalkulasi pos */
    int bulan = ekstrak_bulan(tanggal);
    if (bulan > 0) {
        update_semua_pos_bulan(bulan);
    }

    return 1;
}

/**
 * Mengedit transaksi dengan validasi
 */
int ubah_transaksi(const char *id, const char *tanggal, int jenis,
                   const char *pos, unsigned long long nominal,
                   const char *deskripsi, char *error_msg) {
    if (!id) {
        atur_pesan_error(error_msg, "ID transaksi tidak valid");
        return 0;
    }

    /* Load transaksi yang akan diedit */
    Transaksi trx;
    if (!cari_transaksi_berdasarkan_id(id, &trx)) {
        atur_pesan_error(error_msg, "Transaksi tidak ditemukan");
        return 0;
    }

    int old_bulan = ekstrak_bulan(trx.tanggal);

    /* Update tanggal jika diubah */
    if (tanggal && strlen(tanggal) > 0) {
        if (!validasi_tanggal_lengkap(tanggal)) {
            atur_pesan_error(error_msg, "Tanggal tidak valid");
            return 0;
        }
        salin_string_aman(trx.tanggal, tanggal, sizeof(trx.tanggal));
    }

    /* Update jenis jika diubah */
    if (jenis >= 0) {
        if (!validasi_jenis_transaksi(jenis)) {
            atur_pesan_error(error_msg, "Jenis transaksi tidak valid");
            return 0;
        }
        trx.jenis = jenis;
    }

    /* Update pos jika diubah */
    if (pos && strlen(pos) > 0) {
        salin_string_aman(trx.pos, pos, sizeof(trx.pos));
    }

    /* Update nominal jika diubah */
    if (nominal > 0) {
        trx.nominal = nominal;
    }

    /* Update deskripsi jika diubah */
    if (deskripsi) {
        if (strlen(deskripsi) > 0 && !validasi_panjang_deskripsi(deskripsi)) {
            atur_pesan_error(error_msg, "Deskripsi terlalu panjang");
            return 0;
        }
        salin_string_aman(trx.deskripsi, deskripsi, sizeof(trx.deskripsi));
    }

    if (!ubah_transaksi_di_file(id, &trx)) {
        atur_pesan_error(error_msg, "Gagal menyimpan perubahan");
        return 0;
    }

    /* Update kalkulasi pos untuk bulan lama dan baru */
    int new_bulan = ekstrak_bulan(trx.tanggal);
    if (old_bulan > 0) update_semua_pos_bulan(old_bulan);
    if (new_bulan > 0 && new_bulan != old_bulan) update_semua_pos_bulan(new_bulan);

    return 1;
}

/**
 * Menghapus transaksi dengan validasi
 */
int hapus_transaksi(const char *id, char *error_msg) {
    if (!id) {
        atur_pesan_error(error_msg, "ID transaksi tidak valid");
        return 0;
    }

    /* Load transaksi untuk dapat bulan */
    Transaksi trx;
    if (!cari_transaksi_berdasarkan_id(id, &trx)) {
        atur_pesan_error(error_msg, "Transaksi tidak ditemukan");
        return 0;
    }

    int bulan = ekstrak_bulan(trx.tanggal);

    if (!hapus_transaksi_dari_file(id)) {
        atur_pesan_error(error_msg, "Gagal menghapus transaksi");
        return 0;
    }

    /* Update kalkulasi pos */
    if (bulan > 0) {
        update_semua_pos_bulan(bulan);
    }

    return 1;
}

/**
 * Mengambil daftar transaksi untuk bulan tertentu
 */
int ambil_daftar_transaksi(Transaksi *list, int max_count, int bulan) {
    if (!list || max_count <= 0) return 0;

    if (bulan > 0 && bulan <= 12) {
        return muat_transaksi_bulan(list, max_count, bulan);
    } else {
        return muat_transaksi(list, max_count);
    }
}

/**
 * Mengambil transaksi berdasarkan ID
 */
int ambil_transaksi_berdasarkan_id(const char *id, Transaksi *result) {
    return cari_transaksi_berdasarkan_id(id, result);
}

/* ===== IMPLEMENTASI PERHITUNGAN TRANSAKSI ===== */

/**
 * Menghitung total pemasukan untuk bulan tertentu
 */
unsigned long long hitung_total_pemasukan_transaksi(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi_bulan(list, MAX_TRANSAKSI, bulan);

    unsigned long long total = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].jenis == JENIS_PEMASUKAN) {
            total += list[i].nominal;
        }
    }

    return total;
}

/**
 * Menghitung total pengeluaran untuk bulan tertentu
 */
unsigned long long hitung_total_pengeluaran_transaksi(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi_bulan(list, MAX_TRANSAKSI, bulan);

    unsigned long long total = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].jenis == JENIS_PENGELUARAN) {
            total += list[i].nominal;
        }
    }

    return total;
}

/**
 * Menghitung jumlah transaksi untuk bulan tertentu
 */
int hitung_jumlah_transaksi(int bulan, int jenis) {
    if (bulan < 1 || bulan > 12) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi_bulan(list, MAX_TRANSAKSI, bulan);

    if (jenis < 0) return count;

    int total = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].jenis == jenis) {
            total++;
        }
    }

    return total;
}

/* ===== IMPLEMENTASI TAMPILAN TRANSAKSI ===== */

int tampilkan_daftar_transaksi(int bulan, int selected) {
    Transaksi list[MAX_TRANSAKSI];
    int count = ambil_daftar_transaksi(list, MAX_TRANSAKSI, bulan);

    char title[64];
    snprintf(title, sizeof(title), "Transaksi - %s", dapatkan_nama_bulan(bulan));

    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_aktifkan_tebal();
    tui_cetak(4, 2, title);
    tui_nonaktifkan_tebal();
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);

    return tampilkan_tabel_transaksi(list, count, 6, selected);
}

int tampilkan_detail_transaksi(Transaksi *trx, int start_y) {
    if (!trx) return start_y;

    int y = start_y;
    char buffer[64];

    tui_aktifkan_tebal();
    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_cetak(y++, 2, "Detail Transaksi");
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);
    tui_nonaktifkan_tebal();

    tui_gambar_garis_horizontal(y++, 2, 50, '-');

    tui_cetakf(y++, 2, "ID          : %s", trx->id);
    tui_cetakf(y++, 2, "Tanggal     : %s", trx->tanggal);

    tui_cetak(y, 2, "Jenis       : ");
    if (trx->jenis == JENIS_PENGELUARAN) {
        tui_aktifkan_warna(COLOR_PAIR_RED);
        tui_cetak(y++, 16, "Pengeluaran");
    } else {
        tui_aktifkan_warna(COLOR_PAIR_GREEN);
        tui_cetak(y++, 16, "Pemasukan");
    }
    tui_reset_atribut();

    tui_cetakf(y++, 2, "Pos         : %s", trx->pos);

    format_rupiah(trx->nominal, buffer);
    tui_cetakf(y++, 2, "Nominal     : %s", buffer);

    tui_cetakf(y++, 2, "Deskripsi   : %s",
               strlen(trx->deskripsi) > 0 ? trx->deskripsi : "-");

    return y + 1;
}

int tampilkan_ringkasan_transaksi(int bulan, int start_y) {
    int y = start_y;
    char buffer[64];

    unsigned long long pemasukan = hitung_total_pemasukan_transaksi(bulan);
    unsigned long long pengeluaran = hitung_total_pengeluaran_transaksi(bulan);
    long long saldo = (long long)pemasukan - (long long)pengeluaran;
    int jml_masuk = hitung_jumlah_transaksi(bulan, JENIS_PEMASUKAN);
    int jml_keluar = hitung_jumlah_transaksi(bulan, JENIS_PENGELUARAN);

    tui_gambar_garis_horizontal(y++, 2, 60, '=');

    tui_aktifkan_tebal();
    tui_cetak(y++, 2, "RINGKASAN:");
    tui_nonaktifkan_tebal();

    format_rupiah(pemasukan, buffer);
    tui_cetakf(y++, 2, "Total Pemasukan   : %s (%d transaksi)", buffer, jml_masuk);

    format_rupiah(pengeluaran, buffer);
    tui_cetakf(y++, 2, "Total Pengeluaran : %s (%d transaksi)", buffer, jml_keluar);

    format_saldo(saldo, buffer);
    tui_cetak(y, 2, "Saldo             : ");
    if (saldo < 0) {
        tui_aktifkan_warna(COLOR_PAIR_RED);
    } else {
        tui_aktifkan_warna(COLOR_PAIR_GREEN);
    }
    tui_cetak(y++, 22, buffer);
    tui_reset_atribut();

    return y + 1;
}

int tampilkan_form_tambah_transaksi(int bulan) {
    tui_hapus_layar();
    tampilkan_header("TAMBAH TRANSAKSI");

    int y = 5;
    char tanggal[12], pos[MAX_POS_LENGTH + 1], deskripsi[MAX_DESKRIPSI_LENGTH + 1];
    unsigned long long nominal;
    int jenis;

    /* Info bulan */
    char bulan_info[64];
    snprintf(bulan_info, sizeof(bulan_info), "Bulan: %s", dapatkan_nama_bulan(bulan));
    tui_cetak(y++, 2, bulan_info);
    y++;

    /* Input tanggal dengan validasi bulan */
    while (1) {
        if (!masukan_tanggal(y, 2, "Tanggal", tanggal)) {
            return 0;
        }
        /* Validasi bulan sesuai */
        if (!validasi_bulan_sesuai(tanggal, bulan)) {
            char error_msg[128];
            snprintf(error_msg, sizeof(error_msg),
                     "Anda memasukkan %s yang berbeda dari bulan terpilih! Input tidak valid!",
                     tanggal);
            tampilkan_error(error_msg);
            /* Gambar ulang form */
            tui_hapus_layar();
            tampilkan_header("TAMBAH TRANSAKSI");
            y = 5;
            tui_cetak(y++, 2, bulan_info);
            y++;
            continue;
        }
        y++;
        break;
    }

    /* Input jenis */
    if (!masukan_jenis_transaksi(y++, 2, &jenis)) {
        return 0;
    }
    y++;

    /* Pilih pos (otomatis untuk Pemasukan) */
    if (jenis == JENIS_PENGELUARAN) {
        tui_cetak(y++, 2, "Pilih Pos Anggaran:");
        if (!tampilkan_pilih_pos(bulan, pos)) {
            tampilkan_peringatan("Pos anggaran harus dipilih");
            return 0;
        }
    } else {
        /* Untuk pemasukan, pos otomatis "Pemasukan" */
        strcpy(pos, "Pemasukan");
    }

    /* Kembali ke form */
    tui_hapus_layar();
    tampilkan_header("TAMBAH TRANSAKSI");
    y = 5;
    tui_cetakf(y++, 2, "Tanggal : %s", tanggal);
    tui_cetakf(y++, 2, "Jenis   : %s", dapatkan_label_jenis(jenis));
    if (jenis == JENIS_PENGELUARAN) {
        tui_cetakf(y++, 2, "Pos     : %s", pos);
    } else {
        tui_cetakf(y++, 2, "Pos     : %s (Otomatis)", pos);
    }
    y++;

    /* Input nominal */
    if (!masukan_angka_min(y++, 2, "Nominal: Rp ", &nominal, 1)) {
        return 0;
    }

    /* Input deskripsi (opsional) */
    if (!masukan_string(y++, 2, "Deskripsi (opsional): ", deskripsi, sizeof(deskripsi))) {
        return 0;
    }

    /* Konfirmasi */
    y++;
    if (!masukan_konfirmasi(y, 2, "Simpan transaksi?")) {
        tampilkan_info("Dibatalkan");
        return 0;
    }

    /* Simpan */
    char error_msg[100];
    if (tambah_transaksi(tanggal, jenis, pos, nominal, deskripsi, error_msg)) {
        tampilkan_sukses("Transaksi berhasil ditambahkan");
        return 1;
    } else {
        tampilkan_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_form_edit_transaksi(const char *id) {
    Transaksi trx;
    if (!ambil_transaksi_berdasarkan_id(id, &trx)) {
        tampilkan_error("Transaksi tidak ditemukan");
        return 0;
    }

    /* Ambil bulan dari transaksi yang ada untuk validasi */
    int bulan_trx = ekstrak_bulan(trx.tanggal);

    char tanggal_baru[12], deskripsi_baru[MAX_DESKRIPSI_LENGTH + 1], nominal_input[32];
    unsigned long long nominal_baru = 0;

    while (1) {
        tui_hapus_layar();
        tampilkan_header("EDIT TRANSAKSI");

        int y = 5;
        char nominal_str[64];
        format_rupiah(trx.nominal, nominal_str);

        /* Tampilkan data saat ini */
        tui_cetak(y++, 2, "Data saat ini:");
        tui_cetakf(y++, 2, "  ID        : %s", trx.id);
        tui_cetakf(y++, 2, "  Tanggal   : %s", trx.tanggal);
        tui_cetakf(y++, 2, "  Jenis     : %s", dapatkan_label_jenis(trx.jenis));
        tui_cetakf(y++, 2, "  Pos       : %s", trx.pos);
        tui_cetakf(y++, 2, "  Nominal   : %s", nominal_str);
        tui_cetakf(y++, 2, "  Deskripsi : %s", trx.deskripsi);
        y++;

        tui_cetak(y++, 2, "Masukkan data baru (kosongkan jika tidak diubah):");
        y++;

        /* Input tanggal baru dengan validasi langsung */
        if (!masukan_string(y++, 2, "Tanggal baru (dd-mm-YYYY): ", tanggal_baru, sizeof(tanggal_baru))) {
            return 0;
        }

        /* Validasi langsung setelah input tanggal */
        if (strlen(tanggal_baru) > 0) {
            if (!validasi_tanggal_lengkap(tanggal_baru)) {
                tampilkan_error("Format tanggal tidak valid! Gunakan format dd-mm-YYYY");
                continue;
            }
            /* Validasi bulan sesuai dengan transaksi asli */
            if (!validasi_bulan_sesuai(tanggal_baru, bulan_trx)) {
                char error_msg[128];
                snprintf(error_msg, sizeof(error_msg),
                         "Anda memasukkan %s yang berbeda dari bulan terpilih! Input tidak valid!",
                         tanggal_baru);
                tampilkan_error(error_msg);
                continue;
            }
        }

        /* Input nominal baru dengan validasi langsung */
        if (!masukan_string(y++, 2, "Nominal baru: Rp ", nominal_input, sizeof(nominal_input))) {
            return 0;
        }
        if (strlen(nominal_input) > 0) {
            if (!urai_nominal(nominal_input, &nominal_baru) || nominal_baru == 0) {
                tampilkan_error("Nominal tidak valid! Harus berupa angka lebih dari 0");
                continue;
            }
        }

        /* Input deskripsi baru dengan validasi langsung */
        if (!masukan_string(y++, 2, "Deskripsi baru: ", deskripsi_baru, sizeof(deskripsi_baru))) {
            return 0;
        }
        if (strlen(deskripsi_baru) > MAX_DESKRIPSI_LENGTH) {
            tampilkan_error("Deskripsi terlalu panjang (max 50 karakter)");
            continue;
        }

        break; /* Semua validasi berhasil */
    }

    /* Cek perubahan */
    if (strlen(tanggal_baru) == 0 && nominal_baru == 0 && strlen(deskripsi_baru) == 0) {
        tampilkan_info("Tidak ada perubahan");
        return 0;
    }

    /* Konfirmasi */
    if (!tampilkan_konfirmasi("Simpan perubahan?")) {
        tampilkan_info("Dibatalkan");
        return 0;
    }

    /* Simpan */
    char error_msg[100];
    const char *tgl_param = strlen(tanggal_baru) > 0 ? tanggal_baru : NULL;
    const char *desk_param = strlen(deskripsi_baru) > 0 ? deskripsi_baru : NULL;

    if (ubah_transaksi(id, tgl_param, -1, NULL, nominal_baru, desk_param, error_msg)) {
        tampilkan_sukses("Transaksi berhasil diupdate");
        return 1;
    } else {
        tampilkan_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_konfirmasi_hapus_transaksi(const char *id) {
    Transaksi trx;
    if (!ambil_transaksi_berdasarkan_id(id, &trx)) {
        tampilkan_error("Transaksi tidak ditemukan");
        return 0;
    }

    char nominal_str[30];
    format_rupiah(trx.nominal, nominal_str);

    char msg[128];
    snprintf(msg, sizeof(msg), "Hapus transaksi %s (%s %s)?",
             trx.id, dapatkan_label_jenis(trx.jenis), nominal_str);

    if (!tampilkan_konfirmasi(msg)) {
        tampilkan_info("Dibatalkan");
        return 0;
    }

    char error_msg[100];
    if (hapus_transaksi(id, error_msg)) {
        tampilkan_sukses("Transaksi berhasil dihapus");
        return 1;
    } else {
        tampilkan_error("%s", error_msg);
        return 0;
    }
}

/* ===== IMPLEMENTASI HANDLER TRANSAKSI ===== */

/**
 * Jalankan modul transaksi
 */
void jalankan_modul_transaksi(int bulan_awal) {
    int bulan = bulan_awal;

    /* Pastikan pos anggaran ada untuk bulan ini */
    inisialisasi_pos_untuk_bulan(bulan);

    while (1) {
        int action = menu_transaksi_utama(bulan);

        switch (action) {
            case ACT_VIEW:
                penanganan_lihat_transaksi(bulan);
                break;

            case ACT_TAMBAH:
                penanganan_tambah_transaksi(bulan);
                break;

            case ACT_EDIT:
                penanganan_ubah_transaksi(bulan);
                break;

            case ACT_HAPUS:
                penanganan_hapus_transaksi(bulan);
                break;

            case ACT_KEMBALI:
            case CANCEL:
                return;
        }
    }
}

int menu_transaksi_utama(int bulan) {
    char title[64];
    snprintf(title, sizeof(title), "Menu Transaksi - %s", dapatkan_nama_bulan(bulan));

    Menu menu;
    menu_inisialisasi(&menu, title);

    menu_tambah_item(&menu, "Lihat Daftar Transaksi", ACT_VIEW);
    menu_tambah_item(&menu, "Tambah Transaksi", ACT_TAMBAH);
    menu_tambah_item(&menu, "Edit Transaksi", ACT_EDIT);
    menu_tambah_item(&menu, "Hapus Transaksi", ACT_HAPUS);

    return menu_navigasi(&menu);
}

/**
 * Handler untuk view daftar transaksi
 */
void penanganan_lihat_transaksi(int bulan) {
    int selected = 0;
    Transaksi list[MAX_TRANSAKSI];
    int count = ambil_daftar_transaksi(list, MAX_TRANSAKSI, bulan);

    while (1) {
        tui_hapus_layar();
        tampilkan_header("DAFTAR TRANSAKSI");

        int next_y = tampilkan_daftar_transaksi(bulan, selected);
        next_y = tampilkan_ringkasan_transaksi(bulan, next_y);

        tampilkan_footer("ATAS/BAWAH: Navigasi | ENTER: Detail | ESC: Kembali");
        tui_segarkan();

        int ch = tui_ambil_karakter();

        switch (ch) {
            case KEY_UP:
            case 'k':
                if (selected > 0) selected--;
                break;

            case KEY_DOWN:
            case 'j':
                if (selected < count - 1) selected++;
                break;

            case KEY_ENTER:
            case '\n':
            case '\r':
                if (count > 0) {
                    tui_hapus_layar();
                    tampilkan_header("DETAIL TRANSAKSI");
                    tampilkan_detail_transaksi(&list[selected], 5);
                    tampilkan_footer("Tekan sembarang tombol untuk kembali");
                    tui_segarkan();
                    tui_ambil_karakter();

                    count = ambil_daftar_transaksi(list, MAX_TRANSAKSI, bulan);
                    if (selected >= count) selected = count - 1;
                    if (selected < 0) selected = 0;
                }
                break;

            case 27:
                return;
        }
    }
}

/**
 * Handler untuk tambah transaksi
 */
void penanganan_tambah_transaksi(int bulan) {
    tampilkan_form_tambah_transaksi(bulan);
}

/**
 * Handler untuk edit transaksi
 */
void penanganan_ubah_transaksi(int bulan) {
    Transaksi list[MAX_TRANSAKSI];
    int count = ambil_daftar_transaksi(list, MAX_TRANSAKSI, bulan);

    if (count == 0) {
        tampilkan_peringatan("Belum ada transaksi");
        return;
    }

    Menu menu;
    menu_inisialisasi(&menu, "Pilih Transaksi untuk Diedit");

    char nominal_str[30];
    for (int i = 0; i < count && i < 15; i++) {
        char item[80];
        format_rupiah(list[i].nominal, nominal_str);
        snprintf(item, sizeof(item), "%s | %s | %s | %s",
                 list[i].id, list[i].tanggal,
                 list[i].jenis == JENIS_PENGELUARAN ? "Out" : "In",
                 nominal_str);
        menu_tambah_item(&menu, item, i);
    }

    int pilihan = menu_navigasi(&menu);

    if (pilihan == CANCEL || pilihan < 0) {
        return;
    }

    tampilkan_form_edit_transaksi(list[pilihan].id);
}

/**
 * Handler untuk hapus transaksi
 */
void penanganan_hapus_transaksi(int bulan) {
    Transaksi list[MAX_TRANSAKSI];
    int count = ambil_daftar_transaksi(list, MAX_TRANSAKSI, bulan);

    if (count == 0) {
        tampilkan_peringatan("Belum ada transaksi");
        return;
    }

    Menu menu;
    menu_inisialisasi(&menu, "Pilih Transaksi untuk Dihapus");

    char nominal_str[30];
    for (int i = 0; i < count && i < 15; i++) {
        char item[80];
        format_rupiah(list[i].nominal, nominal_str);
        snprintf(item, sizeof(item), "%s | %s | %s | %s",
                 list[i].id, list[i].tanggal,
                 list[i].jenis == JENIS_PENGELUARAN ? "Out" : "In",
                 nominal_str);
        menu_tambah_item(&menu, item, i);
    }

    int pilihan = menu_navigasi(&menu);

    if (pilihan == CANCEL || pilihan < 0) {
        return;
    }

    tampilkan_konfirmasi_hapus_transaksi(list[pilihan].id);
}
