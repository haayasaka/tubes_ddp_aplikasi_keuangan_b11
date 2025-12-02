#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "file.h"
#include "utils.h"

/* ===== FUNGSI HELPER INTERNAL ===== */

/**
 * Helper: Konversi string ke int yang aman (menangani NULL)
 */
static int aman_atoi(const char *str) {
    if (str == NULL) return 0;
    return atoi(str);
}

/**
 * Helper: Konversi string ke unsigned long long yang aman
 */
static unsigned long long aman_atoull(const char *str) {
    if (str == NULL) return 0;
    return strtoull(str, NULL, 10);
}

/**
 * Helper: Konversi string ke double yang aman
 */
static double aman_atof(const char *str) {
    if (str == NULL) return 0.0;
    return atof(str);
}

/**
 * Helper: Konversi string ke long long yang aman
 */
static long long aman_atoll(const char *str) {
    if (str == NULL) return 0;
    return strtoll(str, NULL, 10);
}

/* ===== IMPLEMENTASI FILE CORE (Operasi Dasar) ===== */

/**
 * Cek apakah file ada
 */
int cek_file_ada(const char *filename) {
    if (filename == NULL) return 0;

    FILE *f = fopen(filename, "r");
    if (f != NULL) {
        fclose(f);
        return 1;
    }
    return 0;
}

/**
 * Buat file baru (kosong)
 */
int buat_file(const char *filename) {
    if (filename == NULL) return 0;

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return 0;
    }
    fclose(f);
    return 1;
}

/**
 * Buka file untuk dibaca
 */
FILE* buka_file_baca(const char *filename) {
    if (filename == NULL) return NULL;

    return fopen(filename, "r");
}

/**
 * Buka file untuk ditulis (overwrite)
 */
FILE* buka_file_tulis(const char *filename) {
    if (filename == NULL) return NULL;

    return fopen(filename, "w");
}

/**
 * Buka file untuk append
 */
FILE* buka_file_tambah(const char *filename) {
    if (filename == NULL) return NULL;

    return fopen(filename, "a");
}

/**
 * Membaca seluruh isi file ke buffer
 */
int baca_semua_file(const char *filename, char *buffer, int buffer_size) {
    if (filename == NULL || buffer == NULL || buffer_size <= 0) {
        return 0;
    }

    FILE *f = buka_file_baca(filename);
    if (f == NULL) {
        buffer[0] = '\0';
        return 0;
    }

    int total = fread(buffer, 1, buffer_size - 1, f);
    buffer[total] = '\0';

    fclose(f);
    return total;
}

/**
 * Menulis string ke file (overwrite)
 */
int tulis_semua_file(const char *filename, const char *content) {
    if (filename == NULL) return 0;

    FILE *f = buka_file_tulis(filename);
    if (f == NULL) {
        return 0;
    }

    if (content != NULL) {
        fputs(content, f);
    }

    fclose(f);
    return 1;
}

/**
 * Menambahkan baris ke akhir file
 */
int tambah_baris_file(const char *filename, const char *line) {
    if (filename == NULL || line == NULL) return 0;

    /* Jika file belum ada, buat dulu */
    if (!cek_file_ada(filename)) {
        if (!buat_file(filename)) {
            return 0;
        }
    }

    FILE *f = buka_file_tambah(filename);
    if (f == NULL) {
        return 0;
    }

    fprintf(f, "%s\n", line);
    fclose(f);

    return 1;
}

/**
 * Memastikan direktori data ada
 */
