#include <stdio.h>
#include "perhitungan.h"

void tampilkanAnalisisKondisi(double saldo) {
    const char *kondisi = analisisKondisiKeuangan(saldo);
    printf("\nKondisi Keuangan Anda: %s.\n", kondisi);
    if (saldo > 0.0) {
        printf("Anda berhasil mengatur keuangan dengan baik bulan ini. Pertahankan pengeluaran agar tetap terkendali.\n");
    } else if (saldo == 0.0) {
        printf("Keuangan Anda seimbang. Tetap disiplin agar tidak beralih ke defisit.\n");
    } else {
        printf("Anda sedang defisit. Segera evaluasi pengeluaran dan pertimbangkan menambah pemasukan.\n");
    }
}
