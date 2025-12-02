/**
 * @file pos.h
 * @brief Header untuk modul pos anggaran
 * @author Kelompok B11
 * @date 2025
 */

#ifndef POS_H
#define POS_H

// Forward declaration to avoid circular dependency if any
typedef struct Transaksi Transaksi; 

/**
 * @brief Struct untuk menyimpan data pos anggaran beserta realisasinya
 * 
 * Digunakan untuk tracking budget dan pengeluaran aktual per kategori
 */
typedef struct PosAnggaran {
    int no;                     /* Nomor urut pos */
    char nama[21];              /* Nama pos (max 20 karakter) */
    unsigned long long nominal; /* Batas anggaran */
    unsigned long long realisasi; /* Total pengeluaran aktual */
    long long sisa;             /* Sisa = nominal - realisasi */
    int jumlah_transaksi;       /* Banyak transaksi pada pos */
    int status;                 /* 1=Aman, 0=Tidak Aman */
} PosAnggaran;

/**
 * @brief Struct untuk array dinamis pos anggaran
 * 
 * Digunakan untuk mengelola koleksi pos anggaran dengan ukuran dinamis
 */
typedef struct ListPos {
    PosAnggaran *data;          /* Pointer ke array pos anggaran */
    int count;                  /* Jumlah pos saat ini */
    int capacity;               /* Kapasitas maksimal array */
} ListPos;

/* ===== KONSTANTA POS ===== */
#define MAX_POS_LENGTH          20      /* Panjang maksimal nama pos */
#define MAX_POS                 50      /* Maksimal pos anggaran */
#define FILE_POS                "pos_anggaran"      /* Nama dasar file pos */
#define FILE_POS_PREFIX         "pos"               /* Prefix file pos */

/* ===== STATUS POS ANGGARAN ===== */
#define STATUS_TIDAK_AMAN       0       /* Realisasi > Nominal */
#define STATUS_AMAN             1       /* Realisasi <= Nominal */

/* --- pos_calc.h --- */

/*
    Function bertujuan untuk menghitung realisasi pos dari transaksi.
    Input : nama_pos (nama pos), bulan (bulan transaksi)
    Output : Mengembalikan total realisasi (pengeluaran) untuk pos tersebut.
*/
unsigned long long hitung_realisasi_pos(const char *nama_pos, int bulan);

/*
    Function bertujuan untuk menghitung sisa anggaran pos.
    Input : nominal (anggaran awal), realisasi (realisasi pengeluaran)
    Output : Mengembalikan sisa anggaran (bisa negatif).
*/
long long hitung_sisa_pos(unsigned long long nominal, unsigned long long realisasi);

/*
    Function bertujuan untuk menentukan status pos berdasarkan sisa.
    Input : sisa (sisa anggaran)
    Output : Mengembalikan STATUS_AMAN atau STATUS_TIDAK_AMAN.
*/
int tentukan_status_pos(long long sisa);

/*
    Procedure bertujuan untuk mengupdate kalkulasi untuk satu pos.
    I. S. : pos (pointer ke pos yang akan diupdate)
    F. S. : Field realisasi, sisa, status, dan jumlah_transaksi pada pos diupdate.
*/
void update_kalkulasi_pos(PosAnggaran *pos, int bulan);

/*
    Function bertujuan untuk mengupdate kalkulasi untuk semua pos dalam bulan.
    Input : bulan (bulan untuk kalkulasi)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int update_semua_pos_bulan(int bulan);

/*
    Function bertujuan untuk menghitung jumlah transaksi untuk pos dalam bulan.
    Input : nama_pos (nama pos), bulan (1-12)
    Output : Mengembalikan jumlah transaksi.
*/
int hitung_jumlah_transaksi_pos(const char *nama_pos, int bulan);

/*
    Function bertujuan untuk menghitung total anggaran semua pos dalam bulan.
    Input : bulan (1-12)
    Output : Mengembalikan total anggaran.
*/
unsigned long long hitung_total_anggaran(int bulan);

/*
    Function bertujuan untuk menghitung total realisasi semua pos dalam bulan.
    Input : bulan (1-12)
    Output : Mengembalikan total realisasi.
*/
unsigned long long hitung_total_realisasi(int bulan);

/* --- pos_crud.h --- */

/*
    Function bertujuan untuk menambahkan pos anggaran baru dengan validasi.
    Input : nama (nama pos), nominal (anggaran), bulan (1-12)
    Output : error_msg (menyimpan pesan error via pointer). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int pos_tambah(const char *nama, unsigned long long nominal, int bulan, char *error_msg);

/*
    Function bertujuan untuk mengedit pos anggaran dengan validasi.
    Input : no (nomor pos), nama_baru, nominal_baru, bulan
    Output : error_msg (menyimpan pesan error via pointer). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int pos_edit(int no, const char *nama_baru, unsigned long long nominal_baru, int bulan, char *error_msg);

/*
    Function bertujuan untuk menghapus pos anggaran dengan validasi.
    Input : no (nomor pos), bulan
    Output : error_msg (menyimpan pesan error via pointer). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int pos_hapus(int no, int bulan, char *error_msg);

/*
    Function bertujuan untuk mengambil daftar pos untuk bulan dengan kalkulasi terbaru.
    Input : max_count (kapasitas maksimal), bulan (1-12)
    Output : list (array menyimpan pos). Mengembalikan jumlah pos.
*/
int pos_get_list(PosAnggaran *list, int max_count, int bulan);

