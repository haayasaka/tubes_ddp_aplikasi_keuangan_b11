#include <string.h>

int isValidJenisTransaksi(const char *jenis) {
    return (strcmp(jenis, "Pemasukan") == 0 || strcmp(jenis, "Pengeluaran") == 0);
}
