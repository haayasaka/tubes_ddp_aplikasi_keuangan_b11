#include "tipe_data.h"

int validasiHapusPos(int kodePos, PosAnggaran *arrayPos, int jumlahPos) {
    for (int i = 0; i < jumlahPos; ++i) {
        if (arrayPos[i].kodePos == kodePos) return 1;
    }
    return 0;
}
