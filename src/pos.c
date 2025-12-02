#include <stdio.h>
#include <string.h>
#include "pos.h"
#include "file.h"
#include "tui.h"
#include "utils.h"
#include "validator.h"

/* ===== KONSTANTA LOKAL ===== */
/* Aksi Menu */
#define ACT_VIEW    1
#define ACT_TAMBAH  2
#define ACT_EDIT    3
#define ACT_HAPUS   4
#define ACT_BULAN   5
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

/* ===== IMPLEMENTASI PERHITUNGAN POS ===== */

/**
 * Menghitung realisasi pos dari transaksi
 */
unsigned long long hitung_realisasi_pos(const char *nama_pos, int bulan) {
    if (!nama_pos || bulan < 1 || bulan > 12) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi_bulan(list, MAX_TRANSAKSI, bulan);

    unsigned long long total = 0;
    for (int i = 0; i < count; i++) {
        /* Hanya hitung pengeluaran untuk pos yang sesuai */
        if (list[i].jenis == JENIS_PENGELUARAN &&
            banding_string_tanpa_case(list[i].pos, nama_pos) == 0) {
            total += list[i].nominal;
        }
    }

    return total;
}

/**
 * Menghitung sisa anggaran pos
 */
long long hitung_sisa_pos(unsigned long long nominal, unsigned long long realisasi) {
    return (long long)nominal - (long long)realisasi;
}

/**
 * Menentukan status pos berdasarkan sisa
 */
int tentukan_status_pos(long long sisa) {
    return (sisa >= 0) ? STATUS_AMAN : STATUS_TIDAK_AMAN;
}

/**
 * Mengupdate kalkulasi untuk satu pos
 */
void update_kalkulasi_pos(PosAnggaran *pos, int bulan) {
    if (!pos || bulan < 1 || bulan > 12) return;

    /* Hitung realisasi dari transaksi */
    pos->realisasi = hitung_realisasi_pos(pos->nama, bulan);

    /* Hitung sisa */
    pos->sisa = hitung_sisa_pos(pos->nominal, pos->realisasi);

    /* Tentukan status */
    pos->status = tentukan_status_pos(pos->sisa);

    /* Hitung jumlah transaksi */
    pos->jumlah_transaksi = hitung_jumlah_transaksi_pos(pos->nama, bulan);
}

/**
 * Mengupdate kalkulasi untuk semua pos dalam bulan
 */
int update_semua_pos_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    if (count == 0) return 1;  /* Tidak ada pos, sukses */

    /* Update kalkulasi untuk setiap pos */
    for (int i = 0; i < count; i++) {
        update_kalkulasi_pos(&list[i], bulan);
    }

    /* Simpan kembali */
    return simpan_pos_bulan(list, count, bulan);
}

/**
 * Menghitung jumlah transaksi untuk pos dalam bulan
 */
int hitung_jumlah_transaksi_pos(const char *nama_pos, int bulan) {
    if (!nama_pos || bulan < 1 || bulan > 12) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi_bulan(list, MAX_TRANSAKSI, bulan);

    int total = 0;
    for (int i = 0; i < count; i++) {
        if (banding_string_tanpa_case(list[i].pos, nama_pos) == 0) {
            total++;
        }
    }

    return total;
}

/**
 * Menghitung total anggaran semua pos dalam bulan
 */
unsigned long long hitung_total_anggaran(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    unsigned long long total = 0;
    for (int i = 0; i < count; i++) {
        total += list[i].nominal;
    }

    return total;
}

/**
 * Menghitung total realisasi semua pos dalam bulan
 */
unsigned long long hitung_total_realisasi(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    /* Update kalkulasi dulu */
    for (int i = 0; i < count; i++) {
        update_kalkulasi_pos(&list[i], bulan);
    }

    unsigned long long total = 0;
    for (int i = 0; i < count; i++) {
        total += list[i].realisasi;
    }

    return total;
}

/* ===== IMPLEMENTASI CRUD POS ===== */

/**
 * Menambahkan pos anggaran baru dengan validasi
 */
