#include <stdio.h>
#include "tipe_data.h"

// Simpan satu transaksi secara append ke data_keuangan.txt
int simpanTransaksi(const Transaksi *trx, const char *namaFile) {
    FILE *f = fopen(namaFile, "a");
    if (!f) return 0;
    fprintf(f, "%s|%s|%s|%s|%.2f|%s\n",
        trx->kodeTransaksi, trx->tanggal, trx->jenis, trx->kategori, trx->nominal, trx->deskripsi);
    fclose(f);
    return 1;
}
