#include <string.h>
#include "tipe_data.h"

double hitungTotalPengeluaran(Transaksi *arrayTrx, int jumlahTrx) {
    double total = 0.0;
    for (int i = 0; i < jumlahTrx; ++i) {
        if (strcmp(arrayTrx[i].jenis, "Pengeluaran") == 0) total += arrayTrx[i].nominal;
    }
    return total;
}
