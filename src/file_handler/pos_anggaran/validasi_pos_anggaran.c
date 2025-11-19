#include "validasi.h"
#include "tipe_data.h"

// Pastikan nama unik dan batas > 0
int validasiPosAnggaran(const char *nama, double batas, PosAnggaran *arrayPos, int jumlahPos) {
    if (!isNotEmpty(nama)) return 0;
    if (!isPositive(batas)) return 0;
    if (!isUniquePos(nama, arrayPos, jumlahPos)) return 0;
    return 1;
}
