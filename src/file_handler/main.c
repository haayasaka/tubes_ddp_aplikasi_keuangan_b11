#include <stdio.h>
#include <string.h>
#include "tipe_data.h"
#include "file_handler.h"
#include "validasi.h"
#include "tampilan.h"
#include "pos_anggaran.h"
#include "transaksi.h"
#include "perhitungan.h"
#include "laporan.h"

#define FILE_POS "data/pos_anggaran.txt"
#define FILE_TRX "data/data_keuangan.txt"

int main(void) {
    PosAnggaran arrayPos[MAKS_POS];
    int jumlahPos = 0;
    Transaksi arrayTrx[MAKS_TRANSAKSI];
    int jumlahTrx = 0;
    int nomorTrxBerikutnya = 1;

    // Muat transaksi dari file pada awal
    jumlahTrx = bacaTransaksiDariFile(arrayTrx, MAKS_TRANSAKSI, FILE_TRX);
    nomorTrxBerikutnya = jumlahTrx + 1;

    // Muat pos anggaran dari file (opsional sederhana)
    // Format: kode|nama|batas
    {
        FILE *f = fopen(FILE_POS, "r");
        if (f) {
            char baris[256];
            while (fgets(baris, sizeof(baris), f) && jumlahPos < MAKS_POS) {
                int kode; char nama[64]; double batas;
                // Parsing sederhana
                if (sscanf(baris, "%d|%63[^|]|%lf", &kode, nama, &batas) == 3) {
                    arrayPos[jumlahPos].kodePos = kode;
                    strncpy(arrayPos[jumlahPos].namaPos, nama, sizeof(arrayPos[jumlahPos].namaPos));
                    arrayPos[jumlahPos].batasNominal = batas;
                    jumlahPos++;
                }
            }
            fclose(f);
        } else {
            // Buat file jika belum ada
            f = fopen(FILE_POS, "w");
            if (f) fclose(f);
        }
    }

    int pilih;
    do {
        tampilkanMenu();
        if (scanf("%d", &pilih) != 1) { getchar(); continue; }
        getchar(); // konsumsi newline

        switch (pilih) {
            case 1: {
                PosAnggaran pos = inputPosAnggaran();
                if (!validasiPosAnggaran(pos.namaPos, pos.batasNominal, arrayPos, jumlahPos)) {
                    tampilkanPesanError("Data pos tidak valid (nama unik & batas > 0).");
                    break;
                }
                pos.kodePos = (jumlahPos == 0) ? 1 : arrayPos[jumlahPos - 1].kodePos + 1;
                arrayPos[jumlahPos++] = pos;
                if (!simpanPosAnggaran(pos, FILE_POS)) {
                    tampilkanPesanError("Gagal menyimpan pos ke file.");
                } else {
                    tampilkanPesanSukses("Pos anggaran ditambahkan.");
                }
                break;
            }
            case 2: {
                int kode;
                printf("Masukkan kode pos yang akan dihapus: ");
                scanf("%d", &kode); getchar();
                if (!validasiHapusPos(kode, arrayPos, jumlahPos)) {
                    tampilkanPesanError("Kode pos tidak ditemukan.");
                    break;
                }
                if (!hapusPosAnggaran(kode, arrayPos, &jumlahPos)) {
                    tampilkanPesanError("Gagal menghapus pos.");
                    break;
                }
                if (!updateFilePosAnggaran(arrayPos, jumlahPos, FILE_POS)) {
                    tampilkanPesanError("Gagal memperbarui file pos.");
                } else {
                    tampilkanPesanSukses("Pos anggaran dihapus.");
                }
                break;
            }
            case 3: {
                int kode;
                char namaBaru[64];
                double batasBaru;
                printf("Masukkan kode pos yang akan diedit: ");
                scanf("%d", &kode);
                while (getchar() != '\n');  // buang newline dari buffer

                printf("Nama baru: ");
                fgets(namaBaru, sizeof(namaBaru), stdin);
                namaBaru[strcspn(namaBaru, "\n")] = '\0';  // hapus newline

                printf("Batas nominal baru: ");
                scanf("%lf", &batasBaru);

                if (!validasiEditPos(namaBaru, batasBaru, arrayPos, jumlahPos, kode)) {
                    tampilkanPesanError("Data edit tidak valid (nama unik & batas > 0).");
                    break;
                }
                if (!editPosAnggaran(kode, namaBaru, batasBaru, arrayPos, jumlahPos)) {
                    tampilkanPesanError("Kode pos tidak ditemukan.");
                    break;
                }
                if (!updateFilePosAnggaran(arrayPos, jumlahPos, FILE_POS)) {
                    tampilkanPesanError("Gagal memperbarui file pos.");
                } else {
                    tampilkanPesanSukses("Pos anggaran diperbarui.");
                }
                break;
            }
            case 4: {
                Transaksi trx = inputTransaksi();
                if (!validasiTransaksi(&trx, arrayPos, jumlahPos)) {
                    tampilkanPesanError("Transaksi tidak valid (jenis/besar/tanggal/pos).");
                    break;
                }
                generateKodeTransaksi(trx.kodeTransaksi, nomorTrxBerikutnya++);
                arrayTrx[jumlahTrx++] = trx;
                if (!simpanTransaksi(&trx, FILE_TRX)) {
                    tampilkanPesanError("Gagal menyimpan transaksi.");
                } else {
                    tampilkanPesanSukses("Transaksi ditambahkan.");
                }
                break;
            }
            case 5:
                tampilkanSemuaTransaksi(arrayTrx, jumlahTrx);
                break;
            case 6:
                tampilkanTransaksiPemasukan(arrayTrx, jumlahTrx);
                break;
            case 7:
                tampilkanTransaksiPengeluaran(arrayTrx, jumlahTrx);
                break;
            case 8:
                tampilkanRekapitulasi(arrayTrx, jumlahTrx);
                break;
            case 9:
                tampilkanRealisasiPosAnggaran(arrayPos, jumlahPos, arrayTrx, jumlahTrx);
                break;
            case 10:
                if (simpanTransaksiKeFile(arrayTrx, jumlahTrx, FILE_TRX))
                    tampilkanPesanSukses("Seluruh transaksi disimpan ulang ke file.");
                else
                    tampilkanPesanError("Gagal menulis ulang transaksi ke file.");
                break;
            case 0:
                printf("Terima kasih, sampai jumpa.\n");
                break;
            default:
                tampilkanPesanError("Menu tidak dikenal.");
        }
    } while (pilih != 0);

    return 0;
}
