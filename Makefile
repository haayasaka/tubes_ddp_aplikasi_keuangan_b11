# ============================================================ 
# Makefile - Aplikasi Keuangan Mahasiswa
# Kelompok B11 - Dasar Pemrograman 2025
# ============================================================ 

# Compiler dan flags
CC = gcc
CFLAGS = -Wall -Wextra -I include
LDFLAGS = -lncurses

# Direktori
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Target executable
TARGET = keuangan

# Cari semua source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate object files dari source files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Semua header files untuk dependency
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# ============================================================ 
# Targets
# ============================================================ 

# Default target
all: dirs $(TARGET)

# Buat direktori yang diperlukan
dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p data

# Link semua object files menjadi executable
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile source files menjadi object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================================ 
# Utility targets
# ============================================================ 

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	@echo "Clean complete."

# Clean semua termasuk data
cleanall: clean
	@echo "Cleaning data files..."
	rm -rf data
	@echo "Clean all complete."

# Rebuild dari awal
rebuild: clean all

# Run aplikasi
run: all
	./$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: clean all

# Release build (optimized)
release: CFLAGS += -O2 -DNDEBUG
release: clean all

# ============================================================ 
# Development targets
# ============================================================ 

# Compile check tanpa linking
check:
	@echo "Checking compilation..."
	@for src in $(SRCS); do \
		echo "Checking $$src..."; \
		$(CC) $(CFLAGS) -c $$src -o /dev/null 2>&1 || exit 1; \
	done
	@echo "All files compile successfully."

# Show project info
info:
	@echo "============================================"
	@echo "Aplikasi Keuangan Mahasiswa"
	@echo "Kelompok B11 - Dasar Pemrograman 2025"
	@echo "============================================"
	@echo ""
	@echo "Source files: $(words $(SRCS))"
	@echo "Header files: $(words $(HEADERS))"
	@echo "Object files: $(words $(OBJS))"
	@echo ""
	@echo "Directories:"
	@echo "  Source:  $(SRC_DIR)/"
	@echo "  Build:   $(BUILD_DIR)/"
	@echo "  Include: $(INCLUDE_DIR)/"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build aplikasi (default)"
	@echo "  run      - Build dan jalankan"
	@echo "  clean    - Hapus build files"
	@echo "  cleanall - Hapus build dan data files"
	@echo "  rebuild  - Clean dan build ulang"
	@echo "  debug    - Build dengan debug symbols"
	@echo "  release  - Build optimized"
	@echo "  check    - Cek kompilasi semua file"
	@echo "  info     - Tampilkan info proyek"

# Help
help: info

# ============================================================ 
# Phony targets
# ============================================================ 
.PHONY: all dirs clean cleanall rebuild run debug release check info help