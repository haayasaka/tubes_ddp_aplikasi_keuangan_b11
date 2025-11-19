#include <stdio.h>

// Membuat kode unik otomatis: T001, T002, ...
void generateKodeTransaksi(char *outKode, int nomorBerikutnya) {
    // Nomor berikutnya diasumsikan 1-based
    sprintf(outKode, "T%03d", nomorBerikutnya);
}
