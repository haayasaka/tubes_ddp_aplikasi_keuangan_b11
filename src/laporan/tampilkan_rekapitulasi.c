#include <stdio.h>
#include "tipe_data.h"
#include "perhitungan.h"

void tampilkanRekapitulasi(Transaksi *arrayTrx, int jumlahTrx) {
    double totalMasuk = hitungTotalPemasukan(arrayTrx, jumlahTrx);
    double totalKeluar = hitungTotalPengeluaran(arrayTrx, jumlahTrx);
    double saldo = hitungSaldo(totalMasuk, totalKeluar);
    double rataKeluar = hitungRataRataPengeluaran(arrayTrx, jumlahTrx);

    printf("\n=== Rekapitulasi ===\n");
    printf("Total Pemasukan   : %.2f\n", totalMasuk);
    printf("Total Pengeluaran : %.2f\n", totalKeluar);
    printf("Saldo             : %.2f\n", saldo);
    printf("Rata-rata Pengeluaran: %.2f\n", rataKeluar);
}
