/**
 * @file transaksi.c
 * @brief Implementasi modul transaksi
 * @author Kelompok B11
 * @date 2025
 */

#include <stdio.h>
#include <string.h>
#include "transaksi.h"
#include "file.h"
#include "pos.h"
#include "tui.h"
#include "utils.h"
#include "validator.h"

/* Menu actions */
#define ACT_VIEW    1
#define ACT_TAMBAH  2
#define ACT_EDIT    3
#define ACT_HAPUS   4
#define ACT_KEMBALI 0

/* --- transaksi_crud.c --- */

/**
 * Helper untuk set error message
 * I.S.: error_msg buffer, msg pesan
 * F.S.: error_msg diisi msg
 */
static void set_error(char *error_msg, const char *msg) {
    if (error_msg && msg) {
        str_copy_safe(error_msg, msg, 100);
    }
}

int transaksi_tambah(const char *tanggal, int jenis, const char *pos,
                     unsigned long long nominal, const char *deskripsi,
                     char *error_msg) {
    /* Validasi tanggal */
    if (!tanggal || !validasi_tanggal_lengkap(tanggal)) {
        set_error(error_msg, "Tanggal tidak valid");
        return 0;
    }
    
    /* Validasi jenis */
    if (!validasi_jenis_transaksi(jenis)) {
        set_error(error_msg, "Jenis transaksi tidak valid");
        return 0;
    }
    
    /* Validasi pos */
    if (!pos || !validasi_tidak_kosong(pos)) {
        set_error(error_msg, "Pos anggaran harus dipilih");
        return 0;
    }
    
    /* Validasi nominal */
    if (!validasi_nominal_positif(nominal)) {
        set_error(error_msg, "Nominal harus lebih dari 0");
        return 0;
    }
    
    /* Validasi deskripsi (opsional tapi tidak boleh ada pipe) */
    if (deskripsi && strlen(deskripsi) > 0) {
        if (!validasi_panjang_deskripsi(deskripsi)) {
            set_error(error_msg, "Deskripsi terlalu panjang (max 50 karakter)");
            return 0;
        }
        if (!validasi_tidak_ada_pipe(deskripsi)) {
            set_error(error_msg, "Deskripsi tidak boleh mengandung karakter |");
            return 0;
        }
    }
    
    /* Buat transaksi baru */
    Transaksi trx;
    memset(&trx, 0, sizeof(Transaksi));
    
    generate_transaksi_id(trx.id);
    str_copy_safe(trx.tanggal, tanggal, sizeof(trx.tanggal));
    trx.jenis = jenis;
    str_copy_safe(trx.pos, pos, sizeof(trx.pos));
    trx.nominal = nominal;
    if (deskripsi) {
        str_copy_safe(trx.deskripsi, deskripsi, sizeof(trx.deskripsi));
    }
    
    if (!add_transaksi(&trx)) {
        set_error(error_msg, "Gagal menyimpan transaksi");
        return 0;
    }
    
    /* Update kalkulasi pos */
    int bulan = ekstrak_bulan(tanggal);
    if (bulan > 0) {
        update_semua_pos_bulan(bulan);
    }
    
    return 1;
}

