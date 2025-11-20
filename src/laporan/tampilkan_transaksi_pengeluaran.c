#include <stdio.h>
#include <string.h>
#include "tipe_data.h"

void tampilkanTransaksiPengeluaran(Transaksi *arrayTrx, int jumlahTrx) {
    printf("\n=== Transaksi Pengeluaran ===\n");
    for (int i = 0; i < jumlahTrx; ++i) {
        if (strcmp(arrayTrx[i].jenis, "Pengeluaran") == 0) {
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
