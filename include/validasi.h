#ifndef VALIDASI_H
#define VALIDASI_H

#include "tipe_data.h"

int isPositive(double angka);
int isNotEmpty(const char *teks);
int isUniquePos(const char *nama, PosAnggaran *arrayPos, int jumlahPos);
int isValidJenisTransaksi(const char *jenis);

#endif