int transaksi_edit(const char *id, const char *tanggal, int jenis,
                   const char *pos, unsigned long long nominal,
                   const char *deskripsi, char *error_msg) {
    if (!id) {
        set_error(error_msg, "ID transaksi tidak valid");
        return 0;
    }
    
    /* Load transaksi yang akan diedit */
    Transaksi trx;
    if (!find_transaksi_by_id(id, &trx)) {
        set_error(error_msg, "Transaksi tidak ditemukan");
        return 0;
    }
    
    int old_bulan = ekstrak_bulan(trx.tanggal);
    
    /* Update tanggal jika diubah */
    if (tanggal && strlen(tanggal) > 0) {
        if (!validasi_tanggal_lengkap(tanggal)) {
            set_error(error_msg, "Tanggal tidak valid");
            return 0;
        }
        str_copy_safe(trx.tanggal, tanggal, sizeof(trx.tanggal));
    }
    
    /* Update jenis jika diubah */
    if (jenis >= 0) {
        if (!validasi_jenis_transaksi(jenis)) {
            set_error(error_msg, "Jenis transaksi tidak valid");
            return 0;
        }
        trx.jenis = jenis;
    }
    
    /* Update pos jika diubah */
    if (pos && strlen(pos) > 0) {
        str_copy_safe(trx.pos, pos, sizeof(trx.pos));
    }
    
    /* Update nominal jika diubah */
    if (nominal > 0) {
        trx.nominal = nominal;
    }
    
    /* Update deskripsi jika diubah */
    if (deskripsi) {
        if (strlen(deskripsi) > 0 && !validasi_panjang_deskripsi(deskripsi)) {
            set_error(error_msg, "Deskripsi terlalu panjang");
            return 0;
        }
        str_copy_safe(trx.deskripsi, deskripsi, sizeof(trx.deskripsi));
    }
    
    if (!update_transaksi(id, &trx)) {
        set_error(error_msg, "Gagal menyimpan perubahan");
        return 0;
    }
    
    /* Update kalkulasi pos untuk bulan lama dan baru */
    int new_bulan = ekstrak_bulan(trx.tanggal);
    if (old_bulan > 0) update_semua_pos_bulan(old_bulan);
    if (new_bulan > 0 && new_bulan != old_bulan) update_semua_pos_bulan(new_bulan);
    
    return 1;
}

int transaksi_hapus(const char *id, char *error_msg) {
    if (!id) {
        set_error(error_msg, "ID transaksi tidak valid");
        return 0;
    }
    
    /* Load transaksi untuk dapat bulan */
    Transaksi trx;
    if (!find_transaksi_by_id(id, &trx)) {
        set_error(error_msg, "Transaksi tidak ditemukan");
        return 0;
    }
    
    int bulan = ekstrak_bulan(trx.tanggal);
    
    if (!delete_transaksi(id)) {
        set_error(error_msg, "Gagal menghapus transaksi");
        return 0;
    }
    
    /* Update kalkulasi pos */
    if (bulan > 0) {
        update_semua_pos_bulan(bulan);
    }
    
    return 1;
}

int transaksi_get_list(Transaksi *list, int max_count, int bulan) {
    if (!list || max_count <= 0) return 0;
    
    if (bulan > 0 && bulan <= 12) {
        return load_transaksi_bulan(list, max_count, bulan);
    } else {
        return load_transaksi(list, max_count);
    }
}

int transaksi_get_by_id(const char *id, Transaksi *result) {
    return find_transaksi_by_id(id, result);
}

unsigned long long transaksi_total_pemasukan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi_bulan(list, MAX_TRANSAKSI, bulan);
    
    unsigned long long total = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].jenis == JENIS_PEMASUKAN) {
            total += list[i].nominal;
        }
    }
    
    return total;
}

unsigned long long transaksi_total_pengeluaran(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi_bulan(list, MAX_TRANSAKSI, bulan);
    
    unsigned long long total = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].jenis == JENIS_PENGELUARAN) {
            total += list[i].nominal;
        }
    }
    
    return total;
}

int transaksi_count(int bulan, int jenis) {
    if (bulan < 1 || bulan > 12) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi_bulan(list, MAX_TRANSAKSI, bulan);
    
    if (jenis < 0) return count;
    
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].jenis == jenis) {
            total++;
        }
    }
    
    return total;
}

/* --- transaksi_display.c --- */

int tampilkan_daftar_transaksi(int bulan, int selected) {
    Transaksi list[MAX_TRANSAKSI];
    int count = transaksi_get_list(list, MAX_TRANSAKSI, bulan);
    
    char title[64];
    snprintf(title, sizeof(title), "Transaksi - %s", get_nama_bulan(bulan));
    
    tui_color_on(COLOR_PAIR_CYAN);
    tui_bold_on();
    tui_print(4, 2, title);
    tui_bold_off();
    tui_color_off(COLOR_PAIR_CYAN);
    
    return display_transaksi_table(list, count, 6, selected);
}

