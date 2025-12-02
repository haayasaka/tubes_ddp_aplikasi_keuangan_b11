/**
 * @file file.c
 * @brief Implementasi modul operasi file
 * @author Kelompok B11
 * @date 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "file.h"
#include "utils.h"

/* --- file_core.c --- */

/**
 * Cek apakah file ada
 */
int file_exists(const char *filename) {
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
int file_create(const char *filename) {
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
FILE* file_open_read(const char *filename) {
    if (filename == NULL) return NULL;
    
    return fopen(filename, "r");
}

/**
 * Buka file untuk ditulis (overwrite)
 */
FILE* file_open_write(const char *filename) {
    if (filename == NULL) return NULL;
    
    return fopen(filename, "w");
}

/**
 * Buka file untuk append
 */
FILE* file_open_append(const char *filename) {
    if (filename == NULL) return NULL;
    
    return fopen(filename, "a");
}

/**
 * Generate nama file dengan suffix bulan
 * I.S.: base string, bulan integer, result buffer
 * F.S.: result berisi path file lengkap
 */
void get_filename_bulan(const char *base, int bulan, char *result, int result_size) {
    if (base == NULL || result == NULL || result_size <= 0) return; 
    
    /* Validasi bulan */
    if (bulan < BULAN_MIN || bulan > BULAN_MAX) {
        bulan = 1;  /* Default ke Januari jika invalid */
    }
    
    /* Format: data/base_MM.txt */
    snprintf(result, result_size, "%s%s_%s%s", DATA_DIR, base, get_kode_bulan(bulan), FILE_EXTENSION);
}

/**
 * Generate nama file transaksi
 * I.S.: result buffer
 * F.S.: result berisi path file transaksi
 */
void get_filename_transaksi(char *result, int result_size) {
    if (result == NULL || result_size <= 0) return; 
    
    snprintf(result, result_size, "%s%s%s", DATA_DIR, FILE_TRANSAKSI, FILE_EXTENSION);
}

/**
 * Memastikan direktori data ada
 */
int ensure_data_directory(void) {
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
 * Membaca seluruh isi file ke buffer
 */
int file_read_all(const char *filename, char *buffer, int buffer_size) {
    if (filename == NULL || buffer == NULL || buffer_size <= 0) {
        return 0;
    }
    
    FILE *f = file_open_read(filename);
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
int file_write_all(const char *filename, const char *content) {
    if (filename == NULL) return 0;
    
    FILE *f = file_open_write(filename);
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
int file_append_line(const char *filename, const char *line) {
    if (filename == NULL || line == NULL) return 0;
    
    /* Jika file belum ada, buat dulu */
    if (!file_exists(filename)) {
        if (!file_create(filename)) {
            return 0;
        }
    }
    
    FILE *f = file_open_append(filename);
    if (f == NULL) {
        return 0;
    }
    
    fprintf(f, "%s\n", line);
    fclose(f);
    
    return 1;
}

/* --- file_parser.c --- */

/**
 * Helper: Safe string to int conversion
 */
static int safe_atoi(const char *str) {
    if (str == NULL) return 0;
    return atoi(str);
}

/**
 * Helper: Safe string to unsigned long long conversion
 */
static unsigned long long safe_atoull(const char *str) {
    if (str == NULL) return 0;
    return strtoull(str, NULL, 10);
}

/**
 * Helper: Safe string to double conversion
 */
static double safe_atof(const char *str) {
    if (str == NULL) return 0.0;
    return atof(str);
}

/**
 * Helper: Safe string to long long conversion
 */
static long long safe_atoll(const char *str) {
    if (str == NULL) return 0;
    return strtoll(str, NULL, 10);
}

/**
 * Mendapatkan label jenis transaksi
 */
const char* get_label_jenis(int jenis) {
    if (jenis == JENIS_PEMASUKAN) {
        return "Pemasukan";
    }
    return "Pengeluaran";
}

/**
 * Konversi string jenis ke kode
 */
int parse_jenis_transaksi(const char *jenis_str) {
    if (jenis_str == NULL) return -1;
    
    /* Case-insensitive comparison */
    if (str_compare_nocase(jenis_str, "Pemasukan") == 0) {
        return JENIS_PEMASUKAN;
    }
    if (str_compare_nocase(jenis_str, "Pengeluaran") == 0) {
        return JENIS_PENGELUARAN;
    }
    
    return -1;  /* Invalid */
}

/**
 * Parse baris teks ke struct Transaksi
 * Format: id|tanggal|jenis|pos|nominal|deskripsi
 */
int parse_line_transaksi(const char *line, Transaksi *trx) {
    if (line == NULL || trx == NULL) return 0;
    
    /* Buat salinan line karena strtok memodifikasi string */
    char buffer[MAX_LINE_LENGTH];
    str_copy_safe(buffer, line, MAX_LINE_LENGTH);
    str_remove_newline(buffer);
    
    /* Parse field dengan delimiter | */
    char *token;
    int field = 0;
    
    token = strtok(buffer, "|");
    while (token != NULL && field < 6) {
        str_trim(token);
        
        switch (field) {
            case 0:  /* id */
                str_copy_safe(trx->id, token, sizeof(trx->id));
                break;
            case 1:  /* tanggal */
                str_copy_safe(trx->tanggal, token, sizeof(trx->tanggal));
                break;
            case 2:  /* jenis */
                trx->jenis = parse_jenis_transaksi(token);
                break;
            case 3:  /* pos */
                str_copy_safe(trx->pos, token, sizeof(trx->pos));
                break;
            case 4:  /* nominal */
                trx->nominal = safe_atoull(token);
                break;
            case 5:  /* deskripsi */
                str_copy_safe(trx->deskripsi, token, sizeof(trx->deskripsi));
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
int parse_line_pos(const char *line, PosAnggaran *pos) {
    if (line == NULL || pos == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    str_copy_safe(buffer, line, MAX_LINE_LENGTH);
    str_remove_newline(buffer);
    
    char *token;
    int field = 0;
    
    token = strtok(buffer, "|");
    while (token != NULL && field < 7) {
        str_trim(token);
        
        switch (field) {
            case 0:  /* no */
                pos->no = safe_atoi(token);
                break;
            case 1:  /* nama */
                str_copy_safe(pos->nama, token, sizeof(pos->nama));
                break;
            case 2:  /* nominal */
                pos->nominal = safe_atoull(token);
                break;
            case 3:  /* realisasi */
                pos->realisasi = safe_atoull(token);
                break;
            case 4:  /* sisa */
                pos->sisa = safe_atoll(token);
                break;
            case 5:  /* jumlah_transaksi */
                pos->jumlah_transaksi = safe_atoi(token);
                break;
            case 6:  /* status */
                pos->status = safe_atoi(token);
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
int parse_line_analisis(const char *line, AnalisisKeuangan *data) {
    if (line == NULL || data == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    str_copy_safe(buffer, line, MAX_LINE_LENGTH);
    str_remove_newline(buffer);
    
    char *token;
    int field = 0;
    
    token = strtok(buffer, "|");
    while (token != NULL && field < 9) {
        str_trim(token);
        
        switch (field) {
            case 0:  /* total_pemasukan */
                data->total_pemasukan = safe_atoull(token);
                break;
            case 1:  /* total_pengeluaran */
                data->total_pengeluaran = safe_atoull(token);
                break;
            case 2:  /* rata_rata_pengeluaran */
                data->rata_rata_pengeluaran = safe_atof(token);
                break;
            case 3:  /* persentase_sisa */
                data->persentase_sisa = safe_atof(token);
                break;
            case 4:  /* saldo_akhir */
                data->saldo_akhir = safe_atoll(token);
                break;
            case 5:  /* kondisi_keuangan */
                data->kondisi_keuangan = safe_atoi(token);
                break;
            case 6:  /* kesimpulan */
                data->kesimpulan = safe_atoi(token);
                break;
            case 7:  /* total_trx_pengeluaran */
                data->total_trx_pengeluaran = safe_atoi(token);
                break;
            case 8:  /* total_trx_pemasukan */
                data->total_trx_pemasukan = safe_atoi(token);
                break;
        }
        
        field++;
        token = strtok(NULL, "|");
    }
    
    return (field >= 9);
}

/**
 * Format struct Transaksi ke string pipe-delimited
 * I.S.: trx terdefinisi, result buffer
 * F.S.: result berisi string format file untuk transaksi
 */
void format_transaksi(const Transaksi *trx, char *result, int result_size) {
    if (trx == NULL || result == NULL || result_size <= 0) return; 
    
    snprintf(result, result_size, "%s|%s|%s|%s|%llu|%s",
            trx->id,
            trx->tanggal,
            get_label_jenis(trx->jenis),
            trx->pos,
            trx->nominal,
            trx->deskripsi);
}

/**
 * Format struct PosAnggaran ke string pipe-delimited
 * I.S.: pos terdefinisi, result buffer
 * F.S.: result berisi string format file untuk pos anggaran
 */
void format_pos(const PosAnggaran *pos, char *result, int result_size) {
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
 * I.S.: data terdefinisi, result buffer
 * F.S.: result berisi string format file untuk analisis
 */
void format_analisis(const AnalisisKeuangan *data, char *result, int result_size) {
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

/* --- file_analisis.c --- */

int load_analisis_bulan(int bulan, AnalisisKeuangan *result) {
    if (!result || bulan < 1 || bulan > 12) return 0;
    
    ensure_data_directory();
    char filename[MAX_PATH];
    get_filename_bulan(FILE_ANALISIS_PREFIX, bulan, filename, sizeof(filename));
    
    FILE *fp = file_open_read(filename);
    if (!fp) return 0;
    
    char line[512];
    int success = 0;
    
    if (fgets(line, sizeof(line), fp)) {
        str_remove_newline(line);
        if (strlen(line) > 0) {
            success = parse_line_analisis(line, result);
        }
    }
    
    fclose(fp);
    return success;
}

int save_analisis_bulan(int bulan, AnalisisKeuangan *analisis) {
    if (!analisis || bulan < 1 || bulan > 12) return 0;
    
    ensure_data_directory();
    char filename[MAX_PATH];
    get_filename_bulan(FILE_ANALISIS_PREFIX, bulan, filename, sizeof(filename));
    
    FILE *fp = file_open_write(filename);
    if (!fp) return 0;
    
    char line[512];
    format_analisis(analisis, line, sizeof(line));
    fprintf(fp, "%s\n", line);
    
    fclose(fp);
    return 1;
}

int analisis_file_exists(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    char filename[MAX_PATH];
    get_filename_bulan(FILE_ANALISIS_PREFIX, bulan, filename, sizeof(filename));
    
    return file_exists(filename);
}

int init_analisis_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    /* Cek apakah sudah ada */
    if (analisis_file_exists(bulan)) return 1;
    
    /* Buat analisis kosong */
    AnalisisKeuangan analisis;
    memset(&analisis, 0, sizeof(AnalisisKeuangan));
    analisis.kondisi_keuangan = KONDISI_SEIMBANG;
    analisis.kesimpulan = KESIMPULAN_SEIMBANG;
    
    return save_analisis_bulan(bulan, &analisis);
}

/* --- file_pos.c --- */

int load_pos_bulan(PosAnggaran *list, int max_count, int bulan) {
    if (!list || max_count <= 0 || bulan < 1 || bulan > 12) return 0;
    
    ensure_data_directory();
    char filename[MAX_PATH];
    get_filename_bulan(FILE_POS_PREFIX, bulan, filename, sizeof(filename));
    
    FILE *fp = file_open_read(filename);
    if (!fp) return 0;
    
    char line[512];
    int count = 0;
    
    while (fgets(line, sizeof(line), fp) && count < max_count) {
        str_remove_newline(line);
        if (strlen(line) == 0) continue;
        
        if (parse_line_pos(line, &list[count])) {
            count++;
        }
    }
    
    fclose(fp);
    return count;
}

int save_pos_bulan(PosAnggaran *list, int count, int bulan) {
    if (!list && count > 0) return 0;
    if (bulan < 1 || bulan > 12) return 0;
    
    ensure_data_directory();
    char filename[MAX_PATH];
    get_filename_bulan(FILE_POS_PREFIX, bulan, filename, sizeof(filename));
    
    FILE *fp = file_open_write(filename);
    if (!fp) return 0;
    
    char line[512];
    for (int i = 0; i < count; i++) {
        format_pos(&list[i], line, sizeof(line));
        fprintf(fp, "%s\n", line);
    }
    
    fclose(fp);
    return 1;
}

int add_pos(PosAnggaran *pos, int bulan) {
    if (!pos || bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
    if (count >= MAX_POS) return 0;
    
    /* Set nomor pos */
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
    
    return save_pos_bulan(list, count, bulan);
}

int update_pos(int no, PosAnggaran *pos, int bulan) {
    if (!pos || no <= 0 || bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
    for (int i = 0; i < count; i++) {
        if (list[i].no == no) {
            /* Pertahankan nomor asli */
            pos->no = no;
            list[i] = *pos;
            return save_pos_bulan(list, count, bulan);
        }
    }
    
    return 0;
}

int delete_pos(int no, int bulan) {
    if (no <= 0 || bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
    int found = -1;
    for (int i = 0; i < count; i++) {
        if (list[i].no == no) {
            found = i;
            break;
        }
    }
    
    if (found < 0) return 0;
    
    /* Shift array */
    for (int i = found; i < count - 1; i++) {
        list[i] = list[i + 1];
    }
    
    return save_pos_bulan(list, count - 1, bulan);
}

int find_pos_by_nama(const char *nama, int bulan, PosAnggaran *result) {
    if (!nama || !result || bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
    for (int i = 0; i < count; i++) {
        if (str_compare_nocase(list[i].nama, nama) == 0) {
            *result = list[i];
            return 1;
        }
    }
    
    return 0;
}

int find_pos_by_no(int no, int bulan, PosAnggaran *result) {
    if (no <= 0 || !result || bulan < 1 || bulan > 12) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan);
    
    for (int i = 0; i < count; i++) {
        if (list[i].no == no) {
            *result = list[i];
            return 1;
        }
    }
    
    return 0;
}

int pos_file_exists(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    char filename[MAX_PATH];
    get_filename_bulan(FILE_POS_PREFIX, bulan, filename, sizeof(filename));
    
    return file_exists(filename);
}

int init_pos_bulan(int bulan) {
    if (bulan < 1 || bulan > 12) return 0;
    
    /* Cek apakah sudah ada */
    if (pos_file_exists(bulan)) return 1;
    
    /* Buat file kosong */
    return save_pos_bulan(NULL, 0, bulan);
}

int copy_pos_from_bulan(int bulan_tujuan, int bulan_sumber) {
    if (bulan_tujuan < 1 || bulan_tujuan > 12) return 0;
    if (bulan_sumber < 1 || bulan_sumber > 12) return 0;
    if (bulan_tujuan == bulan_sumber) return 0;
    
    PosAnggaran list[MAX_POS];
    int count = load_pos_bulan(list, MAX_POS, bulan_sumber);
    
    if (count == 0) return 0;
    
    /* Reset realisasi dan sisa untuk bulan baru */
    for (int i = 0; i < count; i++) {
        list[i].realisasi = 0;
        list[i].sisa = list[i].nominal;
        list[i].jumlah_transaksi = 0;
        list[i].status = STATUS_AMAN;
    }
    
    return save_pos_bulan(list, count, bulan_tujuan);
}

/* --- file_transaksi.c --- */

int load_transaksi(Transaksi *list, int max_count) {
    if (!list || max_count <= 0) return 0;
    
    ensure_data_directory();
    char filename[MAX_PATH];
    get_filename_transaksi(filename, sizeof(filename));
    
    FILE *fp = file_open_read(filename);
    if (!fp) return 0;
    
    char line[512];
    int count = 0;
    
    while (fgets(line, sizeof(line), fp) && count < max_count) {
        str_remove_newline(line);
        if (strlen(line) == 0) continue;
        
        if (parse_line_transaksi(line, &list[count])) {
            count++;
        }
    }
    
    fclose(fp);
    return count;
}

int load_transaksi_bulan(Transaksi *list, int max_count, int bulan) {
    if (!list || max_count <= 0 || bulan < 1 || bulan > 12) return 0;
    
    Transaksi all[MAX_TRANSAKSI];
    int total = load_transaksi(all, MAX_TRANSAKSI);
    
    int count = 0;
    for (int i = 0; i < total && count < max_count; i++) {
        Tanggal tgl;
        if (parse_tanggal_struct(all[i].tanggal, &tgl) && tgl.bulan == bulan) {
            list[count++] = all[i];
        }
    }
    
    return count;
}

int save_transaksi(Transaksi *list, int count) {
    if (!list && count > 0) return 0;
    
    ensure_data_directory();
    char filename[MAX_PATH];
    get_filename_transaksi(filename, sizeof(filename));
    
    FILE *fp = file_open_write(filename);
    if (!fp) return 0;
    
    char line[512];
    for (int i = 0; i < count; i++) {
        format_transaksi(&list[i], line, sizeof(line));
        fprintf(fp, "%s\n", line);
    }
    
    fclose(fp);
    return 1;
}

int add_transaksi(Transaksi *trx) {
    if (!trx) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi(list, MAX_TRANSAKSI);
    
    if (count >= MAX_TRANSAKSI) return 0;
    
    list[count] = *trx;
    count++;
    
    return save_transaksi(list, count);
}

int update_transaksi(const char *id, Transaksi *trx) {
    if (!id || !trx) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi(list, MAX_TRANSAKSI);
    
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].id, id) == 0) {
            list[i] = *trx;
            return save_transaksi(list, count);
        }
    }
    
    return 0;
}

int delete_transaksi(const char *id) {
    if (!id) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi(list, MAX_TRANSAKSI);
    
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
    
    return save_transaksi(list, count - 1);
}

int find_transaksi_by_id(const char *id, Transaksi *result) {
    if (!id || !result) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi(list, MAX_TRANSAKSI);
    
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].id, id) == 0) {
            *result = list[i];
            return 1;
        }
    }
    
    return 0;
}

/**
 * Generate ID transaksi baru
 * I.S.: buffer terdefinisi
 * F.S.: buffer berisi ID unik baru (format Txxxx)
 */
void generate_transaksi_id(char *buffer) {
    if (!buffer) return; 
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi(list, MAX_TRANSAKSI);
    
    int max_num = 0;
    for (int i = 0; i < count; i++) {
        if (list[i].id[0] == 'T' && strlen(list[i].id) == 5) {
            int num = atoi(list[i].id + 1);
            if (num > max_num) max_num = num;
        }
    }
    
    sprintf(buffer, "T%04d", max_num + 1);
}

int count_transaksi_by_pos(const char *nama_pos) {
    if (!nama_pos) return 0;
    
    Transaksi list[MAX_TRANSAKSI];
    int count = load_transaksi(list, MAX_TRANSAKSI);
    
    int hasil = 0;
    for (int i = 0; i < count; i++) {
        if (str_compare_nocase(list[i].pos, nama_pos) == 0) {
            hasil++;
        }
    }
    
    return hasil;
}