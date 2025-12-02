/**
 * @file pos.c
 * @brief Implementasi modul pos anggaran
 * @author Kelompok B11
 * @date 2025
 */

#include <stdio.h>
#include <string.h>
#include "pos.h"
#include "file.h"
#include "tui.h"
#include "utils.h"
#include "validator.h"
#include "constants.h"

/* --- pos_calc.c --- */

unsigned long long hitung_realisasi_pos(const char *nama_pos, int bulan) {
    if (!nama_pos || bulan < 1 || bulan > 12) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi_bulan(list, MAX_TRANSAKSI, bulan);
    
    unsigned long long total = 0;
    for (int i = 0; i < count; i++) {
        /* Hanya hitung pengeluaran untuk pos yang sesuai */
        if (list[i].jenis == JENIS_PENGELUARAN &&
            str_compare_nocase(list[i].pos, nama_pos) == 0) {
            total += list[i].nominal;
        }
    }
    
    return total;
}

long long hitung_sisa_pos(unsigned long long nominal, unsigned long long realisasi) {
    return (long long)nominal - (long long)realisasi;
}

int tentukan_status_pos(long long sisa) {
    return (sisa >= 0) ? STATUS_AMAN : STATUS_TIDAK_AMAN;
}

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

int update_semua_pos_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
    if (count == 0) return 1;  /* Tidak ada pos, sukses */
    
    /* Update kalkulasi untuk setiap pos */
    for (int i = 0; i < count; i++) {
        update_kalkulasi_pos(&list[i], bulan);
    }
    
    /* Simpan kembali */
    return save_pos_bulan(list, count, bulan);
}

int hitung_jumlah_transaksi_pos(const char *nama_pos, int bulan) {
    if (!nama_pos || bulan < 1 || bulan > 12) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi_bulan(list, MAX_TRANSAKSI, bulan);
    
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (str_compare_nocase(list[i].pos, nama_pos) == 0) {
            total++;
        }
    }
    
    return total;
}

unsigned long long hitung_total_anggaran(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
    unsigned long long total = 0;
    for (int i = 0; i < count; i++) {
        total += list[i].nominal;
    }
    
    return total;
}