int pastikan_direktori_data(void) {
    struct stat st = {0};

    /* Cek apakah direktori sudah ada */
    if (stat(DATA_DIR, &st) == -1) {
        /* Buat direktori dengan permission 0755 */
        #ifdef _WIN32
        if (mkdir(DATA_DIR) == -1) {
        #else
        if (mkdir(DATA_DIR, 0755) == -1) {
        #endif
            return 0;  /* Gagal membuat direktori */
        }
    }

    return 1;
}

/**
 * Generate nama file dengan suffix bulan
 */
void dapatkan_nama_file_bulan(const char *base, int bulan, char *result, int result_size) {
    if (base == NULL || result == NULL || result_size <= 0) return;

    /* Validasi bulan */
    if (bulan < BULAN_MIN || bulan > BULAN_MAX) {
        bulan = 1;  /* Default ke Januari jika invalid */
    }

    /* Format: data/base_MM.txt */
    snprintf(result, result_size, "%s%s_%s%s", DATA_DIR, base, dapatkan_kode_bulan(bulan), FILE_EXTENSION);
}

/**
 * Generate nama file transaksi
 */
void dapatkan_nama_file_transaksi(char *result, int result_size) {
    if (result == NULL || result_size <= 0) return;

    snprintf(result, result_size, "%s%s%s", DATA_DIR, FILE_TRANSAKSI, FILE_EXTENSION);
}

/* ===== IMPLEMENTASI FILE PARSER (Parsing & Formatting) ===== */

/**
 * Mendapatkan label jenis transaksi
 */
const char* dapatkan_label_jenis(int jenis) {
    if (jenis == JENIS_PEMASUKAN) {
        return "Pemasukan";
    }
    return "Pengeluaran";
}

/**
 * Konversi string jenis ke kode
 */
int urai_jenis_transaksi(const char *jenis_str) {
    if (jenis_str == NULL) return -1;

    /* Case-insensitive comparison */
    if (banding_string_tanpa_case(jenis_str, "Pemasukan") == 0) {
        return JENIS_PEMASUKAN;
    }
    if (banding_string_tanpa_case(jenis_str, "Pengeluaran") == 0) {
        return JENIS_PENGELUARAN;
    }

    return -1;  /* Invalid */
}

/**
 * Parse baris teks ke struct Transaksi
 * Format: id|tanggal|jenis|pos|nominal|deskripsi
 */
int urai_baris_transaksi(const char *line, Transaksi *trx) {
    if (line == NULL || trx == NULL) return 0;

    /* Buat salinan line karena strtok memodifikasi string */
    char buffer[MAX_LINE_LENGTH];
    salin_string_aman(buffer, line, MAX_LINE_LENGTH);
    hapus_newline_string(buffer);

    /* Parse field dengan delimiter | */
    char *token;
    int field = 0;

    token = strtok(buffer, "|");
    while (token != NULL && field < 6) {
        pangkas_string(token);

        switch (field) {
            case 0:  /* id */
                salin_string_aman(trx->id, token, sizeof(trx->id));
                break;
            case 1:  /* tanggal */
                salin_string_aman(trx->tanggal, token, sizeof(trx->tanggal));
                break;
            case 2:  /* jenis */
                trx->jenis = urai_jenis_transaksi(token);
                break;
            case 3:  /* pos */
                salin_string_aman(trx->pos, token, sizeof(trx->pos));
                break;
            case 4:  /* nominal */
                trx->nominal = aman_atoull(token);
                break;
            case 5:  /* deskripsi */
                salin_string_aman(trx->deskripsi, token, sizeof(trx->deskripsi));
                break;
        }

        field++;
        token = strtok(NULL, "|");
    }

    /* Minimal harus ada 5 field (deskripsi bisa kosong) */
    return (field >= 5);
}

/**
 * Parse baris teks ke struct PosAnggaran
 * Format: no|nama|nominal|realisasi|sisa|jumlah_trx|status
 */
int urai_baris_pos(const char *line, PosAnggaran *pos) {
    if (line == NULL || pos == NULL) return 0;

    char buffer[MAX_LINE_LENGTH];
    salin_string_aman(buffer, line, MAX_LINE_LENGTH);
    hapus_newline_string(buffer);

    char *token;
    int field = 0;

    token = strtok(buffer, "|");
    while (token != NULL && field < 7) {
        pangkas_string(token);

        switch (field) {
            case 0:  /* no */
                pos->no = aman_atoi(token);
                break;
            case 1:  /* nama */
                salin_string_aman(pos->nama, token, sizeof(pos->nama));
                break;
            case 2:  /* nominal */
                pos->nominal = aman_atoull(token);
                break;
            case 3:  /* realisasi */
                pos->realisasi = aman_atoull(token);
                break;
            case 4:  /* sisa */
                pos->sisa = aman_atoll(token);
                break;
            case 5:  /* jumlah_transaksi */
                pos->jumlah_transaksi = aman_atoi(token);
                break;
            case 6:  /* status */
                pos->status = aman_atoi(token);
                break;
        }

        field++;
        token = strtok(NULL, "|");
    }

    return (field >= 7);
}

/**
 * Parse baris teks ke struct AnalisisKeuangan
 * Format: pemasukan|pengeluaran|rata|persen|saldo|kondisi|kesimpulan|trx_out|trx_in
 */
int urai_baris_analisis(const char *line, AnalisisKeuangan *data) {
    if (line == NULL || data == NULL) return 0;

    char buffer[MAX_LINE_LENGTH];
    salin_string_aman(buffer, line, MAX_LINE_LENGTH);
    hapus_newline_string(buffer);

    char *token;
    int field = 0;

    token = strtok(buffer, "|");
    while (token != NULL && field < 9) {
        pangkas_string(token);

        switch (field) {
            case 0:  /* total_pemasukan */
                data->total_pemasukan = aman_atoull(token);
                break;
            case 1:  /* total_pengeluaran */
                data->total_pengeluaran = aman_atoull(token);
                break;
            case 2:  /* rata_rata_pengeluaran */
                data->rata_rata_pengeluaran = aman_atof(token);
                break;
            case 3:  /* persentase_sisa */
                data->persentase_sisa = aman_atof(token);
                break;
            case 4:  /* saldo_akhir */
                data->saldo_akhir = aman_atoll(token);
                break;
            case 5:  /* kondisi_keuangan */
                data->kondisi_keuangan = aman_atoi(token);
                break;
            case 6:  /* kesimpulan */
                data->kesimpulan = aman_atoi(token);
                break;
            case 7:  /* total_trx_pengeluaran */
                data->total_trx_pengeluaran = aman_atoi(token);
                break;
            case 8:  /* total_trx_pemasukan */
                data->total_trx_pemasukan = aman_atoi(token);
                break;
        }

        field++;
        token = strtok(NULL, "|");
    }

    return (field >= 9);
}

/**
 * Format struct Transaksi ke string pipe-delimited
 */
void format_transaksi_ke_string(const Transaksi *trx, char *result, int result_size) {
    if (trx == NULL || result == NULL || result_size <= 0) return;

    snprintf(result, result_size, "%s|%s|%s|%s|%llu|%s",
            trx->id,
            trx->tanggal,
            dapatkan_label_jenis(trx->jenis),
            trx->pos,
            trx->nominal,
            trx->deskripsi);
}

/**
 * Format struct PosAnggaran ke string pipe-delimited
 */
void format_pos_ke_string(const PosAnggaran *pos, char *result, int result_size) {
    if (pos == NULL || result == NULL || result_size <= 0) return;

    snprintf(result, result_size, "%d|%s|%llu|%llu|%lld|%d|%d",
            pos->no,
            pos->nama,
            pos->nominal,
            pos->realisasi,
            pos->sisa,
            pos->jumlah_transaksi,
            pos->status);
}

/**
 * Format struct AnalisisKeuangan ke string pipe-delimited
 */
void format_analisis_ke_string(const AnalisisKeuangan *data, char *result, int result_size) {
    if (data == NULL || result == NULL || result_size <= 0) return;

    snprintf(result, result_size, "%llu|%llu|%.2f|%.2f|%lld|%d|%d|%d|%d",
            data->total_pemasukan,
            data->total_pengeluaran,
            data->rata_rata_pengeluaran,
            data->persentase_sisa,
            data->saldo_akhir,
            data->kondisi_keuangan,
            data->kesimpulan,
            data->total_trx_pengeluaran,
            data->total_trx_pemasukan);
}

/* ===== IMPLEMENTASI FILE ANALISIS ===== */

int muat_analisis_bulan(int bulan, AnalisisKeuangan *result) {
    if (!result || bulan < 1 || bulan > 12) return 0;

    pastikan_direktori_data();
    char filename[MAX_PATH];
    dapatkan_nama_file_bulan(FILE_ANALISIS_PREFIX, bulan, filename, sizeof(filename));

    FILE *fp = buka_file_baca(filename);
    if (!fp) return 0;

    char line[512];
    int success = 0;

    if (fgets(line, sizeof(line), fp)) {
        hapus_newline_string(line);
        if (strlen(line) > 0) {
            success = urai_baris_analisis(line, result);
        }
    }

    fclose(fp);
    return success;
}

int simpan_analisis_bulan(int bulan, AnalisisKeuangan *analisis) {
    if (!analisis || bulan < 1 || bulan > 12) return 0;

    pastikan_direktori_data();
    char filename[MAX_PATH];
    dapatkan_nama_file_bulan(FILE_ANALISIS_PREFIX, bulan, filename, sizeof(filename));

    FILE *fp = buka_file_tulis(filename);
    if (!fp) return 0;

    char line[512];
    format_analisis_ke_string(analisis, line, sizeof(line));
    fprintf(fp, "%s\n", line);

    fclose(fp);
    return 1;
}

int cek_file_analisis_ada(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    char filename[MAX_PATH];
    dapatkan_nama_file_bulan(FILE_ANALISIS_PREFIX, bulan, filename, sizeof(filename));

    return cek_file_ada(filename);
}

int inisialisasi_analisis_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    /* Cek apakah sudah ada */
    if (cek_file_analisis_ada(bulan)) return 1;

    /* Buat analisis kosong */
    AnalisisKeuangan analisis;
    memset(&analisis, 0, sizeof(AnalisisKeuangan));
    analisis.kondisi_keuangan = KONDISI_SEIMBANG;
    analisis.kesimpulan = KESIMPULAN_SEIMBANG;

    return simpan_analisis_bulan(bulan, &analisis);
}

