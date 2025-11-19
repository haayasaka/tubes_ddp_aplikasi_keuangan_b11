#include <stdio.h>
#include "tipe_data.h"
#include <ctype.h>      // Untuk fungsi tolower()
#include <string.h>     // Untuk strcmp()
#include "perhitungan.h"

static int sama_tanpa_case(const char *a, const char *b) {
    if (!a || !b) return 0;
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

/*
 * Menghitung total pemasukan dari array transaksi
 */

double hitungTotalPemasukan(const Transaksi *arrayTrx, int jumlahTrx) {
    if (!arrayTrx || jumlahTrx <= 0) return 0.0;

    double total = 0.0;
    for (int i = 0; i < jumlahTrx; i++) {
        const char *jenis = arrayTrx[i].jenis;
        if (jenis && (sama_tanpa_case(jenis, "PEMASUKAN") || sama_tanpa_case(jenis, "Pemasukan"))) {
            total += arrayTrx[i].nominal;
        }
    }
    return total;
}
