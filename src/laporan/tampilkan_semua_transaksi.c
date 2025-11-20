#include <stdio.h>
#include <string.h>
#include "tipe_data.h"

void tampilkanSemuaTransaksi(Transaksi *arrayTrx, int jumlahTrx) {
    printf("\n=== Semua Transaksi ===\n");
    printf("ID   | Tanggal     | Jenis       | Kategori                | Nominal     | Deskripsi\n");
    printf("-------------------------------------------------------------------------------------\n");
    for (int i = 0; i < jumlahTrx; ++i) {
        printf("%-4s | %-11s | %-11s | %-22s | %11.2f | %s\n",
            arrayTrx[i].kodeTransaksi,
            arrayTrx[i].tanggal,
            arrayTrx[i].jenis,
            arrayTrx[i].kategori,
            arrayTrx[i].nominal,
            arrayTrx[i].deskripsi
        );
    }
}
