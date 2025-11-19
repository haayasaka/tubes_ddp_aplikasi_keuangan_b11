#include <stdio.h>

// Menambahkan satu baris ke akhir file teks
int tulisFile(const char *namaFile, const char *baris) {
    FILE *f = fopen(namaFile, "a");
    if (!f) return 0;
    fprintf(f, "%s\n", baris);
    fclose(f);
    return 1;
}