unsigned long long hitung_total_realisasi(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
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

/* --- pos_crud.c --- */

static void set_error(char *error_msg, const char *msg) {
    if (error_msg && msg) {
        str_copy_safe(error_msg, msg, 100);
    }
}

int pos_tambah(const char *nama, unsigned long long nominal, int bulan, char *error_msg) {
    if (!nama || bulan < 1 || bulan > 12) {
        set_error(error_msg, "Parameter tidak valid");
        return 0;
    }
    
    /* Validasi nama */
    if (!validasi_tidak_kosong(nama)) {
        set_error(error_msg, "Nama pos tidak boleh kosong");
        return 0;
    }
    
    if (!validasi_panjang_pos(nama)) {
        set_error(error_msg, "Nama pos terlalu panjang (max 20 karakter)");
        return 0;
    }
    
    if (!validasi_tidak_ada_pipe(nama)) {
        set_error(error_msg, "Nama pos tidak boleh mengandung karakter |");
        return 0;
    }
    
    /* Cek nama unik */
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
    if (!validasi_pos_unik(nama, list, count)) {
        set_error(error_msg, "Nama pos sudah ada");
        return 0;
    }
    
    /* Validasi nominal */
    if (nominal == 0) {
        set_error(error_msg, "Nominal anggaran harus lebih dari 0");
        return 0;
    }
    
    /* Buat pos baru */
    PosAnggaran pos;
    memset(&pos, 0, sizeof(PosAnggaran));
    str_copy_safe(pos.nama, nama, sizeof(pos.nama));
    pos.nominal = nominal;
    
    if (!add_pos(&pos, bulan)) {
        set_error(error_msg, "Gagal menyimpan pos anggaran");
        return 0;
    }
    
    return 1;
}

int pos_edit(int no, const char *nama_baru, unsigned long long nominal_baru, int bulan, char *error_msg) {
    if (no <= 0 || bulan < 1 || bulan > 12) {
        set_error(error_msg, "Parameter tidak valid");
        return 0;
    }
    
    /* Load pos yang akan diedit */
    PosAnggaran pos;
    if (!find_pos_by_no(no, bulan, &pos)) {
        set_error(error_msg, "Pos tidak ditemukan");
        return 0;
    }
    
    /* Validasi nama baru jika diubah */
    if (nama_baru && strlen(nama_baru) > 0) {
        if (!validasi_panjang_pos(nama_baru)) {
            set_error(error_msg, "Nama pos terlalu panjang (max 20 karakter)");
            return 0;
        }
        
        if (!validasi_tidak_ada_pipe(nama_baru)) {
            set_error(error_msg, "Nama pos tidak boleh mengandung karakter |");
            return 0;
        }
        
        /* Cek nama unik (kecuali pos ini sendiri) */
        PosAnggaran list[MAX_POS];
        int count = load_pos_bulan(list, MAX_POS, bulan);
        
        if (!validasi_pos_unik_edit(nama_baru, no, list, count)) {
            set_error(error_msg, "Nama pos sudah digunakan");
            return 0;
        }
        
        str_copy_safe(pos.nama, nama_baru, sizeof(pos.nama));
    }
    
    /* Update nominal jika diubah */
    if (nominal_baru > 0) {
        pos.nominal = nominal_baru;
    }
    
    /* Update kalkulasi */
    update_kalkulasi_pos(&pos, bulan);
    
    if (!update_pos(no, &pos, bulan)) {
        set_error(error_msg, "Gagal menyimpan perubahan");
        return 0;
    }
    
    return 1;
}

int pos_hapus(int no, int bulan, char *error_msg) {
    if (no <= 0 || bulan < 1 || bulan > 12) {
        set_error(error_msg, "Parameter tidak valid");
        return 0;
    }
    
    /* Load pos yang akan dihapus */
    PosAnggaran pos;
    if (!find_pos_by_no(no, bulan, &pos)) {
        set_error(error_msg, "Pos tidak ditemukan");
        return 0;
    }
    
    /* Update kalkulasi untuk cek transaksi terkait */
    update_kalkulasi_pos(&pos, bulan);
    
    /* Cek apakah pos bisa dihapus */
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
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
        set_error(error_msg, "Pos tidak bisa dihapus karena masih ada transaksi terkait");
        return 0;
    }
    
    if (!delete_pos(no, bulan)) {
        set_error(error_msg, "Gagal menghapus pos");
        return 0;
    }
    
    return 1;
}

int pos_get_list(PosAnggaran *list, int max_count, int bulan) {
    if (!list || max_count <= 0 || bulan < 1 || bulan > 12) return 0;
    
    int count = load_pos_bulan(list, max_count, bulan);
    
    /* Update kalkulasi untuk setiap pos */
    for (int i = 0; i < count; i++) {
        update_kalkulasi_pos(&list[i], bulan);
    }
    
    return count;
}

int pos_get_by_no(int no, int bulan, PosAnggaran *result) {
    if (no <= 0 || bulan < 1 || bulan > 12 || !result) return 0;
    
    if (!find_pos_by_no(no, bulan, result)) return 0;
    
    /* Update kalkulasi */
    update_kalkulasi_pos(result, bulan);
    
    return 1;
}

