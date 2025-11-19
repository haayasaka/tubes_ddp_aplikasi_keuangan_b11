#ifndef TRANSAKSI_H
#define TRANSAKSI_H

#include "tipe_data.h"

Transaksi inputTransaksi(void);
int validasiTransaksi(const Transaksi *trx, PosAnggaran *arrayPos, int jumlahPos);
void generateKodeTransaksi(char *outKode, int nomorBerikutnya);
int simpanTransaksi(const Transaksi *trx, const char *namaFile);

#endif
