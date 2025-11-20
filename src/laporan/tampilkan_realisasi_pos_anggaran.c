#include <stdio.h>
#include "tipe_data.h"
#include "perhitungan.h"

void tampilkanRealisasiPosAnggaran(PosAnggaran *arrayPos, int jumlahPos, Transaksi *arrayTrx, int jumlahTrx) {
    double realisasi[MAKS_POS];
    hitungRealisasiAnggaran(arrayPos, jumlahPos, arrayTrx, jumlahTrx, realisasi);

    printf("\n=== Realisasi Pos Anggaran ===\n");
    printf("Pos                | Batas       | Pengeluaran | Sisa\n");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < jumlahPos; ++i) {
        double sisa = arrayPos[i].batasNominal - realisasi[i];
        printf("%-18s | %11.2f | %11.2f | %11.2f\n",
               arrayPos[i].namaPos, arrayPos[i].batasNominal, realisasi[i], sisa);
    }
}