int pos_get_names(char names[][MAX_POS_LENGTH + 1], int max_count, int bulan) {
    if (!names || max_count <= 0 || bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
    int result_count = (count < max_count) ? count : max_count;
    
    for (int i = 0; i < result_count; i++) {
        str_copy_safe(names[i], list[i].nama, MAX_POS_LENGTH + 1);
    }
    
    return result_count;
}

int pos_exists(const char *nama, int bulan) {
    if (!nama || bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran result;
    return find_pos_by_nama(nama, bulan, &result);
}

int pos_refresh(int bulan) {
    return update_semua_pos_bulan(bulan);
}

/* --- pos_display.c --- */

int tampilkan_daftar_pos(int bulan, int selected) {
    PosAnggaran list[MAX_POS];
    int count = pos_get_list(list, MAX_POS, bulan);
    
    /* Tampilkan header bulan */
    char title[64];
    snprintf(title, sizeof(title), "Pos Anggaran - %s", get_nama_bulan(bulan));
    
    tui_color_on(COLOR_PAIR_CYAN);
    tui_bold_on();
    tui_print(4, 2, title);
    tui_bold_off();
    tui_color_off(COLOR_PAIR_CYAN);
    
    return display_pos_table(list, count, 6, selected);
}

int tampilkan_detail_pos(PosAnggaran *pos, int start_y) {
    if (!pos) return start_y;
    
    int y = start_y;
    char buffer[64];
    
    tui_bold_on();
    tui_color_on(COLOR_PAIR_CYAN);
    tui_print(y++, 2, "Detail Pos Anggaran");
    tui_color_off(COLOR_PAIR_CYAN);
    tui_bold_off();
    
    tui_draw_hline(y++, 2, 50, '-');
    
    tui_printf(y++, 2, "Nomor       : %d", pos->no);
    tui_printf(y++, 2, "Nama        : %s", pos->nama);
    
    format_rupiah(pos->nominal, buffer);
    tui_printf(y++, 2, "Anggaran    : %s", buffer);
    
    format_rupiah(pos->realisasi, buffer);
    tui_printf(y++, 2, "Realisasi   : %s", buffer);
    
    format_saldo(pos->sisa, buffer);
    tui_print(y, 2, "Sisa        : ");
    if (pos->sisa < 0) {
        tui_color_on(COLOR_PAIR_RED);
    } else {
        tui_color_on(COLOR_PAIR_GREEN);
    }
    tui_print(y++, 16, buffer);
    tui_color_off(pos->sisa < 0 ? COLOR_PAIR_RED : COLOR_PAIR_GREEN);
    
    tui_printf(y++, 2, "Transaksi   : %d", pos->jumlah_transaksi);
    
    tui_print(y, 2, "Status      : ");
    if (pos->status == STATUS_AMAN) {
        tui_color_on(COLOR_PAIR_GREEN);
        tui_print(y++, 16, "AMAN");
    } else {
        tui_color_on(COLOR_PAIR_RED);
        tui_print(y++, 16, "TIDAK AMAN");
    }
    tui_attr_reset();
    
    return y + 1;
}

int tampilkan_ringkasan_pos(int bulan, int start_y) {
    int y = start_y;
    char buffer[64];
    
    unsigned long long total_anggaran = hitung_total_anggaran(bulan);
    unsigned long long total_realisasi = hitung_total_realisasi(bulan);
    long long total_sisa = (long long)total_anggaran - (long long)total_realisasi;
    
    tui_draw_hline(y++, 2, 60, '=');
    
    tui_bold_on();
    tui_print(y++, 2, "RINGKASAN:");
    tui_bold_off();
    
    format_rupiah(total_anggaran, buffer);
    tui_printf(y++, 2, "Total Anggaran  : %s", buffer);
    
    format_rupiah(total_realisasi, buffer);
    tui_printf(y++, 2, "Total Realisasi : %s", buffer);
    
    format_saldo(total_sisa, buffer);
    tui_print(y, 2, "Total Sisa      : ");
    if (total_sisa < 0) {
        tui_color_on(COLOR_PAIR_RED);
    } else {
        tui_color_on(COLOR_PAIR_GREEN);
    }
    tui_print(y++, 20, buffer);
    tui_attr_reset();
    
    return y + 1;
}

int tampilkan_form_tambah_pos(int bulan) {
    tui_clear();
    display_header("TAMBAH POS ANGGARAN");
    
    char nama[MAX_POS_LENGTH + 1];
    unsigned long long nominal;
    
    int y = 5;
    
    char bulan_info[64];
    snprintf(bulan_info, sizeof(bulan_info), "Bulan: %s", get_nama_bulan(bulan));
    tui_print(y++, 2, bulan_info);
    y++;
    
    /* Input nama */
    if (!input_string_required(y++, 2, "Nama Pos: ", nama, sizeof(nama))) {
        return 0;
    }
    
    /* Input nominal */
    if (!input_number_min(y++, 2, "Anggaran: Rp ", &nominal, 1)) {
        return 0;
    }
    
    /* Konfirmasi */
    y++;
    if (!input_konfirmasi(y, 2, "Simpan pos anggaran?")) {
        show_info("Dibatalkan");
        return 0;
    }
    
    /* Simpan */
    char error_msg[100];
    if (pos_tambah(nama, nominal, bulan, error_msg)) {
        show_success("Pos anggaran berhasil ditambahkan");
        return 1;
    } else {
        show_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_form_edit_pos(int no, int bulan) {
    PosAnggaran pos;
    if (!pos_get_by_no(no, bulan, &pos)) {
        show_error("Pos tidak ditemukan");
        return 0;
    }
    
    tui_clear();
    display_header("EDIT POS ANGGARAN");
    
    int y = 5;
    
    /* Tampilkan data saat ini */
    tui_print(y++, 2, "Data saat ini:");
    tui_printf(y++, 2, "  Nama    : %s", pos.nama);
    char nominal_str[30];
    format_rupiah(pos.nominal, nominal_str);
    tui_printf(y++, 2, "  Anggaran: %s", nominal_str);
    y++;
    
    tui_print(y++, 2, "Masukkan data baru (kosongkan jika tidak diubah):");
    y++;
    
    char nama_baru[MAX_POS_LENGTH + 1];
    unsigned long long nominal_baru = 0;
    
    /* Input nama baru */
    if (!input_string(y++, 2, "Nama baru: ", nama_baru, sizeof(nama_baru))) {
        return 0;
    }
    
    /* Input nominal baru */
    char nominal_input[32];
    if (!input_string(y++, 2, "Anggaran baru: Rp ", nominal_input, sizeof(nominal_input))) {
        return 0;
    }
    if (strlen(nominal_input) > 0) {
        parse_nominal(nominal_input, &nominal_baru);
    }
    
    /* Cek apakah ada perubahan */
    if (strlen(nama_baru) == 0 && nominal_baru == 0) {
        show_info("Tidak ada perubahan");
        return 0;
    }
    
    /* Konfirmasi */
    y++;
    if (!input_konfirmasi(y, 2, "Simpan perubahan?")) {
        show_info("Dibatalkan");
        return 0;
    }
    
    /* Simpan */
    char error_msg[100];
    const char *nama_param = strlen(nama_baru) > 0 ? nama_baru : NULL;
    
    if (pos_edit(no, nama_param, nominal_baru, bulan, error_msg)) {
        show_success("Pos anggaran berhasil diupdate");
        return 1;
    } else {
        show_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_konfirmasi_hapus_pos(int no, int bulan) {
    PosAnggaran pos;
    if (!pos_get_by_no(no, bulan, &pos)) {
        show_error("Pos tidak ditemukan");
        return 0;
    }
    
    /* Cek apakah bisa dihapus */
    if (pos.jumlah_transaksi > 0) {
        show_error("Pos tidak bisa dihapus karena masih ada %d transaksi", pos.jumlah_transaksi);
        return 0;
    }
    
    char msg[100];
    snprintf(msg, sizeof(msg), "Hapus pos '%s'?", pos.nama);
    
    if (!show_confirm(msg)) {
        show_info("Dibatalkan");
        return 0;
    }
    
    char error_msg[100];
    if (pos_hapus(no, bulan, error_msg)) {
        show_success("Pos anggaran berhasil dihapus");
        return 1;
    } else {
        show_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_pilih_pos(int bulan, char *result) {
    if (!result) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = pos_get_list(list, MAX_POS, bulan);
    
    if (count == 0) {
        show_warning("Belum ada pos anggaran untuk bulan ini");
        return 0;
    }
    
    Menu menu;
    menu_init(&menu, "Pilih Pos Anggaran");
    
    for (int i = 0; i < count; i++) {
        menu_add_item(&menu, list[i].nama, i + 1);
    }
    
    int pilihan = menu_navigate(&menu);
    
    if (pilihan == CANCEL || pilihan <= 0) {
        return 0;
    }
    
    str_copy_safe(result, list[pilihan - 1].nama, MAX_POS_LENGTH + 1);
    return 1;
}

/* --- pos_handler.c --- */

/* Menu actions (sama dengan pos_menu.c) */
#define ACT_VIEW    1
#define ACT_TAMBAH  2
#define ACT_EDIT    3
#define ACT_HAPUS   4
#define ACT_BULAN   5
#define ACT_KEMBALI 0

void run_pos_module(int bulan_awal) {
    int bulan = bulan_awal;

    /* Inisialisasi pos untuk bulan jika belum ada */
    init_pos_for_bulan(bulan);

    while (1) {
        /* Sinkronisasi kalkulasi */
        sync_pos_transaksi(bulan);

        int action = menu_pos_utama(bulan);

        switch (action) {
            case ACT_VIEW:
                handler_view_pos(bulan);
                break;

            case ACT_TAMBAH:
                handler_tambah_pos(bulan);
                break;

            case ACT_EDIT:
                handler_edit_pos(bulan);
                break;

            case ACT_HAPUS:
                handler_hapus_pos(bulan);
                break;

            /* REV-6: Removed ACT_BULAN case - Ganti Bulan moved to main menu only */

            case ACT_KEMBALI:
            case CANCEL:
                return;
        }
    }
}

int init_pos_for_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    /* Cek apakah sudah ada file */
    if (pos_file_exists(bulan)) {
        return 1;  /* Sudah ada */
    }
    
    /* Cek bulan sebelumnya */
    int bulan_sebelum = (bulan == 1) ? 12 : bulan - 1;
    
    if (pos_file_exists(bulan_sebelum)) {
        /* Tanyakan apakah mau copy dari bulan sebelumnya */
        char msg[100];
        snprintf(msg, sizeof(msg), 
                 "Copy pos dari %s ke %s?",
                 get_nama_bulan(bulan_sebelum),
                 get_nama_bulan(bulan));
        
        if (show_confirm(msg)) {
            if (copy_pos_from_bulan(bulan, bulan_sebelum)) {
                show_success("Pos berhasil dicopy");
                return 1;
            } else {
                show_error("Gagal copy pos");
            }
        }
    }
    
    /* Buat file pos kosong */
    return init_pos_bulan(bulan);
}

int sync_pos_transaksi(int bulan) {
    return update_semua_pos_bulan(bulan);
}

/* --- pos_menu.c --- */

/* Menu actions */
#define ACT_VIEW    1
#define ACT_TAMBAH  2
#define ACT_EDIT    3
#define ACT_HAPUS   4
#define ACT_BULAN   5
#define ACT_KEMBALI 0

int menu_pos_utama(int bulan) {
    char title[64];
    snprintf(title, sizeof(title), "Menu Pos Anggaran - %s", get_nama_bulan(bulan));

    Menu menu;
    menu_init(&menu, title);

    menu_add_item(&menu, "Lihat Daftar Pos", ACT_VIEW);
    menu_add_item(&menu, "Tambah Pos Baru", ACT_TAMBAH);
    menu_add_item(&menu, "Edit Pos", ACT_EDIT);
    menu_add_item(&menu, "Hapus Pos", ACT_HAPUS);
    /* REV-2 & REV-6: Removed "Ganti Bulan" and "Kembali" - use ESC to go back */

    return menu_navigate(&menu);
}

int menu_pilih_bulan(int bulan_saat_ini) {
    Menu menu;
    menu_init(&menu, "Pilih Bulan");
    
    for (int i = 1; i <= 12; i++) {
        char item[32];
        if (i == bulan_saat_ini) {
            snprintf(item, sizeof(item), "%s (aktif)", get_nama_bulan(i));
        } else {
            snprintf(item, sizeof(item), "%s", get_nama_bulan(i));
        }
        menu_add_item(&menu, item, i);
    }
    
    int pilihan = menu_navigate(&menu);
    
    if (pilihan == CANCEL) {
        return bulan_saat_ini;  /* Tetap di bulan sekarang */
    }
    
    return pilihan;
}

void handler_view_pos(int bulan) {
    int selected = 0;
    PosAnggaran list[MAX_POS];
    int count = pos_get_list(list, MAX_POS, bulan);
    
    while (1) {
        tui_clear();
        display_header("DAFTAR POS ANGGARAN");
        
        int next_y = tampilkan_daftar_pos(bulan, selected);
        next_y = tampilkan_ringkasan_pos(bulan, next_y);
        
        display_footer("UP/DOWN: Navigasi | ENTER: Detail | ESC: Kembali");
        tui_refresh();
        
        int ch = tui_getch();
        
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
                    tui_clear();
                    display_header("DETAIL POS ANGGARAN");
                    tampilkan_detail_pos(&list[selected], 5);
                    display_footer("Tekan sembarang tombol untuk kembali");
                    tui_refresh();
                    tui_getch();
                    
                    /* Reload list */
                    count = pos_get_list(list, MAX_POS, bulan);
                    if (selected >= count) selected = count - 1;
                    if (selected < 0) selected = 0;
                }
                break;
                
            case 27:  /* ESC */
                return;
        }
    }
}

void handler_tambah_pos(int bulan) {
    tampilkan_form_tambah_pos(bulan);
}

void handler_edit_pos(int bulan) {
    PosAnggaran list[MAX_POS];
    int count = pos_get_list(list, MAX_POS, bulan);
    
    if (count == 0) {
        show_warning("Belum ada pos anggaran");
        return;
    }
    
    /* Pilih pos untuk diedit */
    Menu menu;
    menu_init(&menu, "Pilih Pos untuk Diedit");
    
    for (int i = 0; i < count; i++) {
        char item[64];
        snprintf(item, sizeof(item), "%d. %s", list[i].no, list[i].nama);
        menu_add_item(&menu, item, list[i].no);
    }
    
    int pilihan = menu_navigate(&menu);
    
    if (pilihan == CANCEL || pilihan <= 0) {
        return;
    }
    
    tampilkan_form_edit_pos(pilihan, bulan);
}

void handler_hapus_pos(int bulan) {
    PosAnggaran list[MAX_POS];
    int count = pos_get_list(list, MAX_POS, bulan);
    
    if (count == 0) {
        show_warning("Belum ada pos anggaran");
        return;
    }
    
    /* Pilih pos untuk dihapus */
    Menu menu;
    menu_init(&menu, "Pilih Pos untuk Dihapus");
    
    for (int i = 0; i < count; i++) {
        char item[64];
        if (list[i].jumlah_transaksi > 0) {
            snprintf(item, sizeof(item), "%d. %s (ada %d transaksi)", 
                     list[i].no, list[i].nama, list[i].jumlah_transaksi);
        } else {
            snprintf(item, sizeof(item), "%d. %s", list[i].no, list[i].nama);
        }
        menu_add_item(&menu, item, list[i].no);
    }
    
    int pilihan = menu_navigate(&menu);
    
    if (pilihan == CANCEL || pilihan <= 0) {
        return;
    }
    
    tampilkan_konfirmasi_hapus_pos(pilihan, bulan);
}
