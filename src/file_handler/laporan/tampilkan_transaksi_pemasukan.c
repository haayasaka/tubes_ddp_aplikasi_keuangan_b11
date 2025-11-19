#include <stdio.h>
#include <string.h>
#include "tipe_data.h"

void tampilkanTransaksiPemasukan(Transaksi *arrayTrx, int jumlahTrx) {
    printf("\n=== Transaksi Pemasukan ===\n");
    for (int i = 0; i < jumlahTrx; ++i) {
        if (strcmp(arrayTrx[i].jenis, "Pemasukan") == 0) {
            printf("%s | %s | %s | %.2f | %s\n",
                arrayTrx[i].kodeTransaksi,
                arrayTrx[i].tanggal,
                arrayTrx[i].kategori,
                arrayTrx[i].nominal,
                arrayTrx[i].deskripsi
            );
        }
    }
}
