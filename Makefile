CC = gcc
SRC = src
BIN = bin
CFLAGS_O0 = -O0 -Wall
CFLAGS_O3 = -O3 -Wall
PTHREAD = -lpthread

PROGRAMS = varredura_linha varredura_coluna matmul_padrao matmul_bloco

all: dirs $(PROGRAMS:%=$(BIN)/%_O0) $(PROGRAMS:%=$(BIN)/%_O3) \
     $(BIN)/matmul_pthreads_O3 $(BIN)/matmul_pthreads_bloco_O3

dirs:
	mkdir -p $(BIN)

$(BIN)/varredura_linha_O0: $(SRC)/varredura_linha.c
	$(CC) $(CFLAGS_O0) $< -o $@

$(BIN)/varredura_linha_O3: $(SRC)/varredura_linha.c
	$(CC) $(CFLAGS_O3) $< -o $@

$(BIN)/varredura_coluna_O0: $(SRC)/varredura_coluna.c
	$(CC) $(CFLAGS_O0) $< -o $@

$(BIN)/varredura_coluna_O3: $(SRC)/varredura_coluna.c
	$(CC) $(CFLAGS_O3) $< -o $@

$(BIN)/matmul_padrao_O0: $(SRC)/matmul_padrao.c
	$(CC) $(CFLAGS_O0) $< -o $@

$(BIN)/matmul_padrao_O3: $(SRC)/matmul_padrao.c
	$(CC) $(CFLAGS_O3) $< -o $@

$(BIN)/matmul_bloco_O0: $(SRC)/matmul_bloco.c
	$(CC) $(CFLAGS_O0) $< -o $@

$(BIN)/matmul_bloco_O3: $(SRC)/matmul_bloco.c
	$(CC) $(CFLAGS_O3) $< -o $@

$(BIN)/matmul_pthreads_O3: $(SRC)/matmul_pthreads.c
	$(CC) $(CFLAGS_O3) $< -o $@ $(PTHREAD)

$(BIN)/matmul_pthreads_bloco_O3: $(SRC)/matmul_pthreads_bloco.c
	$(CC) $(CFLAGS_O3) $< -o $@ $(PTHREAD)

clean:
	rm -rf $(BIN)
