const char* analisisKondisiKeuangan(double saldo) {
    if (saldo < 0.0) return "Defisit";
    if (saldo == 0.0) return "Seimbang";
    return "Surplus";
}
