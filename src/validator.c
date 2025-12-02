#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include "validator.h"
#include "utils.h"
#include "transaksi.h"

/**
 * Validasi format tanggal dd-mm-YYYY
 */
int validasi_format_tanggal(const char *tanggal) {
    if (tanggal == NULL) return 0;

    int len = strlen(tanggal);

    /* Panjang harus tepat 10 karakter (dd-mm-YYYY) */
    if (len != 10) return 0;

    /* Posisi 2 dan 5 harus berisi '-' */
    if (tanggal[2] != '-' || tanggal[5] != '-') return 0;

    /* Posisi lainnya harus digit */
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;  /* Skip posisi '-' */
        if (!isdigit((unsigned char)tanggal[i])) {
            return 0;
        }
    }

    return 1;
}

/**
 * Validasi tanggal benar-benar valid (termasuk kabisat)
 */
int validasi_tanggal_sah(int d, int m, int y) {
    /* Validasi tahun */
    if (y < 1900 || y > 2100) return 0;

    /* Validasi bulan */
    if (m < 1 || m > 12) return 0;

    /* Validasi hari */
    if (d < 1) return 0;

    /* Dapatkan jumlah hari dalam bulan */
    int max_days = dapatkan_jumlah_hari_bulan(m, y);

    if (d > max_days) return 0;

    return 1;
}

/**
 * Validasi tanggal bukan 00-00-0000
 */
int validasi_bukan_nol(const char *tanggal) {
    if (tanggal == NULL) return 0;

    int d, m, y;
    if (!urai_tanggal(tanggal, &d, &m, &y)) {
        return 0;
    }

    /* Tanggal tidak boleh semua nol */
    if (d == 0 && m == 0 && y == 0) {
        return 0;
    }

    return 1;
}

/**
 * Validasi tanggal lengkap (format dan kevalidan)
 */
int validasi_tanggal_lengkap(const char *tanggal) {
    /* Cek format dulu */
    if (!validasi_format_tanggal(tanggal)) {
        return 0;
    }

    /* Parse komponen tanggal */
    int d, m, y;
    if (!urai_tanggal(tanggal, &d, &m, &y)) {
        return 0;
    }

    /* Cek bukan tanggal nol */
    if (!validasi_bukan_nol(tanggal)) {
        return 0;
    }

    /* Cek kevalidan tanggal */
    if (!validasi_tanggal_sah(d, m, y)) {
        return 0;
    }

    return 1;
}

/**
 * Validasi bulan dalam range 1-12
 */
int validasi_bulan(int bulan) {
    return (bulan >= BULAN_MIN && bulan <= BULAN_MAX);
}

/**
 * Validasi tahun (harus > 0 dan reasonable)
 */
int validasi_tahun(int tahun) {
    return (tahun >= 1900 && tahun <= 2100);
}

/**
 * REV-3: Validasi bulan pada tanggal sesuai dengan bulan aktif
 * @param tanggal String tanggal format dd-mm-YYYY
 * @param bulan_aktif Bulan yang sedang dipilih (1-12)
 * @return 1 jika bulan sesuai, 0 jika tidak sesuai
 */
int validasi_bulan_sesuai(const char *tanggal, int bulan_aktif) {
    if (tanggal == NULL || bulan_aktif < 1 || bulan_aktif > 12) return 0;

    int d, m, y;
    if (!urai_tanggal(tanggal, &d, &m, &y)) {
        return 0;
    }

    return (m == bulan_aktif);
}

/**
 * Validasi nominal harus positif (> 0)
 */
int validasi_nominal_positif(unsigned long long nominal) {
    return (nominal > 0);
}

/**
 * Validasi nomor pos dalam range yang valid
 */
int validasi_no_pos(int no, int max) {
    return (no >= 1 && no <= max);
}

/**
 * Parse string ke unsigned long long dengan validasi
 */
int urai_nominal(const char *str, unsigned long long *result) {
    if (str == NULL || result == NULL) return 0;

    /* Skip whitespace di awal */
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }

    /* String kosong tidak valid */
    if (*str == '\0') return 0;

    /* Cek apakah semua karakter adalah digit */
    const char *p = str;
    while (*p) {
        if (!isdigit((unsigned char)*p)) {
            return 0;  /* Karakter non-digit ditemukan */
        }
        p++;
    }

    /* Konversi dengan strtoull */
    errno = 0;
    char *endptr;
    unsigned long long value = strtoull(str, &endptr, 10);

    /* Cek error overflow */
    if (errno == ERANGE) {
        return 0;
    }

    /* Pastikan seluruh string terkonversi */
    if (*endptr != '\0') {
        return 0;
    }

    *result = value;
    return 1;
}

/**
 * Validasi pilihan menu dalam range
 */
int validasi_pilihan_menu(int pilihan, int min, int max) {
    return (pilihan >= min && pilihan <= max);
}

/**
 * Validasi jenis transaksi (0 atau 1)
 */
int validasi_jenis_transaksi(int jenis) {
    return (jenis == JENIS_PENGELUARAN || jenis == JENIS_PEMASUKAN);
}

/**
 * Helper: Membandingkan string case-insensitive
 */
static int str_equal_nocase(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return 0;

    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return 0;
        }
        s1++;
        s2++;
    }

    return (*s1 == '\0' && *s2 == '\0');
}

/**
 * Validasi nama pos unik (belum ada di daftar)
 */
