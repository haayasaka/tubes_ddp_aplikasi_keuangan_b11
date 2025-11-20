#include "validasi.h"
#include "tipe_data.h"
#include <string.h>

// Cek jenis valid, nominal > 0, tanggal tidak kosong, pos ada di arrayPos
int validasiTransaksi(const Transaksi *trx, PosAnggaran *arrayPos, int jumlahPos) {
    if (!isNotEmpty(trx->tanggal)) return 0;
    if (!isValidJenisTransaksi(trx->jenis)) return 0;
    if (!isPositive(trx->nominal)) return 0;

    // Pos harus ada
    int adaPos = 0;
    for (int i = 0; i < jumlahPos; ++i) {
        if (strcmp(arrayPos[i].namaPos, trx->kategori) == 0) { adaPos = 1; break; }
    }
    return adaPos;
}
