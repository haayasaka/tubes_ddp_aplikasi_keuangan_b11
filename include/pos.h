/**
 * @file pos.h
 * @brief Header untuk modul pos anggaran
 * @author Kelompok B11
 * @date 2025
 */

#ifndef POS_H
#define POS_H

#include "types.h"
#include "constants.h"

/* --- pos_calc.h --- */

/**
 * @brief Hitung realisasi pos dari transaksi
 * @param nama_pos Nama pos yang dihitung
 * @param bulan Bulan transaksi (1-12)
 * @return Total realisasi (pengeluaran) untuk pos
 */
unsigned long long hitung_realisasi_pos(const char *nama_pos, int bulan);

/**
 * @brief Hitung sisa anggaran pos
 * @param nominal Anggaran awal
 * @param realisasi Realisasi pengeluaran
 * @return Sisa anggaran (bisa negatif)
 */
long long hitung_sisa_pos(unsigned long long nominal, unsigned long long realisasi);

/**
 * @brief Tentukan status pos berdasarkan sisa
 * @param sisa Sisa anggaran
 * @return STATUS_AMAN atau STATUS_TIDAK_AMAN
 */
int tentukan_status_pos(long long sisa);

/**
 * @brief Update kalkulasi untuk satu pos
 * @param pos Pointer ke pos yang akan diupdate
 * @param bulan Bulan untuk kalkulasi
 */
void update_kalkulasi_pos(PosAnggaran *pos, int bulan);

/**
 * @brief Update kalkulasi untuk semua pos dalam bulan
 * @param bulan Bulan untuk kalkulasi
 * @return 1 jika berhasil, 0 jika gagal
 */
int update_semua_pos_bulan(int bulan);

/**
 * @brief Hitung jumlah transaksi untuk pos dalam bulan
 * @param nama_pos Nama pos
 * @param bulan Bulan (1-12)
 * @return Jumlah transaksi
 */
int hitung_jumlah_transaksi_pos(const char *nama_pos, int bulan);

/**
 * @brief Hitung total anggaran semua pos dalam bulan
 * @param bulan Bulan (1-12)
 * @return Total anggaran
 */
unsigned long long hitung_total_anggaran(int bulan);

/**
 * @brief Hitung total realisasi semua pos dalam bulan
 * @param bulan Bulan (1-12)
 * @return Total realisasi
 */
unsigned long long hitung_total_realisasi(int bulan);

/* --- pos_crud.h --- */

/**
 * @brief Tambah pos anggaran baru dengan validasi
 * @param nama Nama pos
 * @param nominal Anggaran pos
 * @param bulan Bulan (1-12)
 * @param error_msg Buffer untuk pesan error (bisa NULL)
 * @return 1 jika berhasil, 0 jika gagal
 */
int pos_tambah(const char *nama, unsigned long long nominal, int bulan, char *error_msg);

/**
 * @brief Edit pos anggaran dengan validasi
 * @param no Nomor pos yang diedit
 * @param nama_baru Nama baru (bisa NULL jika tidak diubah)
 * @param nominal_baru Anggaran baru (0 jika tidak diubah)
 * @param bulan Bulan (1-12)
 * @param error_msg Buffer untuk pesan error (bisa NULL)
 * @return 1 jika berhasil, 0 jika gagal
 */
int pos_edit(int no, const char *nama_baru, unsigned long long nominal_baru, int bulan, char *error_msg);

/**
 * @brief Hapus pos anggaran dengan validasi
 * @param no Nomor pos yang dihapus
 * @param bulan Bulan (1-12)
 * @param error_msg Buffer untuk pesan error (bisa NULL)
 * @return 1 jika berhasil, 0 jika gagal
 */
int pos_hapus(int no, int bulan, char *error_msg);

/**
 * @brief Ambil daftar pos untuk bulan dengan kalkulasi terbaru
 * @param list Array untuk menyimpan pos
 * @param max_count Kapasitas maksimal
 * @param bulan Bulan (1-12)
 * @return Jumlah pos
 */
int pos_get_list(PosAnggaran *list, int max_count, int bulan);

/**
 * @brief Ambil satu pos berdasarkan nomor dengan kalkulasi terbaru
 * @param no Nomor pos
 * @param bulan Bulan (1-12)
 * @param result Pointer untuk menyimpan hasil
 * @return 1 jika ditemukan, 0 jika tidak
 */
int pos_get_by_no(int no, int bulan, PosAnggaran *result);

/**
 * @brief Ambil daftar nama pos untuk dropdown/pilihan
 * @param names Array string untuk nama
 * @param max_count Kapasitas maksimal
 * @param bulan Bulan (1-12)
 * @return Jumlah pos
 */
int pos_get_names(char names[][MAX_POS_LENGTH + 1], int max_count, int bulan);

