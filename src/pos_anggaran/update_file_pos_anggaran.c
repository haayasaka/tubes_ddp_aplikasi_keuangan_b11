#include <stdio.h>
#include "tipe_data.h"

// Menulis ulang seluruh array pos ke file (sinkronisasi setelah edit/hapus)
int updateFilePosAnggaran(PosAnggaran *arrayPos, int jumlahPos, const char *namaFile) {
    FILE *f = fopen(namaFile, "w");
    if (!f) return 0;
    for (int i = 0; i < jumlahPos; ++i) {
        fprintf(f, "%d|%s|%.2f\n", arrayPos[i].kodePos, arrayPos[i].namaPos, arrayPos[i].batasNominal);
    }
    fclose(f);
    return 1;
}