int tambah_pos(const char *nama, unsigned long long nominal, int bulan, char *error_msg) {
    if (!nama || bulan < 1 || bulan > 12) {
        atur_pesan_error(error_msg, "Parameter tidak valid");
        return 0;
    }

    /* Validasi nama */
    if (!validasi_tidak_kosong(nama)) {
        atur_pesan_error(error_msg, "Nama pos tidak boleh kosong");
        return 0;
    }

    if (!validasi_panjang_pos(nama)) {
        atur_pesan_error(error_msg, "Nama pos terlalu panjang (max 20 karakter)");
        return 0;
    }

    if (!validasi_tidak_ada_pipe(nama)) {
        atur_pesan_error(error_msg, "Nama pos tidak boleh mengandung karakter |");
        return 0;
    }

    /* Cek nama unik */
    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    if (!validasi_pos_unik(nama, list, count)) {
        atur_pesan_error(error_msg, "Nama pos sudah ada");
        return 0;
    }

    /* Validasi nominal */
    if (nominal == 0) {
        atur_pesan_error(error_msg, "Nominal anggaran harus lebih dari 0");
        return 0;
    }

    /* Buat pos baru */
    PosAnggaran pos;
    memset(&pos, 0, sizeof(PosAnggaran));
    salin_string_aman(pos.nama, nama, sizeof(pos.nama));
    pos.nominal = nominal;

    if (!tambah_pos_ke_file(&pos, bulan)) {
        atur_pesan_error(error_msg, "Gagal menyimpan pos anggaran");
        return 0;
    }

    return 1;
}

/**
 * Mengedit pos anggaran dengan validasi
 */
int ubah_pos(int no, const char *nama_baru, unsigned long long nominal_baru, int bulan, char *error_msg) {
    if (no <= 0 || bulan < 1 || bulan > 12) {
        atur_pesan_error(error_msg, "Parameter tidak valid");
        return 0;
    }

    /* Load pos yang akan diedit */
    PosAnggaran pos;
    if (!cari_pos_berdasarkan_nomor(no, bulan, &pos)) {
        atur_pesan_error(error_msg, "Pos tidak ditemukan");
        return 0;
    }

    /* Validasi nama baru jika diubah */
    if (nama_baru && strlen(nama_baru) > 0) {
        if (!validasi_panjang_pos(nama_baru)) {
            atur_pesan_error(error_msg, "Nama pos terlalu panjang (max 20 karakter)");
            return 0;
        }

        if (!validasi_tidak_ada_pipe(nama_baru)) {
            atur_pesan_error(error_msg, "Nama pos tidak boleh mengandung karakter |");
            return 0;
        }

        /* Cek nama unik (kecuali pos ini sendiri) */
        PosAnggaran list[MAX_POS];
        int count = muat_pos_bulan(list, MAX_POS, bulan);

        if (!validasi_pos_unik_edit(nama_baru, no, list, count)) {
            atur_pesan_error(error_msg, "Nama pos sudah digunakan");
            return 0;
        }

        salin_string_aman(pos.nama, nama_baru, sizeof(pos.nama));
    }

    /* Update nominal jika diubah */
    if (nominal_baru > 0) {
        pos.nominal = nominal_baru;
    }

    /* Update kalkulasi */
    update_kalkulasi_pos(&pos, bulan);

    if (!ubah_pos_di_file(no, &pos, bulan)) {
        atur_pesan_error(error_msg, "Gagal menyimpan perubahan");
        return 0;
    }

    return 1;
}

/**
 * Menghapus pos anggaran dengan validasi
 */
int hapus_pos(int no, int bulan, char *error_msg) {
    if (no <= 0 || bulan < 1 || bulan > 12) {
        atur_pesan_error(error_msg, "Parameter tidak valid");
        return 0;
    }

    /* Load pos yang akan dihapus */
    PosAnggaran pos;
    if (!cari_pos_berdasarkan_nomor(no, bulan, &pos)) {
        atur_pesan_error(error_msg, "Pos tidak ditemukan");
        return 0;
    }

    /* Update kalkulasi untuk cek transaksi terkait */
    update_kalkulasi_pos(&pos, bulan);

    /* Cek apakah pos bisa dihapus */
    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    /* Cari index pos */
    int idx = -1;
    for (int i = 0; i < count; i++) {
        if (list[i].no == no) {
            list[i] = pos;  /* Update dengan kalkulasi terbaru */
            idx = i;
            break;
        }
    }

    if (idx < 0 || !validasi_pos_bisa_hapus(no, list, count)) {
        atur_pesan_error(error_msg, "Pos tidak bisa dihapus karena masih ada transaksi terkait");
        return 0;
    }

    if (!hapus_pos_dari_file(no, bulan)) {
        atur_pesan_error(error_msg, "Gagal menghapus pos");
        return 0;
    }

    return 1;
}

/**
 * Mengambil daftar pos untuk bulan dengan kalkulasi terbaru
 */
