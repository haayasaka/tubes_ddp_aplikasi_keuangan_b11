#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "tipe_data.h"

// Abstraksi operasi file umum
int tulisFile(const char *namaFile, const char *baris);
int bacaFile(const char *namaFile, char *buffer, int ukuranBuffer);

// Update (penyimpanan batch transaksi)
int simpanTransaksiKeFile(Transaksi *arrayTransaksi, int jumlah, const char *namaFile);
int bacaTransaksiDariFile(Transaksi *arrayTransaksi, int kapasitas, const char *namaFile);

#endif
