#include "tipe_data.h"

// Menghapus pos dari array (geser elemen), mengubah jumlahPos
int hapusPosAnggaran(int kodePos, PosAnggaran *arrayPos, int *jumlahPos) {
    int idx = -1;
    for (int i = 0; i < *jumlahPos; ++i) {
        if (arrayPos[i].kodePos == kodePos) { idx = i; break; }
    }
    if (idx == -1) return 0;

    for (int j = idx; j < *jumlahPos - 1; ++j) {
        arrayPos[j] = arrayPos[j + 1];
    }
    (*jumlahPos)--;
    return 1;
}