/*
    Function bertujuan untuk mengambil satu pos berdasarkan nomor dengan kalkulasi terbaru.
    Input : no (nomor pos), bulan (1-12)
    Output : result (pointer menyimpan hasil). Mengembalikan 1 jika ditemukan, 0 jika tidak.
*/
int pos_get_by_no(int no, int bulan, PosAnggaran *result);

/*
    Function bertujuan untuk mengambil daftar nama pos untuk dropdown/pilihan.
    Input : max_count (kapasitas maksimal), bulan (1-12)
    Output : names (array string menyimpan nama). Mengembalikan jumlah pos.
*/
int pos_get_names(char names[][MAX_POS_LENGTH + 1], int max_count, int bulan);

/*
    Function bertujuan untuk memeriksa apakah pos dengan nama tertentu sudah ada.
    Input : nama (nama pos), bulan (1-12)
    Output : Mengembalikan 1 jika ada, 0 jika tidak.
*/
int pos_exists(const char *nama, int bulan);

/*
    Function bertujuan untuk me-refresh kalkulasi semua pos untuk bulan.
    Input : bulan (1-12)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int pos_refresh(int bulan);

/* --- pos_display.h --- */

/*
    Function bertujuan untuk menampilkan daftar pos anggaran dengan kalkulasi.
    Input : bulan, selected (index terpilih)
    Output : Mengembalikan posisi baris berikutnya setelah tabel.
*/
int tampilkan_daftar_pos(int bulan, int selected);

/*
    Function bertujuan untuk menampilkan detail satu pos anggaran.
    Input : pos (pointer ke pos), start_y (baris awal)
    Output : Mengembalikan posisi baris berikutnya setelah detail.
*/
int tampilkan_detail_pos(PosAnggaran *pos, int start_y);

/*
    Function bertujuan untuk menampilkan ringkasan pos anggaran bulan.
    Input : bulan, start_y (baris awal)
    Output : Mengembalikan posisi baris berikutnya setelah ringkasan.
*/
int tampilkan_ringkasan_pos(int bulan, int start_y);

/*
    Function bertujuan untuk menampilkan form tambah pos.
    Input : bulan (untuk pos baru)
    Output : Mengembalikan 1 jika berhasil tambah, 0 jika dibatalkan.
*/
int tampilkan_form_tambah_pos(int bulan);

/*
    Function bertujuan untuk menampilkan form edit pos.
    Input : no (nomor pos), bulan
    Output : Mengembalikan 1 jika berhasil edit, 0 jika dibatalkan.
*/
int tampilkan_form_edit_pos(int no, int bulan);

/*
    Function bertujuan untuk menampilkan konfirmasi hapus pos.
    Input : no (nomor pos), bulan
    Output : Mengembalikan 1 jika berhasil hapus, 0 jika dibatalkan.
*/
int tampilkan_konfirmasi_hapus_pos(int no, int bulan);

/*
    Function bertujuan untuk menampilkan pilihan pos untuk input transaksi.
    Input : bulan
    Output : result (buffer menyimpan nama pos terpilih). Mengembalikan 1 jika dipilih, 0 jika dibatalkan.
*/
int tampilkan_pilih_pos(int bulan, char *result);

/* --- pos_handler.h --- */

/*
    Procedure bertujuan untuk menjalankan modul pos anggaran.
    I. S. : bulan_awal valid
    F. S. : Modul pos dijalankan hingga user kembali.
*/
void run_pos_module(int bulan_awal);

/*
    Function bertujuan untuk menginisialisasi data pos untuk bulan baru.
    Input : bulan (yang akan diinisialisasi)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int init_pos_for_bulan(int bulan);

/*
    Function bertujuan untuk menyinkronisasi kalkulasi pos dengan transaksi.
    Input : bulan (yang akan disinkronisasi)
    Output : Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int sync_pos_transaksi(int bulan);

/* --- pos_menu.h --- */

/*
    Function bertujuan untuk menampilkan dan menjalankan menu utama pos anggaran.
    Input : bulan (bulan aktif)
    Output : Mengembalikan kode aksi yang dipilih.
*/
int menu_pos_utama(int bulan);

/*
    Function bertujuan untuk menampilkan menu pilih bulan.
    Input : bulan_saat_ini
    Output : Mengembalikan bulan yang dipilih, atau 0 jika dibatalkan.
*/
int menu_pilih_bulan(int bulan_saat_ini);

/*
    Procedure bertujuan untuk menangani view daftar pos dengan navigasi.
    I. S. : bulan valid
    F. S. : User kembali dari view.
*/
void handler_view_pos(int bulan);

/*
    Procedure bertujuan untuk menangani tambah pos baru.
    I. S. : bulan valid
    F. S. : Pos baru ditambahkan jika user konfirmasi.
*/
void handler_tambah_pos(int bulan);

/*
    Procedure bertujuan untuk menangani edit pos.
    I. S. : bulan valid
    F. S. : Pos diedit jika user konfirmasi.
*/
void handler_edit_pos(int bulan);

/*
    Procedure bertujuan untuk menangani hapus pos.
    I. S. : bulan valid
    F. S. : Pos dihapus jika user konfirmasi.
*/
void handler_hapus_pos(int bulan);

#endif /* POS_H */
