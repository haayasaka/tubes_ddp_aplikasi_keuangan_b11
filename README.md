# Aplikasi Keuangan Mahasiswa

Aplikasi pencatatan keuangan berbasis TUI (Text User Interface) untuk membantu mahasiswa mengelola keuangan bulanan.

## 👥 Tim Pengembang

**Kelompok B11** - Dasar Pemrograman 2025
Elang Permadi Lau (251524042)
Ghaisan Khoirul Badruzaman (251524048)
Hafiz Fauzan Syafrudin (251524049)

## ✨ Fitur Utama

### 📝 Pencatatan Transaksi
- Input transaksi pemasukan dan pengeluaran
- Edit dan hapus transaksi
- Lihat daftar transaksi per bulan
- Format tanggal: dd-mm-YYYY

### 💰 Pengelolaan Pos Anggaran
- Buat pos anggaran dengan batas nominal
- Tracking realisasi pengeluaran real-time
- Status otomatis (Aman/Tidak Aman)
- Copy pos dari bulan sebelumnya

### 📊 Analisis Keuangan
- Ringkasan total pemasukan & pengeluaran
- Grafik perbandingan visual
- Kondisi keuangan: Surplus/Seimbang/Defisit
- Kesimpulan: Hemat/Seimbang/Boros/Tidak Sehat
- Saran pengelolaan keuangan

## 🔧 Requirements

- GCC Compiler
- ncurses library
- Linux/Unix environment

### Install ncurses (Ubuntu/Debian):
```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## 🚀 Build & Run

### Quick Start
```bash
make run
```

### Build Only
```bash
make
```

### Clean Build
```bash
make rebuild
```

### Debug Build
```bash
make debug
```

## 📁 Struktur Proyek

```
AplikasiKeuangan/
├── include/                 # Header files
│   ├── types.h             # Definisi struct
│   ├── constants.h         # Konstanta global
│   ├── app_menu.h          # Menu utama
│   ├── utils/              # Utilitas (string, date, number, dll)
│   ├── validator/          # Validasi input
│   ├── file/               # File handling
│   ├── tui/                # Text User Interface
│   ├── pos/                # Modul pos anggaran
│   ├── transaksi/          # Modul transaksi
│   └── analisis/           # Modul analisis
├── src/                    # Source files
│   ├── main.c              # Entry point
│   ├── app_menu.c          # Menu utama
│   └── [modules]/          # Implementasi modul
├── build/                  # Object files (generated)
├── data/                   # Data files (generated)
├── Makefile                # Build system
└── README.md               # Dokumentasi
```

## 🎮 Navigasi

| Tombol | Fungsi |
|--------|--------|
| ↑/↓ atau k/j | Pindah pilihan |
| ENTER | Pilih/Konfirmasi |
| ESC | Kembali/Batal |
| 1-9 | Pilih langsung item |

## 📋 Format Data

### Transaksi
```
ID|Tanggal|Jenis|Pos|Nominal|Deskripsi
T0001|15-11-2025|Pengeluaran|Makan|50000|Makan siang
```

### Pos Anggaran
```
No|Nama|Nominal|Realisasi|Sisa|JmlTrx|Status
1|Makan|500000|350000|150000|7|1
```

### Analisis Keuangan
```
Pemasukan|Pengeluaran|RataRata|Persentase|Saldo|Kondisi|Kesimpulan|TrxOut|TrxIn
```

## 📊 Kriteria Kesimpulan

| Persentase Sisa | Kesimpulan |
|-----------------|------------|
| > 25% | HEMAT ✅ |
| 0% - 25% | SEIMBANG ⚖️ |
| -10% - 0% | BOROS ⚠️ |
| < -10% | TIDAK SEHAT ❌ |

## 📄 License

Proyek ini dibuat untuk keperluan akademik mata kuliah Dasar Pemrograman.

---

**Kelompok B11** © 2025
