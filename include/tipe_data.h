#ifndef TIPE_DATA_H
#define TIPE_DATA_H

#define MAKS_POS 200
#define MAKS_TRANSAKSI 1000

// Menggunakan string tetap untuk memudahkan I/O teks
typedef struct {
    int kodePos;                 // Kode numerik internal (unik)
    char namaPos[64];            // Nama kategori pos anggaran
    double batasNominal;         // Batas anggaran untuk pos
} PosAnggaran;

typedef struct {
    char kodeTransaksi[8];       // Format: T001, T002, ...
    char tanggal[16];            // Format bebas: YYYY-MM-DD disarankan
    char jenis[16];              // "Pemasukan" atau "Pengeluaran"
    char kategori[64];           // Nama pos anggaran terkait
    double nominal;              // Nilai uang
    char deskripsi[128];         // Catatan singkat
} Transaksi;

#endif
