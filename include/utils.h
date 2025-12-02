#ifndef UTILS_H
#define UTILS_H

/* ===== DEFINISI KONSTANTA ===== */

/* KODE RETURN */
#define SUCCESS                 1       /* Operasi berhasil */
#define FAILURE                 0       /* Operasi gagal */
#define CANCEL                  -1      /* Operasi dibatalkan user */

/* BULAN */
#define BULAN_MIN               1       /* Januari */
#define BULAN_MAX               12      /* Desember */

/* ===== DEFINISI STRUKTUR DATA ===== */

/**
 * Struct untuk menyimpan komponen tanggal
 *
 * Digunakan untuk parsing dan manipulasi tanggal
 */
typedef struct {
    int hari;                   /* Hari (1-31) */
    int bulan;                  /* Bulan (1-12) */
    int tahun;                  /* Tahun (4 digit) */
} Tanggal;

/* Forward declarations untuk tipe yang digunakan pada fungsi-fungsi alloc */
/* Definisi sebenarnya ada pada .c masing-masing */
typedef struct Transaksi Transaksi;
typedef struct PosAnggaran PosAnggaran;
typedef struct AnalisisKeuangan AnalisisKeuangan;
typedef struct ListTransaksi ListTransaksi;
typedef struct ListPos ListPos;

/*
    Function bertujuan untuk mengalokasikan array Transaksi secara dinamis.
    Input : size (Jumlah elemen yang akan dialokasikan)
    Output : Mengembalikan pointer ke array Transaksi yang dialokasikan, atau NULL jika gagal.
*/
Transaksi* alokasi_array_transaksi(int size);

/*
    Function bertujuan untuk mengalokasikan array PosAnggaran secara dinamis.
    Input : size (Jumlah elemen yang akan dialokasikan)
    Output : Mengembalikan pointer ke array PosAnggaran yang dialokasikan, atau NULL jika gagal.
*/
PosAnggaran* alokasi_array_pos(int size);

/*
    Procedure bertujuan untuk membebaskan memory array Transaksi.
    I. S. : arr (Pointer ke array yang akan dibebaskan, bisa NULL)
    F. S. : Memory yang ditunjuk arr dibebaskan.
*/
void bebaskan_array_transaksi(Transaksi *arr);

/*
    Procedure bertujuan untuk membebaskan memory array PosAnggaran.
    I. S. : arr (Pointer ke array yang akan dibebaskan, bisa NULL)
    F. S. : Memory yang ditunjuk arr dibebaskan.
*/
void bebaskan_array_pos(PosAnggaran *arr);

/*
    Function bertujuan untuk menginisialisasi struct ListTransaksi.
    Input : list (Pointer ke ListTransaksi), initial_capacity (Kapasitas awal)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int inisialisasi_list_transaksi(ListTransaksi *list, int initial_capacity);

/*
    Procedure bertujuan untuk membebaskan memory ListTransaksi.
    I. S. : list (Pointer ke ListTransaksi yang terdefinisi)
    F. S. : Memory data list dibebaskan, count dan capacity di-reset ke 0.
*/
void bebaskan_list_transaksi(ListTransaksi *list);

/*
    Function bertujuan untuk menambahkan transaksi ke ListTransaksi.
    Input : list (Pointer ke ListTransaksi), trx (Pointer ke Transaksi yang ditambahkan)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int tambah_ke_list_transaksi(ListTransaksi *list, const Transaksi *trx);

/*
    Function bertujuan untuk menginisialisasi struct ListPos.
    Input : list (Pointer ke ListPos), initial_capacity (Kapasitas awal)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int inisialisasi_list_pos(ListPos *list, int initial_capacity);

/*
    Procedure bertujuan untuk membebaskan memory ListPos.
    I. S. : list (Pointer ke ListPos yang terdefinisi)
    F. S. : Memory data list dibebaskan, count dan capacity di-reset ke 0.
*/
void bebaskan_list_pos(ListPos *list);

/*
    Function bertujuan untuk menambahkan pos anggaran ke ListPos.
    Input : list (Pointer ke ListPos), pos (Pointer ke PosAnggaran yang ditambahkan)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int tambah_ke_list_pos(ListPos *list, const PosAnggaran *pos);

/*
    Procedure bertujuan untuk menginisialisasi struct Transaksi ke nilai default.
    I. S. : trx (Pointer ke Transaksi, bisa belum diinisialisasi)
    F. S. : Field-field trx diisi dengan nilai default (0/kosong).
*/
void inisialisasi_transaksi(Transaksi *trx);

