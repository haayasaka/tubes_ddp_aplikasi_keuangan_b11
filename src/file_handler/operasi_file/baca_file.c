#include <stdio.h>

// Membaca seluruh isi file ke buffer (untuk kasus sederhana)
// Mengembalikan jumlah karakter yang terbaca
int bacaFile(const char *namaFile, char *buffer, int ukuranBuffer) {
    FILE *f = fopen(namaFile, "r");
    if (!f) {
        // Jika belum ada, buat file kosong
        f = fopen(namaFile, "w");
        if (!f) return 0;
        fclose(f);
        buffer[0] = '\0';
        return 0;
    }
    int total = fread(buffer, 1, ukuranBuffer - 1, f);
    buffer[total] = '\0';
    fclose(f);
    return total;
}
