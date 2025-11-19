#ifndef LAPORAN_H
#define LAPORAN_H

#include "tipe_data.h"

void tampilkanSemuaTransaksi(Transaksi *arrayTrx, int jumlahTrx);
void tampilkanTransaksiPemasukan(Transaksi *arrayTrx, int jumlahTrx);
void tampilkanTransaksiPengeluaran(Transaksi *arrayTrx, int jumlahTrx);
void tampilkanRekapitulasi(Transaksi *arrayTrx, int jumlahTrx);
void tampilkanRealisasiPosAnggaran(PosAnggaran *arrayPos, int jumlahPos, Transaksi *arrayTrx, int jumlahTrx);
void tampilkanAnalisisKondisi(double saldo);
void tampilkanKesimpulanMahasiswa(double totalPemasukan, double saldo);

#endif
