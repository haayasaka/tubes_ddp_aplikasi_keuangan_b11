#include <stdio.h>
#include <string.h>
#include <perhitungan.h>

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

void tampilkanKesimpulanMahasiswa(double totalPemasukan, double saldo) {
    const char *kesimpulan = kesimpulanMahasiswa(totalPemasukan, saldo);
    printf("\nKesimpulan: %s\n", kesimpulan);
}

void tampilkanRealisasiPosAnggaran(PosAnggaran *arrayPos, int jumlahPos, Transaksi *arrayTrx, int jumlahTrx) {
    double realisasi[MAKS_POS];
    hitungRealisasiAnggaran(arrayPos, jumlahPos, arrayTrx, jumlahTrx, realisasi);

    printf("\n=== Realisasi Pos Anggaran ===\n");
    printf("Pos                | Batas       | Pengeluaran | Sisa\n");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < jumlahPos; ++i) {
        double sisa = arrayPos[i].batasNominal - realisasi[i];
        printf("%-18s | %11.2f | %11.2f | %11.2f\n",
               arrayPos[i].namaPos, arrayPos[i].batasNominal, realisasi[i], sisa);
    }
}

void tampilkanRekapitulasi(Transaksi *arrayTrx, int jumlahTrx) {
    double totalMasuk = hitungTotalPemasukan(arrayTrx, jumlahTrx);
    double totalKeluar = hitungTotalPengeluaran(arrayTrx, jumlahTrx);
    double saldo = hitungSaldo(totalMasuk, totalKeluar);
    double rataKeluar = hitungRataRataPengeluaran(arrayTrx, jumlahTrx);

    printf("\n=== Rekapitulasi ===\n");
    printf("Total Pemasukan   : %.2f\n", totalMasuk);
    printf("Total Pengeluaran : %.2f\n", totalKeluar);
    printf("Saldo             : %.2f\n", saldo);
    printf("Rata-rata Pengeluaran: %.2f\n", rataKeluar);
}

void tampilkanSemuaTransaksi(Transaksi *arrayTrx, int jumlahTrx) {
    printf("\n=== Semua Transaksi ===\n");
    printf("ID   | Tanggal     | Jenis       | Kategori                | Nominal     | Deskripsi\n");
    printf("-------------------------------------------------------------------------------------\n");
    for (int i = 0; i < jumlahTrx; ++i) {
        printf("%-4s | %-11s | %-11s | %-22s | %11.2f | %s\n",
            arrayTrx[i].kodeTransaksi,
            arrayTrx[i].tanggal,
            arrayTrx[i].jenis,
            arrayTrx[i].kategori,
            arrayTrx[i].nominal,
            arrayTrx[i].deskripsi
        );
    }
}

void tampilkanTransaksiPemasukan(Transaksi *arrayTrx, int jumlahTrx) {
    printf("\n=== Transaksi Pemasukan ===\n");
    for (int i = 0; i < jumlahTrx; ++i) {
        if (strcmp(arrayTrx[i].jenis, "Pemasukan") == 0) {
            printf("%s | %s | %s | %.2f | %s\n",
                arrayTrx[i].kodeTransaksi,
                arrayTrx[i].tanggal,
                arrayTrx[i].kategori,
                arrayTrx[i].nominal,
                arrayTrx[i].deskripsi
            );
        }
    }
}

void tampilkanTransaksiPengeluaran(Transaksi *arrayTrx, int jumlahTrx) {
    printf("\n=== Transaksi Pengeluaran ===\n");
    for (int i = 0; i < jumlahTrx; ++i) {
        if (strcmp(arrayTrx[i].jenis, "Pengeluaran") == 0) {
            printf("%s | %s | %s | %.2f | %s\n",
                arrayTrx[i].kodeTransaksi,
                arrayTrx[i].tanggal,
                arrayTrx[i].kategori,
                arrayTrx[i].nominal,
                arrayTrx[i].deskripsi
            );
        }
    }
}
