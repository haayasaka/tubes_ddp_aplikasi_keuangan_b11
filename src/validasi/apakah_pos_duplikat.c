#include <string.h>
#include "tipe_data.h"

int isUniquePos(const char *nama, PosAnggaran *arrayPos, int jumlahPos) {
    for (int i = 0; i < jumlahPos; ++i) {
        if (strcmp(arrayPos[i].namaPos, nama) == 0) return 0;
    }
    return 1;
}
