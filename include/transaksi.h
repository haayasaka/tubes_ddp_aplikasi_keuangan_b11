#ifndef TRANSAKSI_H
#define TRANSAKSI_H

/* ===== KONSTANTA ===== */

/* Konfigurasi Transaksi */
#define MAX_DESKRIPSI_LENGTH    50      /* Panjang maksimal deskripsi */
#define MAX_ID_LENGTH           5       /* Panjang ID transaksi (T0001) */
#define MAX_TANGGAL_LENGTH      10      /* Format dd-mm-YYYY */
#define MAX_TRANSAKSI           1000    /* Maksimal transaksi per bulan */
#define FILE_TRANSAKSI          "transaksi"         /* Nama dasar file transaksi */
#define FILE_TRANSAKSI_NAME     "transaksi.txt"     /* Nama file transaksi */

/* Jenis Transaksi */
#define JENIS_PENGELUARAN       0       /* Kode untuk pengeluaran */
#define JENIS_PEMASUKAN         1       /* Kode untuk pemasukan */

/* ===== STRUKTUR DATA ===== */

/**
 * Struct untuk menyimpan data satu transaksi keuangan
 *
 * Menyimpan informasi lengkap tentang transaksi baik pemasukan maupun pengeluaran
 */
typedef struct Transaksi {
    char id[6];                 /* ID transaksi (T0001-T9999) */
    char tanggal[11];           /* Format dd-mm-YYYY */
    int jenis;                  /* 0=Pengeluaran, 1=Pemasukan */
    char pos[21];               /* Nama pos anggaran (max 20 char) */
    unsigned long long nominal; /* Nilai transaksi > 0 */
    char deskripsi[51];         /* Keterangan (max 50 char) */
} Transaksi;

/**
 * Struct untuk array dinamis transaksi
 *
 * Digunakan untuk mengelola koleksi transaksi dengan ukuran dinamis
 */
typedef struct ListTransaksi {
    Transaksi *data;            /* Pointer ke array transaksi */
    int count;                  /* Jumlah transaksi saat ini */
    int capacity;               /* Kapasitas maksimal array */
} ListTransaksi;

/*
    Function bertujuan untuk menambahkan transaksi baru dengan validasi.
    Input : tanggal (Tanggal transaksi), jenis (Jenis transaksi), pos (Nama pos), nominal (Nilai nominal), deskripsi (Deskripsi transaksi)
    Output : error_msg (Buffer untuk menyimpan pesan error). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int tambah_transaksi(const char *tanggal, int jenis, const char *pos,
                     unsigned long long nominal, const char *deskripsi,
                     char *error_msg);

/*
    Function bertujuan untuk mengedit transaksi dengan validasi.
    Input : id (ID transaksi), tanggal (Tanggal baru), jenis (Jenis baru), pos (Pos baru), nominal (Nominal baru), deskripsi (Deskripsi baru)
    Output : error_msg (Buffer untuk menyimpan pesan error). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int ubah_transaksi(const char *id, const char *tanggal, int jenis,
                   const char *pos, unsigned long long nominal,
                   const char *deskripsi, char *error_msg);

/*
    Function bertujuan untuk menghapus transaksi dengan validasi.
    Input : id (ID transaksi)
    Output : error_msg (Buffer untuk menyimpan pesan error). Mengembalikan 1 jika berhasil, 0 jika gagal.
*/
int hapus_transaksi(const char *id, char *error_msg);

/*
    Function bertujuan untuk mengambil daftar transaksi untuk bulan tertentu.
    Input : max_count (Kapasitas maksimal), bulan (Bulan filter 1-12, 0 untuk semua)
    Output : list (Array untuk menyimpan hasil). Mengembalikan jumlah transaksi.
*/
int ambil_daftar_transaksi(Transaksi *list, int max_count, int bulan);

/*
    Function bertujuan untuk mengambil transaksi berdasarkan ID.
    Input : id (ID transaksi)
    Output : result (Pointer untuk menyimpan hasil). Mengembalikan 1 jika ditemukan, 0 jika tidak.
*/
int ambil_transaksi_berdasarkan_id(const char *id, Transaksi *result);

