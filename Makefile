# Compiler dan opsi
CC     := gcc
CFLAGS := -Iinclude -Wall -Wextra -O2

# Cari semua file .c di src dan subfoldernya
SRC    := $(shell find src -name "*.c")
OBJ    := $(SRC:.c=.o)

# Nama file output
BIN    := bin/keuangan_mahasiswa.exe

# Target utama
all: $(BIN)

# Gabungkan semua object file jadi executable
$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Compile setiap .c jadi .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Bersihkan semua file hasil kompilasi
clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