int validasi_pos_unik(const char *nama, PosAnggaran *list, int count) {
    if (nama == NULL || list == NULL) return 1;  /* Jika tidak ada data, dianggap unik */

    for (int i = 0; i < count; i++) {
        if (str_equal_nocase(nama, list[i].nama)) {
            return 0;  /* Nama sudah ada */
        }
    }

    return 1;  /* Nama unik */
}

/**
 * Validasi pos dengan nomor tertentu ada dalam daftar
 */
int validasi_pos_ada(int no, PosAnggaran *list, int count) {
    if (list == NULL || count <= 0) return 0;

    for (int i = 0; i < count; i++) {
        if (list[i].no == no) {
            return 1;  /* Pos ditemukan */
        }
    }

    return 0;  /* Pos tidak ditemukan */
}

/**
 * Mendapatkan nama pos berdasarkan nomor
 */
int dapatkan_nama_pos_berdasarkan_no(int no, PosAnggaran *list, int count, char *result) {
    if (list == NULL || result == NULL || count <= 0) return 0;

    for (int i = 0; i < count; i++) {
        if (list[i].no == no) {
            salin_string_aman(result, list[i].nama, 21);
            return 1;  /* Berhasil */
        }
    }

    result[0] = '\0';
    return 0;  /* Tidak ditemukan */
}

/**
 * Mendapatkan index pos berdasarkan nama
 */
int dapatkan_index_pos_berdasarkan_nama(const char *nama, PosAnggaran *list, int count) {
    if (nama == NULL || list == NULL) return -1;

    for (int i = 0; i < count; i++) {
        if (str_equal_nocase(nama, list[i].nama)) {
            return i;  /* Index ditemukan */
        }
    }

    return -1;  /* Tidak ditemukan */
}

/**
 * Validasi nama pos untuk edit (unik kecuali nama yang sedang diedit)
 */
int validasi_pos_unik_edit(const char *nama, int no_edit, PosAnggaran *list, int count) {
    if (nama == NULL || list == NULL) return 1;

    for (int i = 0; i < count; i++) {
        /* Skip pos yang sedang diedit */
        if (list[i].no == no_edit) continue;

        if (str_equal_nocase(nama, list[i].nama)) {
            return 0;  /* Nama sudah ada di pos lain */
        }
    }

    return 1;  /* Nama valid untuk edit */
}

/**
 * Validasi pos bisa dihapus (tidak ada transaksi terkait)
 */
int validasi_pos_bisa_hapus(int no, PosAnggaran *pos_list, int pos_count) {
    if (pos_list == NULL || pos_count <= 0) return 0;

    for (int i = 0; i < pos_count; i++) {
        if (pos_list[i].no == no) {
            /* Pos bisa dihapus jika tidak ada transaksi */
            return (pos_list[i].jumlah_transaksi == 0);
        }
    }

    return 0;  /* Pos tidak ditemukan */
}

/**
 * Validasi panjang nama pos anggaran
 */
int validasi_panjang_pos(const char *nama) {
    if (nama == NULL) return 0;

    return (strlen(nama) <= MAX_POS_LENGTH);
}

/**
 * Validasi panjang deskripsi transaksi
 */
int validasi_panjang_deskripsi(const char *deskripsi) {
    if (deskripsi == NULL) return 0;

    return (strlen(deskripsi) <= MAX_DESKRIPSI_LENGTH);
}

/**
 * Validasi string tidak kosong
 */
int validasi_tidak_kosong(const char *str) {
    if (str == NULL) return 0;

    /* Skip whitespace dan cek apakah ada karakter lain */
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 1;  /* Ditemukan karakter non-whitespace */
        }
        str++;
    }

    return 0;  /* String kosong atau hanya whitespace */
}

/**
 * Validasi format ID transaksi (T0001-T9999)
 */
int validasi_format_id(const char *id) {
    if (id == NULL) return 0;

    int len = strlen(id);

    /* ID harus 5 karakter (T + 4 digit) */
    if (len != 5) return 0;

    /* Karakter pertama harus 'T' */
    if (id[0] != 'T') return 0;

    /* 4 karakter berikutnya harus digit */
    for (int i = 1; i < 5; i++) {
        if (!isdigit((unsigned char)id[i])) {
            return 0;
        }
    }

    return 1;
}

/**
 * Validasi string tidak mengandung karakter pipe (|)
 */
int validasi_tidak_ada_pipe(const char *str) {
    if (str == NULL) return 1;  /* NULL dianggap valid (tidak ada pipe) */

    while (*str) {
        if (*str == '|') {
            return 0;  /* Ditemukan pipe */
        }
        str++;
    }

    return 1;  /* Tidak ada pipe */
}

/**
 * Validasi string hanya mengandung karakter yang diizinkan
 * Karakter yang diizinkan: alfanumerik, spasi, dan simbol umum (.,'-/)
 */
int validasi_karakter_sah(const char *str) {
    if (str == NULL) return 0;

    while (*str) {
        char c = *str;

        /* Cek karakter yang diizinkan */
        if (!isalnum((unsigned char)c) &&
            c != ' ' && c != '.' && c != ',' &&
            c != '\'' && c != '-' && c != '/' &&
            c != '(' && c != ')') {
            return 0;  /* Karakter tidak valid */
        }
        str++;
    }

    return 1;
}
