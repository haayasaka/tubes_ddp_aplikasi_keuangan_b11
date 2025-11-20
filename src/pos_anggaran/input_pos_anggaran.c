#include <stdio.h>
#include <string.h>
#include "tipe_data.h"

// Menerima input nama pos dan batas nominal
PosAnggaran inputPosAnggaran(void) {
    PosAnggaran pos;
    char nama[64];
    double batas;

    printf("Masukkan nama pos anggaran: ");
    fgets(nama, sizeof(nama), stdin);
    // Hapus newline
    nama[strcspn(nama, "\n")] = '\0';

    printf("Masukkan batas nominal: ");
    scanf("%lf", &batas);
    getchar(); // konsumsi newline sisa

    pos.kodePos = -1; // akan diisi kemudian saat ditambahkan ke array
    strncpy(pos.namaPos, nama, sizeof(pos.namaPos));
    pos.batasNominal = batas;

    return pos;
}