/*
    Procedure bertujuan untuk menginisialisasi struct PosAnggaran ke nilai default.
    I. S. : pos (Pointer ke PosAnggaran, bisa belum diinisialisasi)
    F. S. : Field-field pos diisi dengan nilai default.
*/
void inisialisasi_pos_anggaran(PosAnggaran *pos);

/*
    Procedure bertujuan untuk menginisialisasi struct AnalisisKeuangan ke nilai default.
    I. S. : analisis (Pointer ke AnalisisKeuangan, bisa belum diinisialisasi)
    F. S. : Field-field analisis diisi dengan nilai default.
*/
void inisialisasi_analisis_keuangan(AnalisisKeuangan *analisis);

/*
    Function bertujuan untuk mendapatkan nama bulan dalam Bahasa Indonesia.
    Input : bulan (Nomor bulan 1-12)
    Output : Mengembalikan pointer ke string nama bulan (statis), atau "Invalid" jika di luar range.
*/
const char* dapatkan_nama_bulan(int bulan);

/*
    Function bertujuan untuk mendapatkan kode bulan 2 digit.
    Input : bulan (Nomor bulan 1-12)
    Output : Mengembalikan pointer ke string kode bulan (statis), atau "00" jika di luar range.
*/
const char* dapatkan_kode_bulan(int bulan);

/*
    Function bertujuan untuk mendapatkan nama bulan singkat (3 huruf).
    Input : bulan (Nomor bulan 1-12)
    Output : Mengembalikan pointer ke string nama bulan singkat (statis).
*/
const char* dapatkan_nama_bulan_singkat(int bulan);

/*
    Function bertujuan untuk mengkonversi nama bulan ke nomor bulan.
    Input : nama (Nama bulan, case-insensitive)
    Output : Mengembalikan nomor bulan (1-12), atau 0 jika tidak valid.
*/
int nama_ke_nomor_bulan(const char *nama);

/*
    Function bertujuan untuk memvalidasi nomor bulan.
    Input : bulan (Nomor bulan yang akan divalidasi)
    Output : Mengembalikan 1 jika valid (1-12), 0 jika tidak valid.
*/
int cek_valid_bulan(int bulan);

/*
    Function bertujuan untuk mem-parse string tanggal ke komponen hari, bulan, tahun.
    Input : str (String tanggal format dd-mm-YYYY)
    Output : d (Pointer untuk menyimpan hari), m (Pointer untuk menyimpan bulan), y (Pointer untuk menyimpan tahun). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int urai_tanggal(const char *str, int *d, int *m, int *y);

/*
    Function bertujuan untuk mem-parse string tanggal ke struct Tanggal.
    Input : str (String tanggal format dd-mm-YYYY)
    Output : tgl (Pointer struct Tanggal untuk menyimpan hasil). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int urai_tanggal_struct(const char *str, Tanggal *tgl);

/*
    Procedure bertujuan untuk memformat komponen tanggal ke string dd-mm-YYYY.
    Input : d (Hari), m (Bulan), y (Tahun)
    Output : result (Buffer untuk menyimpan string tanggal terformat "dd-mm-YYYY")
*/
void format_tanggal(int d, int m, int y, char *result);

/*
    Function bertujuan untuk mendapatkan bulan saat ini dari sistem.
    Input : -
    Output : Mengembalikan bulan saat ini (1-12).
*/
int dapatkan_bulan_saat_ini(void);

/*
    Function bertujuan untuk mendapatkan tahun saat ini dari sistem.
    Input : -
    Output : Mengembalikan tahun saat ini.
*/
int dapatkan_tahun_saat_ini(void);

/*
    Function bertujuan untuk mendapatkan hari saat ini dari sistem.
    Input : -
    Output : Mengembalikan hari saat ini (1-31).
*/
int dapatkan_hari_saat_ini(void);

/*
    Function bertujuan untuk memeriksa apakah tahun adalah tahun kabisat.
    Input : year (Tahun yang akan diperiksa)
    Output : Mengembalikan 1 jika tahun kabisat, 0 jika bukan.
*/
int cek_tahun_kabisat(int year);

/*
    Function bertujuan untuk mendapatkan jumlah hari dalam bulan tertentu.
    Input : month (Bulan 1-12), year (Tahun untuk pengecekan kabisat)
    Output : Mengembalikan jumlah hari dalam bulan.
*/
int dapatkan_jumlah_hari_bulan(int month, int year);

/*
    Function bertujuan untuk mengekstrak bulan dari string tanggal.
    Input : tanggal (String tanggal format dd-mm-YYYY)
    Output : Mengembalikan nomor bulan (1-12), atau 0 jika gagal.
*/
int ekstrak_bulan(const char *tanggal);

