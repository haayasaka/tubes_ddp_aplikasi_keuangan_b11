#include <stdio.h>
#include "perhitungan.h"

void tampilkanKesimpulanMahasiswa(double totalPemasukan, double saldo) {
    const char *kesimpulan = kesimpulanMahasiswa(totalPemasukan, saldo);
    printf("\nKesimpulan: %s\n", kesimpulan);
}
