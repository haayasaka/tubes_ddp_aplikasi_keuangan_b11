/**
 * @file utils.c
 * @brief Implementasi modul utilitas
 * @author Kelompok B11
 * @date 2025
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"
#include "constants.h"

/* --- utils_array.c --- */

/**
 * Mengalokasikan array Transaksi
 */
Transaksi* alloc_transaksi_array(int size) {
    if (size <= 0) return NULL;
    
    Transaksi *arr = (Transaksi*)malloc(size * sizeof(Transaksi));
    if (arr != NULL) {
        /* Inisialisasi semua elemen */
        memset(arr, 0, size * sizeof(Transaksi));
    }
    return arr;
}

/**
 * Mengalokasikan array PosAnggaran
 */
PosAnggaran* alloc_pos_array(int size) {
    if (size <= 0) return NULL;
    
    PosAnggaran *arr = (PosAnggaran*)malloc(size * sizeof(PosAnggaran));
    if (arr != NULL) {
        /* Inisialisasi semua elemen */
        memset(arr, 0, size * sizeof(PosAnggaran));
    }
    return arr;
}

/**
 * Membebaskan memory array Transaksi
 */
void free_transaksi_array(Transaksi *arr) {
    if (arr != NULL) {
        free(arr);
    }
}

/**
 * Membebaskan memory array PosAnggaran
 */
void free_pos_array(PosAnggaran *arr) {
    if (arr != NULL) {
        free(arr);
    }
}

/**
 * Menginisialisasi struct ListTransaksi
 */
int init_list_transaksi(ListTransaksi *list, int initial_capacity) {
    if (list == NULL || initial_capacity <= 0) return FAILURE;
    
    list->data = alloc_transaksi_array(initial_capacity);
    if (list->data == NULL) return FAILURE;
    
    list->count = 0;
    list->capacity = initial_capacity;
    return SUCCESS;
}

/**
 * Membebaskan memory ListTransaksi
 */
void free_list_transaksi(ListTransaksi *list) {
    if (list == NULL) return;
    
    free_transaksi_array(list->data);
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
}

/**
 * Menambahkan transaksi ke ListTransaksi
 */
int add_to_list_transaksi(ListTransaksi *list, const Transaksi *trx) {
    if (list == NULL || trx == NULL) return FAILURE;
    
    /* Cek apakah perlu memperbesar kapasitas */
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Transaksi *new_data = (Transaksi*)realloc(list->data, 
                                                   new_capacity * sizeof(Transaksi));
        if (new_data == NULL) return FAILURE;
        
        list->data = new_data;
        list->capacity = new_capacity;
    }
    
    /* Copy data transaksi */
    memcpy(&list->data[list->count], trx, sizeof(Transaksi));
    list->count++;
    
    return SUCCESS;
}

/**
 * Menginisialisasi struct ListPos
 */
int init_list_pos(ListPos *list, int initial_capacity) {
    if (list == NULL || initial_capacity <= 0) return FAILURE;
    
    list->data = alloc_pos_array(initial_capacity);
    if (list->data == NULL) return FAILURE;
    
    list->count = 0;
    list->capacity = initial_capacity;
    return SUCCESS;
}

/**
 * Membebaskan memory ListPos
 */
void free_list_pos(ListPos *list) {
    if (list == NULL) return;
    
    free_pos_array(list->data);
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
}

/**
 * Menambahkan pos anggaran ke ListPos
 */
int add_to_list_pos(ListPos *list, const PosAnggaran *pos) {
    if (list == NULL || pos == NULL) return FAILURE;
    
    /* Cek apakah perlu memperbesar kapasitas */
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        PosAnggaran *new_data = (PosAnggaran*)realloc(list->data, 
                                                       new_capacity * sizeof(PosAnggaran));
        if (new_data == NULL) return FAILURE;
        
        list->data = new_data;
        list->capacity = new_capacity;
    }
    
    /* Copy data pos */
    memcpy(&list->data[list->count], pos, sizeof(PosAnggaran));
    list->count++;
    
    return SUCCESS;
}

/**
 * Menginisialisasi struct Transaksi ke nilai default
 */
void init_transaksi(Transaksi *trx) {
    if (trx == NULL) return;
    
    memset(trx->id, 0, sizeof(trx->id));
    memset(trx->tanggal, 0, sizeof(trx->tanggal));
    trx->jenis = 0;
    memset(trx->pos, 0, sizeof(trx->pos));
    trx->nominal = 0;
    memset(trx->deskripsi, 0, sizeof(trx->deskripsi));
}