int ambil_daftar_pos(PosAnggaran *list, int max_count, int bulan) {
    if (!list || max_count <= 0 || bulan < 1 || bulan > 12) return 0;

    int count = muat_pos_bulan(list, max_count, bulan);

    /* Update kalkulasi untuk setiap pos */
    for (int i = 0; i < count; i++) {
        update_kalkulasi_pos(&list[i], bulan);
    }

    return count;
}

/**
 * Mengambil satu pos berdasarkan nomor dengan kalkulasi terbaru
 */
int ambil_pos_berdasarkan_nomor(int no, int bulan, PosAnggaran *result) {
    if (no <= 0 || bulan < 1 || bulan > 12 || !result) return 0;

    if (!cari_pos_berdasarkan_nomor(no, bulan, result)) return 0;

    /* Update kalkulasi */
    update_kalkulasi_pos(result, bulan);

    return 1;
}

/**
 * Mengambil daftar nama pos untuk dropdown/pilihan
 */
int ambil_nama_pos(char names[][MAX_POS_LENGTH + 1], int max_count, int bulan) {
    if (!names || max_count <= 0 || bulan < 1 || bulan > 12) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    int result_count = (count < max_count) ? count : max_count;

    for (int i = 0; i < result_count; i++) {
        salin_string_aman(names[i], list[i].nama, MAX_POS_LENGTH + 1);
    }

    return result_count;
}

/**
 * Memeriksa apakah pos dengan nama tertentu sudah ada
 */
int cek_pos_ada(const char *nama, int bulan) {
    if (!nama || bulan < 1 || bulan > 12) return 0;

    PosAnggaran result;
    return cari_pos_berdasarkan_nama(nama, bulan, &result);
}

/**
 * Me-refresh kalkulasi semua pos untuk bulan
 */
int segarkan_pos(int bulan) {
    return update_semua_pos_bulan(bulan);
}

/* ===== IMPLEMENTASI TAMPILAN POS ===== */

int tampilkan_daftar_pos(int bulan, int selected) {
    PosAnggaran list[MAX_POS];
    int count = ambil_daftar_pos(list, MAX_POS, bulan);

    /* Tampilkan header bulan */
    char title[64];
    snprintf(title, sizeof(title), "Pos Anggaran - %s", dapatkan_nama_bulan(bulan));

    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_aktifkan_tebal();
    tui_cetak(4, 2, title);
    tui_nonaktifkan_tebal();
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);

    return tampilkan_tabel_pos(list, count, 6, selected);
}

int tampilkan_detail_pos(PosAnggaran *pos, int start_y) {
    if (!pos) return start_y;

    int y = start_y;
    char buffer[64];

    tui_aktifkan_tebal();
    tui_aktifkan_warna(COLOR_PAIR_CYAN);
    tui_cetak(y++, 2, "Detail Pos Anggaran");
    tui_nonaktifkan_warna(COLOR_PAIR_CYAN);
    tui_nonaktifkan_tebal();

    tui_gambar_garis_horizontal(y++, 2, 50, '-');

    tui_cetakf(y++, 2, "Nomor       : %d", pos->no);
    tui_cetakf(y++, 2, "Nama        : %s", pos->nama);

    format_rupiah(pos->nominal, buffer);
    tui_cetakf(y++, 2, "Anggaran    : %s", buffer);

    format_rupiah(pos->realisasi, buffer);
    tui_cetakf(y++, 2, "Realisasi   : %s", buffer);

    format_saldo(pos->sisa, buffer);
    tui_cetak(y, 2, "Sisa        : ");
    if (pos->sisa < 0) {
        tui_aktifkan_warna(COLOR_PAIR_RED);
    } else {
        tui_aktifkan_warna(COLOR_PAIR_GREEN);
    }
    tui_cetak(y++, 16, buffer);
    tui_nonaktifkan_warna(pos->sisa < 0 ? COLOR_PAIR_RED : COLOR_PAIR_GREEN);

    tui_cetakf(y++, 2, "Transaksi   : %d", pos->jumlah_transaksi);

    tui_cetak(y, 2, "Status      : ");
    if (pos->status == STATUS_AMAN) {
        tui_aktifkan_warna(COLOR_PAIR_GREEN);
        tui_cetak(y++, 16, "AMAN");
    } else {
        tui_aktifkan_warna(COLOR_PAIR_RED);
        tui_cetak(y++, 16, "TIDAK AMAN");
    }
    tui_reset_atribut();

    return y + 1;
}

