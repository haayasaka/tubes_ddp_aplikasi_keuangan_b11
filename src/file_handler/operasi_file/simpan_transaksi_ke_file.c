#include <stdio.h>
#include "tipe_data.h"

// Menulis seluruh transaksi ke file dengan format: ID|Tanggal|Jenis|Kategori|Nominal|Deskripsi
int simpanTransaksiKeFile(Transaksi *arrayTransaksi, int jumlah, const char *namaFile) {
    FILE *f = fopen(namaFile, "w");
    if (!f) return 0;
    for (int i = 0; i < jumlah; ++i) {
        fprintf(f, "%s|%s|%s|%s|%.2f|%s\n",
            arrayTransaksi[i].kodeTransaksi,
            arrayTransaksi[i].tanggal,
            arrayTransaksi[i].jenis,
            arrayTransaksi[i].kategori,
            arrayTransaksi[i].nominal,
            arrayTransaksi[i].deskripsi
        );
    }
    fclose(f);
    return 1;
}