/*
    Function bertujuan untuk menghitung total pemasukan untuk bulan tertentu.
    Input : bulan (Nomor bulan 1-12)
    Output : Mengembalikan total pemasukan.
*/
unsigned long long hitung_total_pemasukan_transaksi(int bulan);

/*
    Function bertujuan untuk menghitung total pengeluaran untuk bulan tertentu.
    Input : bulan (Nomor bulan 1-12)
    Output : Mengembalikan total pengeluaran.
*/
unsigned long long hitung_total_pengeluaran_transaksi(int bulan);

/*
    Function bertujuan untuk menghitung jumlah transaksi untuk bulan tertentu.
    Input : bulan (Nomor bulan 1-12), jenis (Jenis transaksi, -1 untuk semua)
    Output : Mengembalikan jumlah transaksi.
*/
int hitung_jumlah_transaksi(int bulan, int jenis);

/*
    Function bertujuan untuk menampilkan daftar transaksi bulan.
    Input : bulan (Nomor bulan), selected (Index terpilih)
    Output : Mengembalikan posisi baris berikutnya setelah tabel.
*/
int tampilkan_daftar_transaksi(int bulan, int selected);

/*
    Function bertujuan untuk menampilkan detail satu transaksi.
    Input : trx (Pointer ke transaksi), start_y (Baris awal)
    Output : Mengembalikan posisi baris berikutnya setelah detail.
*/
int tampilkan_detail_transaksi(Transaksi *trx, int start_y);

/*
    Function bertujuan untuk menampilkan ringkasan transaksi bulan.
    Input : bulan (Nomor bulan), start_y (Baris awal)
    Output : Mengembalikan posisi baris berikutnya setelah ringkasan.
*/
int tampilkan_ringkasan_transaksi(int bulan, int start_y);

/*
    Function bertujuan untuk menampilkan form tambah transaksi.
    Input : bulan (Bulan default)
    Output : Mengembalikan 1 jika berhasil tambah, 0 jika dibatalkan.
*/
int tampilkan_form_tambah_transaksi(int bulan);

/*
    Function bertujuan untuk menampilkan form edit transaksi.
    Input : id (ID transaksi)
    Output : Mengembalikan 1 jika berhasil edit, 0 jika dibatalkan.
*/
int tampilkan_form_edit_transaksi(const char *id);

/*
    Function bertujuan untuk menampilkan konfirmasi hapus transaksi.
    Input : id (ID transaksi)
    Output : Mengembalikan 1 jika berhasil hapus, 0 jika dibatalkan.
*/
int tampilkan_konfirmasi_hapus_transaksi(const char *id);

/*
    Procedure bertujuan untuk menjalankan modul transaksi.
    I. S. : bulan_awal (Bulan awal saat modul dijalankan, valid)
    F. S. : Modul transaksi dijalankan hingga pengguna kembali.
*/
void jalankan_modul_transaksi(int bulan_awal);

/*
    Function bertujuan untuk menampilkan menu utama transaksi.
    Input : bulan (Bulan aktif)
    Output : Mengembalikan kode aksi yang dipilih.
*/
int menu_transaksi_utama(int bulan);

/*
    Procedure bertujuan untuk menangani tampilan daftar transaksi.
    I. S. : bulan (Nomor bulan valid)
    F. S. : Pengguna kembali dari tampilan.
*/
void penanganan_lihat_transaksi(int bulan);

/*
    Procedure bertujuan untuk menangani tambah transaksi.
    I. S. : bulan (Nomor bulan valid)
    F. S. : Transaksi baru ditambahkan jika pengguna konfirmasi.
*/
void penanganan_tambah_transaksi(int bulan);

/*
    Procedure bertujuan untuk menangani edit transaksi.
    I. S. : bulan (Nomor bulan valid)
    F. S. : Transaksi diedit jika pengguna konfirmasi.
*/
void penanganan_ubah_transaksi(int bulan);

/*
    Procedure bertujuan untuk menangani hapus transaksi.
    I. S. : bulan (Nomor bulan valid)
    F. S. : Transaksi dihapus jika pengguna konfirmasi.
*/
void penanganan_hapus_transaksi(int bulan);

#endif