/**
 * Menginisialisasi struct PosAnggaran ke nilai default
 */
void init_pos_anggaran(PosAnggaran *pos) {
    if (pos == NULL) return;
    
    pos->no = 0;
    memset(pos->nama, 0, sizeof(pos->nama));
    pos->nominal = 0;
    pos->realisasi = 0;
    pos->sisa = 0;
    pos->jumlah_transaksi = 0;
    pos->status = STATUS_AMAN;
}

/**
 * Menginisialisasi struct AnalisisKeuangan ke nilai default
 */
void init_analisis_keuangan(AnalisisKeuangan *analisis) {
    if (analisis == NULL) return;
    
    analisis->total_pemasukan = 0;
    analisis->total_pengeluaran = 0;
    analisis->rata_rata_pengeluaran = 0.0;
    analisis->persentase_sisa = 0.0;
    analisis->saldo_akhir = 0;
    analisis->kondisi_keuangan = KONDISI_SEIMBANG;
    analisis->kesimpulan = KESIMPULAN_SEIMBANG;
    analisis->total_trx_pengeluaran = 0;
    analisis->total_trx_pemasukan = 0;
}

/* --- utils_bulan.c --- */

/* Array nama bulan dalam Bahasa Indonesia */
static const char *nama_bulan[] = {
    "Invalid",
    "Januari",
    "Februari",
    "Maret",
    "April",
    "Mei",
    "Juni",
    "Juli",
    "Agustus",
    "September",
    "Oktober",
    "November",
    "Desember"
};

/* Array nama bulan singkat */
static const char *nama_bulan_singkat[] = {
    "Inv",
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "Mei",
    "Jun",
    "Jul",
    "Agu",
    "Sep",
    "Okt",
    "Nov",
    "Des"
};

/* Array kode bulan 2 digit */
static const char *kode_bulan[] = {
    "00",
    "01", "02", "03", "04", "05", "06",
    "07", "08", "09", "10", "11", "12"
};

/**
 * Mendapatkan nama bulan dalam Bahasa Indonesia
 */
const char* get_nama_bulan(int bulan) {
    if (bulan < BULAN_MIN || bulan > BULAN_MAX) {
        return nama_bulan[0];  /* "Invalid" */
    }
    return nama_bulan[bulan];
}

/**
 * Mendapatkan kode bulan 2 digit
 */
const char* get_kode_bulan(int bulan) {
    if (bulan < BULAN_MIN || bulan > BULAN_MAX) {
        return kode_bulan[0];  /* "00" */
    }
    return kode_bulan[bulan];
}

/**
 * Mendapatkan nama bulan singkat (3 huruf)
 */
const char* get_nama_bulan_singkat(int bulan) {
    if (bulan < BULAN_MIN || bulan > BULAN_MAX) {
        return nama_bulan_singkat[0];  /* "Inv" */
    }
    return nama_bulan_singkat[bulan];
}

/**
 * Helper: Membandingkan string case-insensitive
 */
static int str_cmp_nocase(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

/**
 * Mengkonversi nama bulan ke nomor bulan
 */
int nama_ke_nomor_bulan(const char *nama) {
    if (nama == NULL) return 0;
    
    for (int i = 1; i <= 12; i++) {
        if (str_cmp_nocase(nama, nama_bulan[i]) == 0) {
            return i;
        }
        /* Cek juga nama singkat */
        if (str_cmp_nocase(nama, nama_bulan_singkat[i]) == 0) {
            return i;
        }
    }
    
    return 0;  /* Tidak ditemukan */
}

/**
 * Validasi nomor bulan
 */
int is_valid_bulan(int bulan) {
    return (bulan >= BULAN_MIN && bulan <= BULAN_MAX);
}

/* --- utils_date.c --- */

/* Tabel jumlah hari per bulan (non-kabisat) */
static const int days_in_month[] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/**
 * Parse string tanggal ke komponen hari, bulan, tahun
 */
int parse_tanggal(const char *str, int *d, int *m, int *y) {
    if (str == NULL || d == NULL || m == NULL || y == NULL) {
        return 0;
    }
    
    /* Format yang diharapkan: dd-mm-YYYY */
    int parsed = sscanf(str, "%d-%d-%d", d, m, y);
    
    if (parsed != 3) {
        return 0;
    }
    
    return 1;
}

/**
 * Parse string tanggal ke struct Tanggal
 */
int parse_tanggal_struct(const char *str, Tanggal *tgl) {
    if (str == NULL || tgl == NULL) {
        return 0;
    }
    
    return parse_tanggal(str, &tgl->hari, &tgl->bulan, &tgl->tahun);
}

/**
 * Format komponen tanggal ke string dd-mm-YYYY
 */
void format_tanggal(int d, int m, int y, char *result) {
    if (result == NULL) return;
    
    sprintf(result, "%02d-%02d-%04d", d, m, y);
}

/**
 * Mendapatkan bulan saat ini dari sistem
 */
int get_current_month(void) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    return local->tm_mon + 1;  /* tm_mon dimulai dari 0 */
}

