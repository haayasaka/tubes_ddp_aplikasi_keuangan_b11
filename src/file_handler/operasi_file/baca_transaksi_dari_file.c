#include <stdio.h>
#include <string.h>
#include "tipe_data.h"
#include <stdlib.h>

// Membaca file transaksi ke array (membatasi sesuai kapasitas)
int bacaTransaksiDariFile(Transaksi *arrayTransaksi, int kapasitas, const char *namaFile) {
    FILE *f = fopen(namaFile, "r");
    if (!f) {
        f = fopen(namaFile, "w");
        if (!f) return 0;
        fclose(f);
        return 0;
    }

    int hitung = 0;
    char baris[512];
    while (fgets(baris, sizeof(baris), f) && hitung < kapasitas) {
        // Parsel format ID|Tanggal|Jenis|Kategori|Nominal|Deskripsi
        // Gunakan strtok untuk memisahkan
        char *token = strtok(baris, "|");
        if (!token) continue;
        strncpy(arrayTransaksi[hitung].kodeTransaksi, token, sizeof(arrayTransaksi[hitung].kodeTransaksi) - 1);
        arrayTransaksi[hitung].kodeTransaksi[sizeof(arrayTransaksi[hitung].kodeTransaksi) - 1] = '\0';
        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(arrayTransaksi[hitung].tanggal, token, sizeof(arrayTransaksi[hitung].tanggal) - 1);
        arrayTransaksi[hitung].tanggal[sizeof(arrayTransaksi[hitung].tanggal) - 1] = '\0';
        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(arrayTransaksi[hitung].jenis, token, sizeof(arrayTransaksi[hitung].jenis) - 1);
        arrayTransaksi[hitung].jenis[sizeof(arrayTransaksi[hitung].jenis) - 1] = '\0';
        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(arrayTransaksi[hitung].kategori, token, sizeof(arrayTransaksi[hitung].kategori) - 1);
        arrayTransaksi[hitung].kategori[sizeof(arrayTransaksi[hitung].kategori) - 1] = '\0';
        token = strtok(NULL, "|");
        if (!token) continue;
        arrayTransaksi[hitung].nominal = atof(token);
        token = strtok(NULL, "\n");
        if (!token) token = "";
        strncpy(arrayTransaksi[hitung].deskripsi, token, sizeof(arrayTransaksi[hitung].deskripsi) - 1);
        arrayTransaksi[hitung].deskripsi[sizeof(arrayTransaksi[hitung].deskripsi) - 1] = '\0';

        // Trim newline di field string jika ada
        for (char *p = arrayTransaksi[hitung].kodeTransaksi; *p; ++p) if (*p == '\n') *p = '\0';
        for (char *p = arrayTransaksi[hitung].tanggal; *p; ++p) if (*p == '\n') *p = '\0';
        for (char *p = arrayTransaksi[hitung].jenis; *p; ++p) if (*p == '\n') *p = '\0';
        for (char *p = arrayTransaksi[hitung].kategori; *p; ++p) if (*p == '\n') *p = '\0';
        for (char *p = arrayTransaksi[hitung].deskripsi; *p; ++p) if (*p == '\n') *p = '\0';

        hitung++;
    }
    fclose(f);
    return hitung;
}
