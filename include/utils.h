/**
 * @file utils.h
 * @brief Header untuk modul utilitas
 * @author Kelompok B11
 * @date 2025
 */

#ifndef UTILS_H
#define UTILS_H

/* ===== KODE RETURN ===== */
#define SUCCESS                 1       /* Operasi berhasil */
#define FAILURE                 0       /* Operasi gagal */
#define CANCEL                  -1      /* Operasi dibatalkan user */

/* ===== BULAN ===== */
#define BULAN_MIN               1       /* Januari */
#define BULAN_MAX               12      /* Desember */

/**
 * @brief Struct untuk menyimpan komponen tanggal
 * 
 * Digunakan untuk parsing dan manipulasi tanggal
 */
typedef struct {
    int hari;                   /* Hari (1-31) */
    int bulan;                  /* Bulan (1-12) */
    int tahun;                  /* Tahun (4 digit) */
} Tanggal;

// Forward declarations for types used in alloc functions
// The actual definitions are in their respective headers
typedef struct Transaksi Transaksi;
typedef struct PosAnggaran PosAnggaran;
typedef struct AnalisisKeuangan AnalisisKeuangan;
typedef struct ListTransaksi ListTransaksi;
typedef struct ListPos ListPos;

/* --- utils_array.h --- */

/*
    Function bertujuan untuk mengalokasikan array Transaksi secara dinamis.
    Input : size (jumlah elemen yang akan dialokasikan)
    Output : Mengembalikan pointer ke array Transaksi yang dialokasikan, atau NULL jika gagal.
*/
Transaksi* alloc_transaksi_array(int size);

/*
    Function bertujuan untuk mengalokasikan array PosAnggaran secara dinamis.
    Input : size (jumlah elemen yang akan dialokasikan)
    Output : Mengembalikan pointer ke array PosAnggaran yang dialokasikan, atau NULL jika gagal.
*/
PosAnggaran* alloc_pos_array(int size);

/*
    Procedure bertujuan untuk membebaskan memory array Transaksi.
    I. S. : arr (pointer ke array yang akan dibebaskan, bisa NULL)
    F. S. : Memory yang ditunjuk arr dibebaskan.
*/
void free_transaksi_array(Transaksi *arr);

/*
    Procedure bertujuan untuk membebaskan memory array PosAnggaran.
    I. S. : arr (pointer ke array yang akan dibebaskan, bisa NULL)
    F. S. : Memory yang ditunjuk arr dibebaskan.
*/
void free_pos_array(PosAnggaran *arr);

/*
    Function bertujuan untuk menginisialisasi struct ListTransaksi.
    Input : list (pointer ke ListTransaksi), initial_capacity (kapasitas awal)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int init_list_transaksi(ListTransaksi *list, int initial_capacity);

/*
    Procedure bertujuan untuk membebaskan memory ListTransaksi.
    I. S. : list (pointer ke ListTransaksi yang terdefinisi)
    F. S. : Memory data list dibebaskan, count dan capacity di-reset ke 0.
*/
void free_list_transaksi(ListTransaksi *list);

/*
    Function bertujuan untuk menambahkan transaksi ke ListTransaksi.
    Input : list (pointer ke ListTransaksi), trx (pointer ke Transaksi yang ditambahkan)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int add_to_list_transaksi(ListTransaksi *list, const Transaksi *trx);

/*
    Function bertujuan untuk menginisialisasi struct ListPos.
    Input : list (pointer ke ListPos), initial_capacity (kapasitas awal)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int init_list_pos(ListPos *list, int initial_capacity);

/*
    Procedure bertujuan untuk membebaskan memory ListPos.
    I. S. : list (pointer ke ListPos yang terdefinisi)
    F. S. : Memory data list dibebaskan, count dan capacity di-reset ke 0.
*/
void free_list_pos(ListPos *list);

/*
    Function bertujuan untuk menambahkan pos anggaran ke ListPos.
    Input : list (pointer ke ListPos), pos (pointer ke PosAnggaran yang ditambahkan)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int add_to_list_pos(ListPos *list, const PosAnggaran *pos);

/*
    Procedure bertujuan untuk menginisialisasi struct Transaksi ke nilai default.
    I. S. : trx (pointer ke Transaksi, bisa belum diinisialisasi)
    F. S. : Field-field trx diisi dengan nilai default (0/kosong).
*/
void init_transaksi(Transaksi *trx);