int tampilkan_detail_transaksi(Transaksi *trx, int start_y) {
    if (!trx) return start_y;
    
    int y = start_y;
    char buffer[64];
    
    tui_bold_on();
    tui_color_on(COLOR_PAIR_CYAN);
    tui_print(y++, 2, "Detail Transaksi");
    tui_color_off(COLOR_PAIR_CYAN);
    tui_bold_off();
    
    tui_draw_hline(y++, 2, 50, '-');
    
    tui_printf(y++, 2, "ID          : %s", trx->id);
    tui_printf(y++, 2, "Tanggal     : %s", trx->tanggal);
    
    tui_print(y, 2, "Jenis       : ");
    if (trx->jenis == JENIS_PENGELUARAN) {
        tui_color_on(COLOR_PAIR_RED);
        tui_print(y++, 16, "Pengeluaran");
    } else {
        tui_color_on(COLOR_PAIR_GREEN);
        tui_print(y++, 16, "Pemasukan");
    }
    tui_attr_reset();
    
    tui_printf(y++, 2, "Pos         : %s", trx->pos);
    
    format_rupiah(trx->nominal, buffer);
    tui_printf(y++, 2, "Nominal     : %s", buffer);
    
    tui_printf(y++, 2, "Deskripsi   : %s", 
               strlen(trx->deskripsi) > 0 ? trx->deskripsi : "-");
    
    return y + 1;
}

int tampilkan_ringkasan_transaksi(int bulan, int start_y) {
    int y = start_y;
    char buffer[64];
    
    unsigned long long pemasukan = transaksi_total_pemasukan(bulan);
    unsigned long long pengeluaran = transaksi_total_pengeluaran(bulan);
    long long saldo = (long long)pemasukan - (long long)pengeluaran;
    int jml_masuk = transaksi_count(bulan, JENIS_PEMASUKAN);
    int jml_keluar = transaksi_count(bulan, JENIS_PENGELUARAN);
    
    tui_draw_hline(y++, 2, 60, '=');
    
    tui_bold_on();
    tui_print(y++, 2, "RINGKASAN:");
    tui_bold_off();
    
    format_rupiah(pemasukan, buffer);
    tui_printf(y++, 2, "Total Pemasukan   : %s (%d transaksi)", buffer, jml_masuk);
    
    format_rupiah(pengeluaran, buffer);
    tui_printf(y++, 2, "Total Pengeluaran : %s (%d transaksi)", buffer, jml_keluar);
    
    format_saldo(saldo, buffer);
    tui_print(y, 2, "Saldo             : ");
    if (saldo < 0) {
        tui_color_on(COLOR_PAIR_RED);
    } else {
        tui_color_on(COLOR_PAIR_GREEN);
    }
    tui_print(y++, 22, buffer);
    tui_attr_reset();
    
    return y + 1;
}