/* ===== IMPLEMENTASI FILE POS ANGGARAN ===== */

int muat_pos_bulan(PosAnggaran *list, int max_count, int bulan) {
    if (!list || max_count <= 0 || bulan < 1 || bulan > 12) return 0;

    pastikan_direktori_data();
    char filename[MAX_PATH];
    dapatkan_nama_file_bulan(FILE_POS_PREFIX, bulan, filename, sizeof(filename));

    FILE *fp = buka_file_baca(filename);
    if (!fp) return 0;

    char line[512];
    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < max_count) {
        hapus_newline_string(line);
        if (strlen(line) == 0) continue;

        if (urai_baris_pos(line, &list[count])) {
            count++;
        }
    }

    fclose(fp);
    return count;
}

int simpan_pos_bulan(PosAnggaran *list, int count, int bulan) {
    if (!list && count > 0) return 0;
    if (bulan < 1 || bulan > 12) return 0;

    pastikan_direktori_data();
    char filename[MAX_PATH];
    dapatkan_nama_file_bulan(FILE_POS_PREFIX, bulan, filename, sizeof(filename));

    FILE *fp = buka_file_tulis(filename);
    if (!fp) return 0;

    char line[512];
    for (int i = 0; i < count; i++) {
        format_pos_ke_string(&list[i], line, sizeof(line));
        fprintf(fp, "%s\n", line);
    }

    fclose(fp);
    return 1;
}

