#include <stdio.h>
#include <string.h>
#include "tipe_data.h"

// Menerima input jenis, nominal, tanggal, pos/kategori, deskripsi
Transaksi inputTransaksi(void) {
    Transaksi trx;
    char jenis[16], tanggal[16], kategori[64], deskripsi[128];
    double nominal;

    printf("Masukkan tanggal (YYYY-MM-DD): ");
    fgets(tanggal, sizeof(tanggal), stdin);
    tanggal[strcspn(tanggal, "\n")] = '\0';

    printf("Jenis (Pemasukan/Pengeluaran): ");
    fgets(jenis, sizeof(jenis), stdin);
    jenis[strcspn(jenis, "\n")] = '\0';

    printf("Kategori/Pos Anggaran: ");
    fgets(kategori, sizeof(kategori), stdin);
    kategori[strcspn(kategori, "\n")] = '\0';

    printf("Nominal: ");
    scanf("%lf", &nominal);
    getchar(); // konsumsi newline

    printf("Deskripsi singkat: ");
    fgets(deskripsi, sizeof(deskripsi), stdin);
    deskripsi[strcspn(deskripsi, "\n")] = '\0';

    strncpy(trx.tanggal, tanggal, sizeof(trx.tanggal));
    strncpy(trx.jenis, jenis, sizeof(trx.jenis));
    strncpy(trx.kategori, kategori, sizeof(trx.kategori));
    trx.nominal = nominal;
    strncpy(trx.deskripsi, deskripsi, sizeof(trx.deskripsi));

    trx.kodeTransaksi[0] = '\0'; // akan diisi oleh generateKodeTransaksi
    return trx;
}
