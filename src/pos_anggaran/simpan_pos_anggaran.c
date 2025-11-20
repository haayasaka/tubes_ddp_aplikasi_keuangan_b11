#include <stdio.h>
#include "tipe_data.h"

// Menulis data pos ke file pos_anggaran.txt (format sederhana: kode|nama|batas)
int simpanPosAnggaran(PosAnggaran pos, const char *namaFile) {
    FILE *f = fopen(namaFile, "a");
    if (!f) return 0;
    fprintf(f, "%d|%s|%.2f\n", pos.kodePos, pos.namaPos, pos.batasNominal);
    fclose(f);
    return 1;
}