int tambah_pos_ke_file(PosAnggaran *pos, int bulan) {
    if (!pos || bulan < 1 || bulan > 12) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    if (count >= MAX_POS) return 0;

    /* Set nomor pos (auto-increment) */
    int max_no = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].no > max_no) max_no = list[i].no;
    }
    pos->no = max_no + 1;

    /* Inisialisasi nilai default */
    pos->realisasi = 0;
    pos->sisa = pos->nominal;
    pos->jumlah_transaksi = 0;
    pos->status = STATUS_AMAN;

    list[count] = *pos;
    count++;

    return simpan_pos_bulan(list, count, bulan);
}

int ubah_pos_di_file(int no, PosAnggaran *pos, int bulan) {
    if (!pos || no <= 0 || bulan < 1 || bulan > 12) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    for (int i = 0; i < count; i++) {
        if (list[i].no == no) {
            /* Pertahankan nomor asli */
            pos->no = no;
            list[i] = *pos;
            return simpan_pos_bulan(list, count, bulan);
        }
    }

    return 0;
}

int hapus_pos_dari_file(int no, int bulan) {
    if (no <= 0 || bulan < 1 || bulan > 12) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    int found = -1;
    for (int i = 0; i < count; i++) {
        if (list[i].no == no) {
            found = i;
            break;
        }
    }

    if (found < 0) return 0;

    /* Shift array untuk menghapus elemen */
    for (int i = found; i < count - 1; i++) {
        list[i] = list[i + 1];
    }

    return simpan_pos_bulan(list, count - 1, bulan);
}

int cari_pos_berdasarkan_nama(const char *nama, int bulan, PosAnggaran *result) {
    if (!nama || !result || bulan < 1 || bulan > 12) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    for (int i = 0; i < count; i++) {
        if (banding_string_tanpa_case(list[i].nama, nama) == 0) {
            *result = list[i];
            return 1;
        }
    }

    return 0;
}

int cari_pos_berdasarkan_nomor(int no, int bulan, PosAnggaran *result) {
    if (no <= 0 || !result || bulan < 1 || bulan > 12) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan);

    for (int i = 0; i < count; i++) {
        if (list[i].no == no) {
            *result = list[i];
            return 1;
        }
    }

    return 0;
}

int cek_file_pos_ada(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    char filename[MAX_PATH];
    dapatkan_nama_file_bulan(FILE_POS_PREFIX, bulan, filename, sizeof(filename));

    return cek_file_ada(filename);
}

int inisialisasi_pos_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;

    /* Cek apakah sudah ada */
    if (cek_file_pos_ada(bulan)) return 1;

    /* Buat file kosong */
    return simpan_pos_bulan(NULL, 0, bulan);
}