/*
    Function bertujuan untuk mengekstrak tahun dari string tanggal.
    Input : tanggal (String tanggal format dd-mm-YYYY)
    Output : Mengembalikan tahun, atau 0 jika gagal.
*/
int ekstrak_tahun(const char *tanggal);

/*
    Procedure bertujuan untuk mendapatkan tanggal hari ini dalam format dd-mm-YYYY.
    Input : -
    Output : result (Buffer string tanggal hari ini "dd-mm-YYYY")
*/
void dapatkan_tanggal_hari_ini_string(char *result);

/*
    Procedure bertujuan untuk memformat nominal ke format Rupiah dengan separator ribuan.
    Input : nominal (Nilai nominal)
    Output : result (Buffer string format Rupiah, misal "Rp 1.000.000")
*/
void format_rupiah(unsigned long long nominal, char *result);

/*
    Procedure bertujuan untuk memformat persentase dengan 2 desimal.
    Input : persen (Nilai persentase)
    Output : result (Buffer string format persentase, misal "62.14%")
*/
void format_persentase(double persen, char *result);

/*
    Procedure bertujuan untuk memformat nilai rata-rata dengan 2 desimal.
    Input : rata (Nilai rata-rata)
    Output : result (Buffer string format rata-rata, misal "29.444,33")
*/
void format_rata_rata(double rata, char *result);

/*
    Procedure bertujuan untuk memformat angka dengan separator ribuan (tanpa prefix Rp).
    Input : nominal (Nilai yang akan diformat)
    Output : result (Buffer string angka dengan separator, misal "1.000.000")
*/
void format_angka_pemisah(unsigned long long nominal, char *result);

/*
    Procedure bertujuan untuk memformat nilai saldo (bisa negatif) ke format Rupiah.
    Input : saldo (Nilai saldo)
    Output : result (Buffer string saldo format Rupiah, misal "-Rp 100.000")
*/
void format_saldo(long long saldo, char *result);

/*
    Function bertujuan untuk mengkonversi string angka ke unsigned long long.
    Input : str (String yang berisi angka)
    Output : result (Pointer untuk menyimpan hasil konversi). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int string_ke_ull(const char *str, unsigned long long *result);

/*
    Function bertujuan untuk memeriksa apakah string hanya berisi digit.
    Input : str (String yang akan diperiksa)
    Output : Mengembalikan 1 jika hanya digit, 0 jika tidak.
*/
int cek_string_numerik(const char *str);

/*
    Procedure bertujuan untuk menghapus whitespace di awal dan akhir string.
    I. S. : str (String mungkin ada whitespace di awal/akhir)
    F. S. : str tidak lagi memiliki whitespace di awal/akhir.
*/
void pangkas_string(char *str);

/*
    Procedure bertujuan untuk mengkonversi string ke uppercase.
    I. S. : str (String berisi huruf)
    F. S. : Seluruh huruf kecil di str diubah menjadi huruf besar.
*/
void string_ke_kapital(char *str);

/*
    Procedure bertujuan untuk mengkonversi string ke lowercase.
    I. S. : str (String berisi huruf)
    F. S. : Seluruh huruf besar di str diubah menjadi huruf kecil.
*/
void string_ke_kecil(char *str);

/*
    Procedure bertujuan untuk menyalin string dengan batasan ukuran (safe copy).
    I. S. : dst (Buffer tujuan), src (String sumber), max (Ukuran maksimal buffer)
    F. S. : Isi src disalin ke dst (maksimal max-1 karakter), dst null-terminated.
*/
void salin_string_aman(char *dst, const char *src, int max);

/*
    Function bertujuan untuk memisahkan string berdasarkan delimiter.
    Input : str (String yang dipisahkan), delim (Karakter delimiter), max_parts (Jumlah maksimal bagian)
    Output : result (Array pointer menyimpan hasil split). Mengembalikan jumlah bagian yang dihasilkan.
*/
int pisah_string(char *str, char delim, char **result, int max_parts);

/*
    Function bertujuan untuk memeriksa apakah string kosong (hanya whitespace atau NULL).
    Input : str (String yang diperiksa)
    Output : Mengembalikan 1 jika kosong, 0 jika tidak.
*/
int cek_string_kosong(const char *str);

/*
    Function bertujuan untuk membandingkan dua string secara case-insensitive.
    Input : s1 (String pertama), s2 (String kedua)
    Output : Mengembalikan 0 jika sama, non-zero jika berbeda.
*/
int banding_string_tanpa_case(const char *s1, const char *s2);

/*
    Procedure bertujuan untuk menghapus karakter newline dari akhir string.
    I. S. : str (String yang mungkin ada newline di akhir)
    F. S. : Karakter newline di akhir str dihapus.
*/
void hapus_newline_string(char *str);

#endif