int tampilkan_form_tambah_transaksi(int bulan) {
    tui_clear();
    display_header("TAMBAH TRANSAKSI");

    int y = 5;
    char tanggal[12], pos[MAX_POS_LENGTH + 1], deskripsi[MAX_DESKRIPSI_LENGTH + 1];
    unsigned long long nominal;
    int jenis;

    /* Info bulan */
    char bulan_info[64];
    snprintf(bulan_info, sizeof(bulan_info), "Bulan: %s", get_nama_bulan(bulan));
    tui_print(y++, 2, bulan_info);
    y++;

    /* Input tanggal dengan validasi bulan - REV-3 */
    while (1) {
        if (!input_tanggal(y, 2, "Tanggal", tanggal)) {
            return 0;
        }
        /* REV-3: Validasi bulan sesuai */
        if (!validasi_bulan_sesuai(tanggal, bulan)) {
            char error_msg[128];
            snprintf(error_msg, sizeof(error_msg),
                     "Anda memasukkan %s yang berbeda dari bulan terpilih! Input tidak valid!",
                     tanggal);
            show_error(error_msg);
            /* Redraw form */
            tui_clear();
            display_header("TAMBAH TRANSAKSI");
            y = 5;
            tui_print(y++, 2, bulan_info);
            y++;
            continue;
        }
        y++;
        break;
    }

    /* Input jenis */
    if (!input_jenis_transaksi(y++, 2, &jenis)) {
        return 0;
    }
    y++;
    
    /* Pilih pos (otomatis untuk Pemasukan) */
    if (jenis == JENIS_PENGELUARAN) {
        tui_print(y++, 2, "Pilih Pos Anggaran:");
        if (!tampilkan_pilih_pos(bulan, pos)) {
            show_warning("Pos anggaran harus dipilih");
            return 0;
        }
    } else {
        /* Untuk pemasukan, pos otomatis "Pemasukan" */
        strcpy(pos, "Pemasukan");
    }
    
    /* Kembali ke form */
    tui_clear();
    display_header("TAMBAH TRANSAKSI");
    y = 5;
    tui_printf(y++, 2, "Tanggal : %s", tanggal);
    tui_printf(y++, 2, "Jenis   : %s", get_label_jenis(jenis));
    if (jenis == JENIS_PENGELUARAN) {
        tui_printf(y++, 2, "Pos     : %s", pos);
    } else {
        tui_printf(y++, 2, "Pos     : %s (Otomatis)", pos);
    }
    y++;
    
    /* Input nominal */
    if (!input_number_min(y++, 2, "Nominal: Rp ", &nominal, 1)) {
        return 0;
    }
    
    /* Input deskripsi (opsional) */
    if (!input_string(y++, 2, "Deskripsi (opsional): ", deskripsi, sizeof(deskripsi))) {
        return 0;
    }
    
    /* Konfirmasi */
    y++;
    if (!input_konfirmasi(y, 2, "Simpan transaksi?")) {
        show_info("Dibatalkan");
        return 0;
    }
    
    /* Simpan */
    char error_msg[100];
    if (transaksi_tambah(tanggal, jenis, pos, nominal, deskripsi, error_msg)) {
        show_success("Transaksi berhasil ditambahkan");
        return 1;
    } else {
        show_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_form_edit_transaksi(const char *id) {
    Transaksi trx;
    if (!transaksi_get_by_id(id, &trx)) {
        show_error("Transaksi tidak ditemukan");
        return 0;
    }

    /* Get bulan from existing transaction for validation */
    int bulan_trx = ekstrak_bulan(trx.tanggal);

    char tanggal_baru[12], deskripsi_baru[MAX_DESKRIPSI_LENGTH + 1], nominal_input[32];
    unsigned long long nominal_baru = 0;

    while (1) {
        tui_clear();
        display_header("EDIT TRANSAKSI");

        int y = 5;
        char nominal_str[64];
        format_rupiah(trx.nominal, nominal_str);

        /* Tampilkan data saat ini */
        tui_print(y++, 2, "Data saat ini:");
        tui_printf(y++, 2, "  ID        : %s", trx.id);
        tui_printf(y++, 2, "  Tanggal   : %s", trx.tanggal);
        tui_printf(y++, 2, "  Jenis     : %s", get_label_jenis(trx.jenis));
        tui_printf(y++, 2, "  Pos       : %s", trx.pos);
        tui_printf(y++, 2, "  Nominal   : %s", nominal_str);
        tui_printf(y++, 2, "  Deskripsi : %s", trx.deskripsi);
        y++;

        tui_print(y++, 2, "Masukkan data baru (kosongkan jika tidak diubah):");
        y++;

        /* REV-7: Input tanggal baru dengan validasi langsung */
        if (!input_string(y++, 2, "Tanggal baru (dd-mm-YYYY): ", tanggal_baru, sizeof(tanggal_baru))) {
            return 0;
        }

        /* REV-7: Validasi langsung setelah input tanggal */
        if (strlen(tanggal_baru) > 0) {
            if (!validasi_tanggal_lengkap(tanggal_baru)) {
                show_error("Format tanggal tidak valid! Gunakan format dd-mm-YYYY");
                continue;
            }
            /* REV-3: Validasi bulan sesuai dengan transaksi asli */
            if (!validasi_bulan_sesuai(tanggal_baru, bulan_trx)) {
                char error_msg[128];
                snprintf(error_msg, sizeof(error_msg),
                         "Anda memasukkan %s yang berbeda dari bulan terpilih! Input tidak valid!",
                         tanggal_baru);
                show_error(error_msg);
                continue;
            }
        }

        /* REV-7: Input nominal baru dengan validasi langsung */
        if (!input_string(y++, 2, "Nominal baru: Rp ", nominal_input, sizeof(nominal_input))) {
            return 0;
        }
        if (strlen(nominal_input) > 0) {
            if (!parse_nominal(nominal_input, &nominal_baru) || nominal_baru == 0) {
                show_error("Nominal tidak valid! Harus berupa angka lebih dari 0");
                continue;
            }
        }

        /* REV-7: Input deskripsi baru dengan validasi langsung */
        if (!input_string(y++, 2, "Deskripsi baru: ", deskripsi_baru, sizeof(deskripsi_baru))) {
            return 0;
        }
        if (strlen(deskripsi_baru) > MAX_DESKRIPSI_LENGTH) {
            show_error("Deskripsi terlalu panjang (max 50 karakter)");
            continue;
        }

        break; /* All validations passed */
    }

    /* Cek perubahan */
    if (strlen(tanggal_baru) == 0 && nominal_baru == 0 && strlen(deskripsi_baru) == 0) {
        show_info("Tidak ada perubahan");
        return 0;
    }

    /* Konfirmasi */
    if (!show_confirm("Simpan perubahan?")) {
        show_info("Dibatalkan");
        return 0;
    }

    /* Simpan */
    char error_msg[100];
    const char *tgl_param = strlen(tanggal_baru) > 0 ? tanggal_baru : NULL;
    const char *desk_param = strlen(deskripsi_baru) > 0 ? deskripsi_baru : NULL;

    if (transaksi_edit(id, tgl_param, -1, NULL, nominal_baru, desk_param, error_msg)) {
        show_success("Transaksi berhasil diupdate");
        return 1;
    } else {
        show_error("%s", error_msg);
        return 0;
    }
}

int tampilkan_konfirmasi_hapus_transaksi(const char *id) {
    Transaksi trx;
    if (!transaksi_get_by_id(id, &trx)) {
        show_error("Transaksi tidak ditemukan");
        return 0;
    }
    
    char nominal_str[30];
    format_rupiah(trx.nominal, nominal_str);
    
    char msg[128];
    snprintf(msg, sizeof(msg), "Hapus transaksi %s (%s %s)?", 
             trx.id, get_label_jenis(trx.jenis), nominal_str);
    
    if (!show_confirm(msg)) {
        show_info("Dibatalkan");
        return 0;
    }
    
    char error_msg[100];
    if (transaksi_hapus(id, error_msg)) {
        show_success("Transaksi berhasil dihapus");
        return 1;
    } else {
        show_error("%s", error_msg);
        return 0;
    }
}

/* --- transaksi_handler.c --- */

/**
 * Jalankan modul transaksi
 * I.S.: bulan_awal valid
 * F.S.: modul transaksi dijalankan hingga user kembali
 */
void run_transaksi_module(int bulan_awal) {
    int bulan = bulan_awal;
    
    /* Pastikan pos anggaran ada untuk bulan ini */
    init_pos_for_bulan(bulan);
    
    while (1) {
        int action = menu_transaksi_utama(bulan);
        
        switch (action) {
            case ACT_VIEW:
                handler_view_transaksi(bulan);
                break;
                
            case ACT_TAMBAH:
                handler_tambah_transaksi(bulan);
                break;
                
            case ACT_EDIT:
                handler_edit_transaksi(bulan);
                break;
                
            case ACT_HAPUS:
                handler_hapus_transaksi(bulan);
                break;
                
            case ACT_KEMBALI:
            case CANCEL:
                return;
        }
    }
}

/* --- transaksi_menu.c --- */

int menu_transaksi_utama(int bulan) {
    char title[64];
    snprintf(title, sizeof(title), "Menu Transaksi - %s", get_nama_bulan(bulan));

    Menu menu;
    menu_init(&menu, title);

    menu_add_item(&menu, "Lihat Daftar Transaksi", ACT_VIEW);
    menu_add_item(&menu, "Tambah Transaksi", ACT_TAMBAH);
    menu_add_item(&menu, "Edit Transaksi", ACT_EDIT);
    menu_add_item(&menu, "Hapus Transaksi", ACT_HAPUS);
    /* REV-2: Removed "Kembali" - use ESC to go back */

    return menu_navigate(&menu);
}

/**
 * Handler untuk view daftar transaksi
 * I.S.: bulan valid
 * F.S.: user kembali dari view
 */
void handler_view_transaksi(int bulan) {
    int selected = 0;
    Transaksi list[MAX_TRANSAKSI];
    int count = transaksi_get_list(list, MAX_TRANSAKSI, bulan);
    
    while (1) {
        tui_clear();
        display_header("DAFTAR TRANSAKSI");
        
        int next_y = tampilkan_daftar_transaksi(bulan, selected);
        next_y = tampilkan_ringkasan_transaksi(bulan, next_y);
        
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
                    display_header("DETAIL TRANSAKSI");
                    tampilkan_detail_transaksi(&list[selected], 5);
                    display_footer("Tekan sembarang tombol untuk kembali");
                    tui_refresh();
                    tui_getch();
                    
                    count = transaksi_get_list(list, MAX_TRANSAKSI, bulan);
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
 * I.S.: bulan valid
 * F.S.: transaksi baru ditambahkan jika user konfirmasi
 */
void handler_tambah_transaksi(int bulan) {
    tampilkan_form_tambah_transaksi(bulan);
}

/**
 * Handler untuk edit transaksi
 * I.S.: bulan valid
 * F.S.: transaksi diedit jika user konfirmasi
 */
void handler_edit_transaksi(int bulan) {
    Transaksi list[MAX_TRANSAKSI];
    int count = transaksi_get_list(list, MAX_TRANSAKSI, bulan);
    
    if (count == 0) {
        show_warning("Belum ada transaksi");
        return;
    }
    
    Menu menu;
    menu_init(&menu, "Pilih Transaksi untuk Diedit");
    
    char nominal_str[30];
    for (int i = 0; i < count && i < 15; i++) {
        char item[80];
        format_rupiah(list[i].nominal, nominal_str);
        snprintf(item, sizeof(item), "%s | %s | %s | %s",
                 list[i].id, list[i].tanggal, 
                 list[i].jenis == JENIS_PENGELUARAN ? "Out" : "In",
                 nominal_str);
        menu_add_item(&menu, item, i);
    }
    
    int pilihan = menu_navigate(&menu);
    
    if (pilihan == CANCEL || pilihan < 0) {
        return;
    }
    
    tampilkan_form_edit_transaksi(list[pilihan].id);
}

/**
 * Handler untuk hapus transaksi
 * I.S.: bulan valid
 * F.S.: transaksi dihapus jika user konfirmasi
 */
void handler_hapus_transaksi(int bulan) {
    Transaksi list[MAX_TRANSAKSI];
    int count = transaksi_get_list(list, MAX_TRANSAKSI, bulan);
    
    if (count == 0) {
        show_warning("Belum ada transaksi");
        return;
    }
    
    Menu menu;
    menu_init(&menu, "Pilih Transaksi untuk Dihapus");
    
    char nominal_str[30];
    for (int i = 0; i < count && i < 15; i++) {
        char item[80];
        format_rupiah(list[i].nominal, nominal_str);
        snprintf(item, sizeof(item), "%s | %s | %s | %s",
                 list[i].id, list[i].tanggal,
                 list[i].jenis == JENIS_PENGELUARAN ? "Out" : "In",
                 nominal_str);
        menu_add_item(&menu, item, i);
    }
    
    int pilihan = menu_navigate(&menu);
    
    if (pilihan == CANCEL || pilihan < 0) {
        return;
    }
    
    tampilkan_konfirmasi_hapus_transaksi(list[pilihan].id);
}