int salin_pos_dari_bulan(int bulan_tujuan, int bulan_sumber) {
    if (bulan_tujuan < 1 || bulan_tujuan > 12) return 0;
    if (bulan_sumber < 1 || bulan_sumber > 12) return 0;
    if (bulan_tujuan == bulan_sumber) return 0;

    PosAnggaran list[MAX_POS];
    int count = muat_pos_bulan(list, MAX_POS, bulan_sumber);

    if (count == 0) return 0;

    /* Reset realisasi dan sisa untuk bulan baru */
    for (int i = 0; i < count; i++) {
        list[i].realisasi = 0;
        list[i].sisa = list[i].nominal;
        list[i].jumlah_transaksi = 0;
        list[i].status = STATUS_AMAN;
    }

    return simpan_pos_bulan(list, count, bulan_tujuan);
}

/* ===== IMPLEMENTASI FILE TRANSAKSI ===== */

int muat_transaksi(Transaksi *list, int max_count) {
    if (!list || max_count <= 0) return 0;

    pastikan_direktori_data();
    char filename[MAX_PATH];
    dapatkan_nama_file_transaksi(filename, sizeof(filename));

    FILE *fp = buka_file_baca(filename);
    if (!fp) return 0;

    char line[512];
    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < max_count) {
        hapus_newline_string(line);
        if (strlen(line) == 0) continue;

        if (urai_baris_transaksi(line, &list[count])) {
            count++;
        }
    }

    fclose(fp);
    return count;
}

int muat_transaksi_bulan(Transaksi *list, int max_count, int bulan) {
    if (!list || max_count <= 0 || bulan < 1 || bulan > 12) return 0;

    Transaksi all[MAX_TRANSAKSI];
    int total = muat_transaksi(all, MAX_TRANSAKSI);

    int count = 0;
    for (int i = 0; i < total && count < max_count; i++) {
        Tanggal tgl;
        /* Filter berdasarkan bulan */
        if (urai_tanggal_struct(all[i].tanggal, &tgl) && tgl.bulan == bulan) {
            list[count++] = all[i];
        }
    }

    return count;
}

int simpan_transaksi(Transaksi *list, int count) {
    if (!list && count > 0) return 0;

    pastikan_direktori_data();
    char filename[MAX_PATH];
    dapatkan_nama_file_transaksi(filename, sizeof(filename));

    FILE *fp = buka_file_tulis(filename);
    if (!fp) return 0;

    char line[512];
    for (int i = 0; i < count; i++) {
        format_transaksi_ke_string(&list[i], line, sizeof(line));
        fprintf(fp, "%s\n", line);
    }

    fclose(fp);
    return 1;
}

int tambah_transaksi_ke_file(Transaksi *trx) {
    if (!trx) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi(list, MAX_TRANSAKSI);

    if (count >= MAX_TRANSAKSI) return 0;

    list[count] = *trx;
    count++;

    return simpan_transaksi(list, count);
}

int ubah_transaksi_di_file(const char *id, Transaksi *trx) {
    if (!id || !trx) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi(list, MAX_TRANSAKSI);

    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].id, id) == 0) {
            list[i] = *trx;
            return simpan_transaksi(list, count);
        }
    }

    return 0;
}

int hapus_transaksi_dari_file(const char *id) {
    if (!id) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi(list, MAX_TRANSAKSI);

    int found = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].id, id) == 0) {
            found = i;
            break;
        }
    }

    if (found < 0) return 0;

    /* Shift array */
    for (int i = found; i < count - 1; i++) {
        list[i] = list[i + 1];
    }

    return simpan_transaksi(list, count - 1);
}

int cari_transaksi_berdasarkan_id(const char *id, Transaksi *result) {
    if (!id || !result) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi(list, MAX_TRANSAKSI);

    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].id, id) == 0) {
            *result = list[i];
            return 1;
        }
    }

    return 0;
}

/**
 * Generate ID transaksi baru (format Txxxx)
 */
void buat_id_transaksi(char *buffer) {
    if (!buffer) return;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi(list, MAX_TRANSAKSI);

    int max_num = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].id[0] == 'T' && strlen(list[i].id) == 5) {
            int num = atoi(list[i].id + 1);
            if (num > max_num) max_num = num;
        }
    }

    sprintf(buffer, "T%04d", max_num + 1);
}

int hitung_transaksi_berdasarkan_pos(const char *nama_pos) {
    if (!nama_pos) return 0;

    Transaksi list[MAX_TRANSAKSI];
    int count = muat_transaksi(list, MAX_TRANSAKSI);

    int hasil = 0;
    for (int i = 0; i < count; i++) {
        if (banding_string_tanpa_case(list[i].pos, nama_pos) == 0) {
            hasil++;
        }
    }

    return hasil;
}