/**
 * Mendapatkan tahun saat ini dari sistem
 */
int get_current_year(void) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    return local->tm_year + 1900;  /* tm_year dihitung dari 1900 */
}

/**
 * Mendapatkan hari saat ini dari sistem
 */
int get_current_day(void) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    return local->tm_mday;
}

/**
 * Memeriksa apakah tahun adalah tahun kabisat
 */
int is_leap_year(int year) {
    /* Tahun kabisat: habis dibagi 4 tapi tidak habis dibagi 100,
       kecuali jika habis dibagi 400 */
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    if (year % 4 == 0) return 1;
    return 0;
}

/**
 * Mendapatkan jumlah hari dalam bulan tertentu
 */
int get_days_in_month(int month, int year) {
    if (month < 1 || month > 12) return 0;
    
    if (month == 2 && is_leap_year(year)) {
        return 29;  /* Februari pada tahun kabisat */
    }
    
    return days_in_month[month];
}

/**
 * Mengekstrak bulan dari string tanggal
 */
int ekstrak_bulan(const char *tanggal) {
    int d, m, y;
    
    if (!parse_tanggal(tanggal, &d, &m, &y)) {
        return 0;
    }
    
    if (m < 1 || m > 12) return 0;
    
    return m;
}

/**
 * Mengekstrak tahun dari string tanggal
 */
int ekstrak_tahun(const char *tanggal) {
    int d, m, y;
    
    if (!parse_tanggal(tanggal, &d, &m, &y)) {
        return 0;
    }
    
    return y;
}

/**
 * Mendapatkan tanggal hari ini dalam format dd-mm-YYYY
 */
void get_today_string(char *result) {
    if (result == NULL) return;
    
    format_tanggal(get_current_day(), get_current_month(), get_current_year(), result);
}

/* --- utils_number.c --- */

/**
 * Helper: Membalik string in-place
 */
