#include "tipe_data.h"
#include <string.h>

// Edit data pos di array
int editPosAnggaran(int kodePos, const char *namaBaru, double batasBaru, PosAnggaran *arrayPos, int jumlahPos) {
    for (int i = 0; i < jumlahPos; ++i) {
        if (arrayPos[i].kodePos == kodePos) {
            strncpy(arrayPos[i].namaPos, namaBaru, sizeof(arrayPos[i].namaPos) - 1);
            arrayPos[i].namaPos[sizeof(arrayPos[i].namaPos) - 1] = '\0';
            arrayPos[i].batasNominal = batasBaru;
            return 1;
        }
    }
    return 0;
}