int tampilkan_ringkasan_pos(int bulan, int start_y) {
    int y = start_y;
    char buffer[64];

    unsigned long long total_anggaran = hitung_total_anggaran(bulan);
    unsigned long long total_realisasi = hitung_total_realisasi(bulan);
    long long total_sisa = (long long)total_anggaran - (long long)total_realisasi;

    tui_gambar_garis_horizontal(y++, 2, 60, '=');

    tui_aktifkan_tebal();
    tui_cetak(y++, 2, "RINGKASAN:");
    tui_nonaktifkan_tebal();

    format_rupiah(total_anggaran, buffer);
    tui_cetakf(y++, 2, "Total Anggaran  : %s", buffer);

    format_rupiah(total_realisasi, buffer);
    tui_cetakf(y++, 2, "Total Realisasi : %s", buffer);

    format_saldo(total_sisa, buffer);
    tui_cetak(y, 2, "Total Sisa      : ");
    if (total_sisa < 0) {
        tui_aktifkan_warna(COLOR_PAIR_RED);
    } else {
        tui_aktifkan_warna(COLOR_PAIR_GREEN);
    }
    tui_cetak(y++, 20, buffer);
    tui_reset_atribut();

    return y + 1;
}

int tampilkan_form_tambah_pos(int bulan) {
    tui_hapus_layar();
    tampilkan_header("TAMBAH POS ANGGARAN");

    char nama[MAX_POS_LENGTH + 1];
    unsigned long long nominal;

    int y = 5;

    char bulan_info[64];
    snprintf(bulan_info, sizeof(bulan_info), "Bulan: %s", dapatkan_nama_bulan(bulan));
    tui_cetak(y++, 2, bulan_info);
    y++;

    /* Input nama */
    if (!masukan_string_wajib(y++, 2, "Nama Pos: ", nama, sizeof(nama))) {
        return 0;
    }

    /* Input nominal */
    if (!masukan_angka_min(y++, 2, "Anggaran: Rp ", &nominal, 1)) {
        return 0;
    }

    /* Konfirmasi */
    y++;
    if (!masukan_konfirmasi(y, 2, "Simpan pos anggaran?")) {
        tampilkan_info("Dibatalkan");
        return 0;
    }

    /* Simpan */
    char error_msg[100];
    if (tambah_pos(nama, nominal, bulan, error_msg)) {
        tampilkan_sukses("Pos anggaran berhasil ditambahkan");
        return 1;
    } else {
        tampilkan_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_form_edit_pos(int no, int bulan) {
    PosAnggaran pos;
    if (!ambil_pos_berdasarkan_nomor(no, bulan, &pos)) {
        tampilkan_error("Pos tidak ditemukan");
        return 0;
    }

    tui_hapus_layar();
    tampilkan_header("EDIT POS ANGGARAN");

    int y = 5;

    /* Tampilkan data saat ini */
    tui_cetak(y++, 2, "Data saat ini:");
    tui_cetakf(y++, 2, "  Nama    : %s", pos.nama);
    char nominal_str[30];
    format_rupiah(pos.nominal, nominal_str);
    tui_cetakf(y++, 2, "  Anggaran: %s", nominal_str);
    y++;

    tui_cetak(y++, 2, "Masukkan data baru (kosongkan jika tidak diubah):");
    y++;

    char nama_baru[MAX_POS_LENGTH + 1];
    unsigned long long nominal_baru = 0;

    /* Input nama baru */
    if (!masukan_string(y++, 2, "Nama baru: ", nama_baru, sizeof(nama_baru))) {
        return 0;
    }

    /* Input nominal baru */
    char nominal_input[32];
    if (!masukan_string(y++, 2, "Anggaran baru: Rp ", nominal_input, sizeof(nominal_input))) {
        return 0;
    }
    if (strlen(nominal_input) > 0) {
        urai_nominal(nominal_input, &nominal_baru);
    }

    /* Cek apakah ada perubahan */
    if (strlen(nama_baru) == 0 && nominal_baru == 0) {
        tampilkan_info("Tidak ada perubahan");
        return 0;
    }

    /* Konfirmasi */
    y++;
    if (!masukan_konfirmasi(y, 2, "Simpan perubahan?")) {
        tampilkan_info("Dibatalkan");
        return 0;
    }

    /* Simpan */
    char error_msg[100];
    const char *nama_param = strlen(nama_baru) > 0 ? nama_baru : NULL;

    if (ubah_pos(no, nama_param, nominal_baru, bulan, error_msg)) {
        tampilkan_sukses("Pos anggaran berhasil diupdate");
        return 1;
    } else {
        tampilkan_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_konfirmasi_hapus_pos(int no, int bulan) {
    PosAnggaran pos;
    if (!ambil_pos_berdasarkan_nomor(no, bulan, &pos)) {
        tampilkan_error("Pos tidak ditemukan");
        return 0;
    }

    /* Cek apakah bisa dihapus */
    if (pos.jumlah_transaksi > 0) {
        tampilkan_error("Pos tidak bisa dihapus karena masih ada %d transaksi", pos.jumlah_transaksi);
        return 0;
    }

    char msg[100];
    snprintf(msg, sizeof(msg), "Hapus pos '%s'?", pos.nama);

    if (!tampilkan_konfirmasi(msg)) {
        tampilkan_info("Dibatalkan");
        return 0;
    }

    char error_msg[100];
    if (hapus_pos(no, bulan, error_msg)) {
        tampilkan_sukses("Pos anggaran berhasil dihapus");
        return 1;
    } else {
        tampilkan_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_pilih_pos(int bulan, char *result) {
    if (!result) return 0;

    PosAnggaran list[MAX_POS];
    int count = ambil_daftar_pos(list, MAX_POS, bulan);

    if (count == 0) {
        tampilkan_peringatan("Belum ada pos anggaran untuk bulan ini");
        return 0;
    }

    Menu menu;
    menu_inisialisasi(&menu, "Pilih Pos Anggaran");

    for (int i = 0; i < count; i++) {
        menu_tambah_item(&menu, list[i].nama, i + 1);
    }

    int pilihan = menu_navigasi(&menu);

    if (pilihan == CANCEL || pilihan <= 0) {
        return 0;
    }

    salin_string_aman(result, list[pilihan - 1].nama, MAX_POS_LENGTH + 1);
    return 1;
}

/* ===== IMPLEMENTASI HANDLER POS ===== */

/**
 * Jalankan modul pos anggaran
 */
void jalankan_modul_pos(int bulan_awal) {
    int bulan = bulan_awal;

    /* Inisialisasi pos untuk bulan jika belum ada */
    inisialisasi_pos_untuk_bulan(bulan);

    while (1) {
        /* Sinkronisasi kalkulasi */
        sinkronisasi_pos_transaksi(bulan);

        int action = menu_pos_utama(bulan);

        switch (action) {
            case ACT_VIEW:
                penanganan_lihat_pos(bulan);
                break;

            case ACT_TAMBAH:
                penanganan_tambah_pos(bulan);
                break;

            case ACT_EDIT:
                penanganan_ubah_pos(bulan);
                break;

            case ACT_HAPUS:
                penanganan_hapus_pos(bulan);
                break;

            case ACT_KEMBALI:
            case CANCEL:
                return;
        }
    }
}

/**
 * Inisialisasi data pos untuk bulan baru
 */
int inisialisasi_pos_untuk_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    /* Cek apakah sudah ada file */
    if (cek_file_pos_ada(bulan)) {
        return 1;  /* Sudah ada */
    }

    /* Cek bulan sebelumnya */
    int bulan_sebelum = (bulan == 1) ? 12 : bulan - 1;

    if (cek_file_pos_ada(bulan_sebelum)) {
        /* Tanyakan apakah mau copy dari bulan sebelumnya */
        char msg[100];
        snprintf(msg, sizeof(msg),
                 "Copy pos dari %s ke %s?",
                 dapatkan_nama_bulan(bulan_sebelum),
                 dapatkan_nama_bulan(bulan));

        if (tampilkan_konfirmasi(msg)) {
            if (salin_pos_dari_bulan(bulan, bulan_sebelum)) {
                tampilkan_sukses("Pos berhasil dicopy");
                return 1;
            } else {
                tampilkan_error("Gagal copy pos");
            }
        }
    }

    /* Buat file pos kosong */
    return inisialisasi_pos_bulan(bulan);
}

/**
 * Menyinkronisasi kalkulasi pos dengan transaksi
 */
int sinkronisasi_pos_transaksi(int bulan) {
    return update_semua_pos_bulan(bulan);
}

/**
 * Menampilkan menu utama pos anggaran
 */
int menu_pos_utama(int bulan) {
    char title[64];
    snprintf(title, sizeof(title), "Menu Pos Anggaran - %s", dapatkan_nama_bulan(bulan));

    Menu menu;
    menu_inisialisasi(&menu, title);

    menu_tambah_item(&menu, "Lihat Daftar Pos", ACT_VIEW);
    menu_tambah_item(&menu, "Tambah Pos Baru", ACT_TAMBAH);
    menu_tambah_item(&menu, "Edit Pos", ACT_EDIT);
    menu_tambah_item(&menu, "Hapus Pos", ACT_HAPUS);

    return menu_navigasi(&menu);
}

/**
 * Menampilkan menu pilih bulan
 */
int menu_pilih_bulan(int bulan_saat_ini) {
    Menu menu;
    menu_inisialisasi(&menu, "Pilih Bulan");

    for (int i = 1; i <= 12; i++) {
        char item[32];
        if (i == bulan_saat_ini) {
            snprintf(item, sizeof(item), "%s (aktif)", dapatkan_nama_bulan(i));
        } else {
            snprintf(item, sizeof(item), "%s", dapatkan_nama_bulan(i));
        }
        menu_tambah_item(&menu, item, i);
    }

    int pilihan = menu_navigasi(&menu);

    if (pilihan == CANCEL) {
        return bulan_saat_ini;  /* Tetap di bulan sekarang */
    }

    return pilihan;
}

/**
 * Handler untuk view daftar pos dengan navigasi
 */
void penanganan_lihat_pos(int bulan) {
    int selected = 0;
    PosAnggaran list[MAX_POS];
    int count = ambil_daftar_pos(list, MAX_POS, bulan);

    while (1) {
        tui_hapus_layar();
        tampilkan_header("DAFTAR POS ANGGARAN");

        int next_y = tampilkan_daftar_pos(bulan, selected);
        next_y = tampilkan_ringkasan_pos(bulan, next_y);

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
                    tampilkan_header("DETAIL POS ANGGARAN");
                    tampilkan_detail_pos(&list[selected], 5);
                    tampilkan_footer("Tekan sembarang tombol untuk kembali");
                    tui_segarkan();
                    tui_ambil_karakter();

                    /* Reload list */
                    count = ambil_daftar_pos(list, MAX_POS, bulan);
                    if (selected >= count) selected = count - 1;
                    if (selected < 0) selected = 0;
                }
                break;

            case 27:  /* ESC */
                return;
        }
    }
}

/**
 * Handler untuk tambah pos baru
 */
void penanganan_tambah_pos(int bulan) {
    tampilkan_form_tambah_pos(bulan);
}

/**
 * Handler untuk edit pos
 */
void penanganan_ubah_pos(int bulan) {
    PosAnggaran list[MAX_POS];
    int count = ambil_daftar_pos(list, MAX_POS, bulan);

    if (count == 0) {
        tampilkan_peringatan("Belum ada pos anggaran");
        return;
    }

    /* Pilih pos untuk diedit */
    Menu menu;
    menu_inisialisasi(&menu, "Pilih Pos untuk Diedit");

    for (int i = 0; i < count; i++) {
        char item[64];
        snprintf(item, sizeof(item), "%d. %s", list[i].no, list[i].nama);
        menu_tambah_item(&menu, item, list[i].no);
    }

    int pilihan = menu_navigasi(&menu);

    if (pilihan == CANCEL || pilihan <= 0) {
        return;
    }

    tampilkan_form_edit_pos(pilihan, bulan);
}

/**
 * Handler untuk hapus pos
 */
void penanganan_hapus_pos(int bulan) {
    PosAnggaran list[MAX_POS];
    int count = ambil_daftar_pos(list, MAX_POS, bulan);

    if (count == 0) {
        tampilkan_peringatan("Belum ada pos anggaran");
        return;
    }

    /* Pilih pos untuk dihapus */
    Menu menu;
    menu_inisialisasi(&menu, "Pilih Pos untuk Dihapus");

    for (int i = 0; i < count; i++) {
        char item[64];
        if (list[i].jumlah_transaksi > 0) {
            snprintf(item, sizeof(item), "%d. %s (ada %d transaksi)",
                     list[i].no, list[i].nama, list[i].jumlah_transaksi);
        } else {
            snprintf(item, sizeof(item), "%d. %s", list[i].no, list[i].nama);
        }
        menu_tambah_item(&menu, item, list[i].no);
    }

    int pilihan = menu_navigasi(&menu);

    if (pilihan == CANCEL || pilihan <= 0) {
        return;
    }

    tampilkan_konfirmasi_hapus_pos(pilihan, bulan);
}
