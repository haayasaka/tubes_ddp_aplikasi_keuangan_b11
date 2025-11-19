#include "validasi.h"
#include "tipe_data.h"
#include <string.h>

// Nama baru tidak boleh duplikat (kecuali kalau tetap pada pos yg sama), batas > 0
int validasiEditPos(const char *namaBaru, double batasBaru, PosAnggaran *arrayPos, int jumlahPos, int kodePos) {
    if (!isNotEmpty(namaBaru)) return 0;
    if (!isPositive(batasBaru)) return 0;
    // Cek duplikasi nama selain dirinya
    for (int i = 0; i < jumlahPos; ++i) {
        if (arrayPos[i].kodePos != kodePos && strcmp(arrayPos[i].namaPos, namaBaru) == 0) {
            return 0;
        }
    }
    return 1;
}
