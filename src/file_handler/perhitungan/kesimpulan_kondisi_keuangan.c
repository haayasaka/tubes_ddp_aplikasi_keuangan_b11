// Menentukan kesimpulan gaya hidup keuangan mahasiswa
const char* kesimpulanMahasiswa(double totalPemasukan, double saldo) {
    if (totalPemasukan <= 0.0) {
        return "Pemasukan tidak terdeteksi. Fokus pada menambah pemasukan.";
    }
    double persen = (saldo / totalPemasukan) * 100.0;

    if (saldo >= 0.0) {
        if (persen > 25.0) return "Anda termasuk mahasiswa hemat dan produktif.";
        if (persen > 0.0)  return "Keuangan Anda seimbang, tetap waspada terhadap pengeluaran tidak perlu.";
        return "Keuangan Anda seimbang, tetap waspada terhadap pengeluaran tidak perlu.";
    } else {
        double defisit = -persen;
        if (defisit < 10.0) return "Anda mulai boros, perhatikan pengeluaran harian.";
        return "Kondisi keuangan kurang sehat. Kurangi pengeluaran dan cari tambahan pemasukan.";
    }
}