static void reverse_string(char *str) {
    int len = strlen(str);
    int i, j;
    char temp;
    
    for (i = 0, j = len - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

/**
 * Memformat angka dengan separator ribuan (tanpa prefix Rp)
 */
void format_number_separator(unsigned long long nominal, char *result) {
    if (result == NULL) return;
    
    if (nominal == 0) {
        strcpy(result, "0");
        return;
    }
    
    char buffer[30];
    char formatted[30];
    int i = 0, j = 0, count = 0;
    
    /* Konversi angka ke string */
    sprintf(buffer, "%llu", nominal);
    int len = strlen(buffer);
    
    /* Format dengan separator ribuan (titik) */
    for (i = len - 1; i >= 0; i--) {
        if (count > 0 && count % 3 == 0) {
            formatted[j++] = '.';
        }
        formatted[j++] = buffer[i];
        count++;
    }
    formatted[j] = '\0';
    
    reverse_string(formatted);
    strcpy(result, formatted);
}

/**
 * Memformat nominal ke format Rupiah dengan separator ribuan
 */
void format_rupiah(unsigned long long nominal, char *result) {
    if (result == NULL) return;
    
    char formatted[30];
    format_number_separator(nominal, formatted);
    
    sprintf(result, "Rp %s", formatted);
}

/**
 * Memformat persentase dengan 2 desimal
 */
void format_persentase(double persen, char *result) {
    if (result == NULL) return;
    
    sprintf(result, "%.2f%%", persen);
}

/**
 * Memformat nilai rata-rata dengan separator ribuan dan 2 desimal
 */
void format_rata_rata(double rata, char *result) {
    if (result == NULL) return;
    
    /* Ambil bagian bulat dan desimal */
    unsigned long long bagian_bulat = (unsigned long long)rata;
    int bagian_desimal = (int)((rata - bagian_bulat) * 100 + 0.5);
    
    /* Format bagian bulat dengan separator */
    char formatted[25];
    format_number_separator(bagian_bulat, formatted);
    
    /* Gabungkan dengan bagian desimal */
    sprintf(result, "%s,%02d", formatted, bagian_desimal);
}

/**
 * Memformat nilai saldo (bisa negatif) ke format Rupiah
 */
void format_saldo(long long saldo, char *result) {
    if (result == NULL) return;
    
    if (saldo < 0) {
        char formatted[30];
        format_number_separator((unsigned long long)(-saldo), formatted);
        sprintf(result, "-Rp %s", formatted);
    } else {
        format_rupiah((unsigned long long)saldo, result);
    }
}

/**
 * Mengkonversi string angka ke unsigned long long
 */
int str_to_ull(const char *str, unsigned long long *result) {
    if (str == NULL || result == NULL) return 0;
    
    /* Skip whitespace di awal */
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    
    if (*str == '\0') return 0;
    
    /* Pastikan hanya digit */
    const char *p = str;
    while (*p) {
        if (!isdigit((unsigned char)*p)) {
            return 0;
        }
        p++;
    }
    
    /* Konversi */
    char *endptr;
    *result = strtoull(str, &endptr, 10);
    
    return (*endptr == '\0');
}

/**
 * Memeriksa apakah string hanya berisi digit
 */
int is_numeric_string(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    /* Skip whitespace di awal */
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    
    if (*str == '\0') return 0;
    
    /* Cek setiap karakter */
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    
    return 1;
}

/* --- utils_string.c --- */

/**
 * Menghapus whitespace di awal dan akhir string
 */
void str_trim(char *str) {
    if (str == NULL || *str == '\0') return;
    
    /* Hapus whitespace di awal */
    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    /* Jika string hanya whitespace */
    if (*start == '\0') {
        str[0] = '\0';
        return;
    }
    
    /* Hapus whitespace di akhir */
    char *end = str + strlen(str) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    
    /* Geser string jika ada whitespace di awal */
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

/**
 * Mengkonversi string ke uppercase
 */
void str_to_upper(char *str) {
    if (str == NULL) return;
    
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

/**
 * Mengkonversi string ke lowercase
 */
void str_to_lower(char *str) {
    if (str == NULL) return;
    
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

/**
 * Menyalin string dengan batasan ukuran (safe copy)
 */
void str_copy_safe(char *dst, const char *src, int max) {
    if (dst == NULL || max <= 0) return;
    
    if (src == NULL) {
        dst[0] = '\0';
        return;
    }
    
    int i;
    for (i = 0; i < max - 1 && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }
    dst[i] = '\0';
}

/**
 * Memisahkan string berdasarkan delimiter
 * Catatan: Fungsi ini memodifikasi string asli
 */
int str_split(char *str, char delim, char **result, int max_parts) {
    if (str == NULL || result == NULL || max_parts <= 0) return 0;
    
    int count = 0;
    char *token = str;
    char *next;
    
    while (token != NULL && count < max_parts) {
        /* Cari delimiter berikutnya */
        next = strchr(token, delim);
        
        /* Simpan pointer ke bagian ini */
        result[count++] = token;
        
        if (next != NULL) {
            *next = '\0';  /* Ganti delimiter dengan null terminator */
            token = next + 1;
        } else {
            break;
        }
    }
    
    return count;
}

/**
 * Memeriksa apakah string kosong (hanya whitespace atau NULL)
 */
int str_is_empty(const char *str) {
    if (str == NULL) return 1;
    
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0;  /* Ditemukan karakter non-whitespace */
        }
        str++;
    }
    return 1;  /* String kosong atau hanya whitespace */
}

/**
 * Membandingkan dua string secara case-insensitive
 */
int str_compare_nocase(const char *s1, const char *s2) {
    if (s1 == NULL && s2 == NULL) return 0;
    if (s1 == NULL) return -1;
    if (s2 == NULL) return 1;
    
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

/**
 * Menghapus karakter newline dari akhir string
 */
void str_remove_newline(char *str) {
    if (str == NULL) return;
    
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}