/*
    Procedure bertujuan untuk menginisialisasi struct PosAnggaran ke nilai default.
    I. S. : pos (pointer ke PosAnggaran, bisa belum diinisialisasi)
    F. S. : Field-field pos diisi dengan nilai default.
*/
void init_pos_anggaran(PosAnggaran *pos);

/*
    Procedure bertujuan untuk menginisialisasi struct AnalisisKeuangan ke nilai default.
    I. S. : analisis (pointer ke AnalisisKeuangan, bisa belum diinisialisasi)
    F. S. : Field-field analisis diisi dengan nilai default.
*/
void init_analisis_keuangan(AnalisisKeuangan *analisis);

/* --- utils_bulan.h --- */

/*
    Function bertujuan untuk mendapatkan nama bulan dalam Bahasa Indonesia.
    Input : bulan (nomor bulan 1-12)
    Output : Mengembalikan pointer ke string nama bulan (statis), atau "Invalid" jika di luar range.
*/
const char* get_nama_bulan(int bulan);

/*
    Function bertujuan untuk mendapatkan kode bulan 2 digit.
    Input : bulan (nomor bulan 1-12)
    Output : Mengembalikan pointer ke string kode bulan (statis), atau "00" jika di luar range.
*/
const char* get_kode_bulan(int bulan);

/*
    Function bertujuan untuk mendapatkan nama bulan singkat (3 huruf).
    Input : bulan (nomor bulan 1-12)
    Output : Mengembalikan pointer ke string nama bulan singkat (statis).
*/
const char* get_nama_bulan_singkat(int bulan);

/*
    Function bertujuan untuk mengkonversi nama bulan ke nomor bulan.
    Input : nama (nama bulan, case-insensitive)
    Output : Mengembalikan nomor bulan (1-12), atau 0 jika tidak valid.
*/
int nama_ke_nomor_bulan(const char *nama);

/*
    Function bertujuan untuk memvalidasi nomor bulan.
    Input : bulan (nomor bulan yang akan divalidasi)
    Output : Mengembalikan 1 jika valid (1-12), 0 jika tidak valid.
*/
int is_valid_bulan(int bulan);

/* --- utils_date.h --- */

/*
    Function bertujuan untuk mem-parse string tanggal ke komponen hari, bulan, tahun.
    Input : str (string tanggal format dd-mm-YYYY)
    Output : d (hari), m (bulan), y (tahun) diisi via pointer. Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int parse_tanggal(const char *str, int *d, int *m, int *y);

/*
    Function bertujuan untuk mem-parse string tanggal ke struct Tanggal.
    Input : str (string tanggal format dd-mm-YYYY)
    Output : tgl (struct Tanggal) diisi via pointer. Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int parse_tanggal_struct(const char *str, Tanggal *tgl);

/*
    Procedure bertujuan untuk memformat komponen tanggal ke string dd-mm-YYYY.
    Input : d (hari), m (bulan), y (tahun)
    Output : result (string tanggal terformat "dd-mm-YYYY")
*/
void format_tanggal(int d, int m, int y, char *result);

/*
    Function bertujuan untuk mendapatkan bulan saat ini dari sistem.
    Input : -
    Output : Mengembalikan bulan saat ini (1-12).
*/
int get_current_month(void);

/*
    Function bertujuan untuk mendapatkan tahun saat ini dari sistem.
    Input : -
    Output : Mengembalikan tahun saat ini.
*/
int get_current_year(void);

/*
    Function bertujuan untuk mendapatkan hari saat ini dari sistem.
    Input : -
    Output : Mengembalikan hari saat ini (1-31).
*/
int get_current_day(void);

/*
    Function bertujuan untuk memeriksa apakah tahun adalah tahun kabisat.
    Input : year (tahun yang akan diperiksa)
    Output : Mengembalikan 1 jika tahun kabisat, 0 jika bukan.
*/
int is_leap_year(int year);

/*
    Function bertujuan untuk mendapatkan jumlah hari dalam bulan tertentu.
    Input : month (bulan 1-12), year (tahun untuk pengecekan kabisat)
    Output : Mengembalikan jumlah hari dalam bulan.
*/
int get_days_in_month(int month, int year);

/*
    Function bertujuan untuk mengekstrak bulan dari string tanggal.
    Input : tanggal (string tanggal format dd-mm-YYYY)
    Output : Mengembalikan nomor bulan (1-12), atau 0 jika gagal.
*/
int ekstrak_bulan(const char *tanggal);

/*
    Function bertujuan untuk mengekstrak tahun dari string tanggal.
    Input : tanggal (string tanggal format dd-mm-YYYY)
    Output : Mengembalikan tahun, atau 0 jika gagal.
*/
int ekstrak_tahun(const char *tanggal);

