#include <string.h>
#include "tipe_data.h"

// Mengisi array pengeluaran per pos (outPengeluaranPerPos) sesuai urutan arrayPos
void hitungRealisasiAnggaran(PosAnggaran *arrayPos, int jumlahPos, Transaksi *arrayTrx, int jumlahTrx, double *outPengeluaranPerPos) {
    for (int i = 0; i < jumlahPos; ++i) outPengeluaranPerPos[i] = 0.0;

    for (int i = 0; i < jumlahPos; ++i) {
        for (int j = 0; j < jumlahTrx; ++j) {
            if (strcmp(arrayTrx[j].jenis, "Pengeluaran") == 0 &&
                strcmp(arrayTrx[j].kategori, arrayPos[i].namaPos) == 0) {
                outPengeluaranPerPos[i] += arrayTrx[j].nominal;
            }
        }
    }
}
