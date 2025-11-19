#ifndef PERHITUNGAN_H
#define PERHITUNGAN_H

#include "tipe_data.h"

double hitungTotalPemasukan(const Transaksi *arrayTrx, int jumlahTrx);
double hitungTotalPengeluaran(Transaksi *arrayTrx, int jumlahTrx);
double hitungSaldo(double totalMasuk, double totalKeluar);
double hitungRataRataPengeluaran(Transaksi *arrayTrx, int jumlahTrx);
void   hitungRealisasiAnggaran(PosAnggaran *arrayPos, int jumlahPos, Transaksi *arrayTrx, int jumlahTrx, double *outPengeluaranPerPos);

const char* analisisKondisiKeuangan(double saldo);
const char* kesimpulanMahasiswa(double totalPemasukan, double saldo);

#endif
