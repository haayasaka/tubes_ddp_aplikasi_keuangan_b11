#ifndef POS_ANGGARAN_H
#define POS_ANGGARAN_H

#include "tipe_data.h"

PosAnggaran inputPosAnggaran(void);
int validasiPosAnggaran(const char *nama, double batas, PosAnggaran *arrayPos, int jumlahPos);
int simpanPosAnggaran(PosAnggaran pos, const char *namaFile);

int validasiHapusPos(int kodePos, PosAnggaran *arrayPos, int jumlahPos);
int hapusPosAnggaran(int kodePos, PosAnggaran *arrayPos, int *jumlahPos);
int validasiEditPos(const char *namaBaru, double batasBaru, PosAnggaran *arrayPos, int jumlahPos, int kodePos);
int editPosAnggaran(int kodePos, const char *namaBaru, double batasBaru, PosAnggaran *arrayPos, int jumlahPos);
int updateFilePosAnggaran(PosAnggaran *arrayPos, int jumlahPos, const char *namaFile);

#endif
