#include <string.h>
#include "tipe_data.h"

double hitungRataRataPengeluaran(Transaksi *arrayTrx, int jumlahTrx) {
    double totalKeluar = 0.0;
    int jumlahKeluar = 0;
    for (int i = 0; i < jumlahTrx; ++i) {
        if (strcmp(arrayTrx[i].jenis, "Pengeluaran") == 0) {
            totalKeluar += arrayTrx[i].nominal;
            jumlahKeluar++;
        }
    }
    if (jumlahKeluar == 0) return 0.0;
    return totalKeluar / (double)jumlahKeluar;
}