/**
 * @brief Cek apakah pos dengan nama tertentu sudah ada
 * @param nama Nama pos
 * @param bulan Bulan (1-12)
 * @return 1 jika ada, 0 jika tidak
 */
int pos_exists(const char *nama, int bulan);

/**
 * @brief Refresh kalkulasi semua pos untuk bulan
 * @param bulan Bulan (1-12)
 * @return 1 jika berhasil, 0 jika gagal
 */
int pos_refresh(int bulan);

/* --- pos_display.h --- */

/**
 * @brief Tampilkan daftar pos anggaran dengan kalkulasi
 * @param bulan Bulan yang ditampilkan
 * @param selected Index pos terpilih (-1 jika tidak ada)
 * @return Baris berikutnya setelah tabel
 */
int tampilkan_daftar_pos(int bulan, int selected);

/**
 * @brief Tampilkan detail satu pos anggaran
 * @param pos Pointer ke pos yang ditampilkan
 * @param start_y Baris awal tampilan
 * @return Baris berikutnya setelah detail
 */
int tampilkan_detail_pos(PosAnggaran *pos, int start_y);

/**
 * @brief Tampilkan ringkasan pos anggaran bulan
 * @param bulan Bulan yang ditampilkan
 * @param start_y Baris awal tampilan
 * @return Baris berikutnya setelah ringkasan
 */
int tampilkan_ringkasan_pos(int bulan, int start_y);

/**
 * @brief Tampilkan form tambah pos
 * @param bulan Bulan untuk pos baru
 * @return 1 jika berhasil tambah, 0 jika dibatalkan
 */
int tampilkan_form_tambah_pos(int bulan);

/**
 * @brief Tampilkan form edit pos
 * @param no Nomor pos yang diedit
 * @param bulan Bulan pos
 * @return 1 jika berhasil edit, 0 jika dibatalkan
 */
int tampilkan_form_edit_pos(int no, int bulan);

/**
 * @brief Tampilkan konfirmasi hapus pos
 * @param no Nomor pos yang akan dihapus
 * @param bulan Bulan pos
 * @return 1 jika berhasil hapus, 0 jika dibatalkan
 */
int tampilkan_konfirmasi_hapus_pos(int no, int bulan);

/**
 * @brief Tampilkan pilihan pos untuk input transaksi
 * @param bulan Bulan pos
 * @param result Buffer untuk menyimpan nama pos terpilih
 * @return 1 jika dipilih, 0 jika dibatalkan
 */
int tampilkan_pilih_pos(int bulan, char *result);

/* --- pos_handler.h --- */

/**
 * @brief Jalankan modul pos anggaran
 * @param bulan_awal Bulan awal yang ditampilkan (1-12)
 * 
 * Entry point utama untuk modul pos anggaran.
 * Menampilkan menu dan menangani semua operasi pos.
 */
void run_pos_module(int bulan_awal);

/**
 * @brief Inisialisasi data pos untuk bulan baru
 * @param bulan Bulan yang akan diinisialisasi
 * @return 1 jika berhasil, 0 jika gagal
 * 
 * Akan membuat file pos kosong jika belum ada,
 * atau menawarkan copy dari bulan sebelumnya.
 */
int init_pos_for_bulan(int bulan);

/**
 * @brief Sinkronisasi kalkulasi pos dengan transaksi
 * @param bulan Bulan yang akan disinkronisasi
 * @return 1 jika berhasil, 0 jika gagal
 * 
 * Mengupdate realisasi, sisa, dan status semua pos
 * berdasarkan transaksi di bulan tersebut.
 */
int sync_pos_transaksi(int bulan);

/* --- pos_menu.h --- */

/**
 * @brief Tampilkan dan jalankan menu utama pos anggaran
 * @param bulan Bulan yang aktif
 * @return Kode aksi yang dipilih
 */
int menu_pos_utama(int bulan);

/**
 * @brief Tampilkan menu pilih bulan
 * @param bulan_saat_ini Bulan saat ini
 * @return Bulan yang dipilih, atau 0 jika dibatalkan
 */
int menu_pilih_bulan(int bulan_saat_ini);

/**
 * @brief Handler untuk view daftar pos dengan navigasi
 * @param bulan Bulan yang ditampilkan
 */
void handler_view_pos(int bulan);

/**
 * @brief Handler untuk tambah pos baru
 * @param bulan Bulan untuk pos baru
 */
void handler_tambah_pos(int bulan);

/**
 * @brief Handler untuk edit pos
 * @param bulan Bulan pos
 */
void handler_edit_pos(int bulan);

/**
 * @brief Handler untuk hapus pos
 * @param bulan Bulan pos
 */
void handler_hapus_pos(int bulan);

#endif /* POS_H */