/*
    Procedure bertujuan untuk mendapatkan tanggal hari ini dalam format dd-mm-YYYY.
    Input : -
    Output : result (string tanggal hari ini "dd-mm-YYYY")
*/
void get_today_string(char *result);

/* --- utils_number.h --- */

/*
    Procedure bertujuan untuk memformat nominal ke format Rupiah dengan separator ribuan.
    Input : nominal (nilai nominal)
    Output : result (string format Rupiah, misal "Rp 1.000.000")
*/
void format_rupiah(unsigned long long nominal, char *result);

/*
    Procedure bertujuan untuk memformat persentase dengan 2 desimal.
    Input : persen (nilai persentase)
    Output : result (string format persentase, misal "62.14%")
*/
void format_persentase(double persen, char *result);

/*
    Procedure bertujuan untuk memformat nilai rata-rata dengan 2 desimal.
    Input : rata (nilai rata-rata)
    Output : result (string format rata-rata, misal "29.444,33")
*/
void format_rata_rata(double rata, char *result);

/*
    Procedure bertujuan untuk memformat angka dengan separator ribuan (tanpa prefix Rp).
    Input : nominal (nilai yang akan diformat)
    Output : result (string angka dengan separator, misal "1.000.000")
*/
void format_number_separator(unsigned long long nominal, char *result);

/*
    Procedure bertujuan untuk memformat nilai saldo (bisa negatif) ke format Rupiah.
    Input : saldo (nilai saldo)
    Output : result (string saldo format Rupiah, misal "-Rp 100.000")
*/
void format_saldo(long long saldo, char *result);

/*
    Function bertujuan untuk mengkonversi string angka ke unsigned long long.
    Input : str (string yang berisi angka)
    Output : result (menyimpan hasil konversi via pointer). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int str_to_ull(const char *str, unsigned long long *result);

/*
    Function bertujuan untuk memeriksa apakah string hanya berisi digit.
    Input : str (string yang akan diperiksa)
    Output : Mengembalikan 1 jika hanya digit, 0 jika tidak.
*/
int is_numeric_string(const char *str);

/* --- utils_string.h --- */

/*
    Procedure bertujuan untuk menghapus whitespace di awal dan akhir string.
    I. S. : str (string mungkin ada whitespace di awal/akhir)
    F. S. : str tidak lagi memiliki whitespace di awal/akhir.
*/
void str_trim(char *str);

/*
    Procedure bertujuan untuk mengkonversi string ke uppercase.
    I. S. : str (string berisi huruf)
    F. S. : Seluruh huruf kecil di str diubah menjadi huruf besar.
*/
void str_to_upper(char *str);

/*
    Procedure bertujuan untuk mengkonversi string ke lowercase.
    I. S. : str (string berisi huruf)
    F. S. : Seluruh huruf besar di str diubah menjadi huruf kecil.
*/
void str_to_lower(char *str);

/*
    Procedure bertujuan untuk menyalin string dengan batasan ukuran (safe copy).
    I. S. : dst (buffer tujuan), src (string sumber), max (ukuran maksimal buffer)
    F. S. : isi src disalin ke dst (maksimal max-1 karakter), dst null-terminated.
*/
void str_copy_safe(char *dst, const char *src, int max);

/*
    Function bertujuan untuk memisahkan string berdasarkan delimiter.
    Input : str (string yang dipisahkan), delim (karakter delimiter), max_parts (jumlah maksimal bagian)
    Output : result (array pointer menyimpan hasil split). Mengembalikan jumlah bagian yang dihasilkan.
*/
int str_split(char *str, char delim, char **result, int max_parts);

/*
    Function bertujuan untuk memeriksa apakah string kosong (hanya whitespace atau NULL).
    Input : str (string yang diperiksa)
    Output : Mengembalikan 1 jika kosong, 0 jika tidak.
*/
int str_is_empty(const char *str);

/*
    Function bertujuan untuk membandingkan dua string secara case-insensitive.
    Input : s1, s2 (string yang dibandingkan)
    Output : Mengembalikan 0 jika sama, non-zero jika berbeda.
*/
int str_compare_nocase(const char *s1, const char *s2);

/*
    Procedure bertujuan untuk menghapus karakter newline dari akhir string.
    I. S. : str (string mungkin ada newline di akhir)
    F. S. : Karakter newline di akhir str dihapus.
*/
void str_remove_newline(char *str);

#endif /* UTILS_H */