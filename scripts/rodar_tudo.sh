#!/bin/bash
set -e

OUT="resultados.txt"
echo "Log completo em: $OUT"
echo "===== INICIO DOS TESTES - $(date) =====" > "$OUT"

run() {
    echo "\$ $*" | tee -a "$OUT"
    "$@" 2>&1 | tee -a "$OUT"
    echo "" >> "$OUT"
}

echo ""
echo "### 0. HARDWARE E SISTEMA ###"
echo "### 0. HARDWARE E SISTEMA ###" >> "$OUT"
echo "--- Linux (Codespaces) detectado ---" | tee -a "$OUT"
run uname -a
run lscpu
run cat /proc/meminfo
run gcc --version

echo ""
echo "### 1. COMPILACAO ###"
echo "### 1. COMPILACAO ###" >> "$OUT"
run make clean
run make

echo ""
echo "### 2. ITEM 2 - VARREDURA LINHA vs COLUNA ###"
echo "### 2. ITEM 2 - VARREDURA LINHA vs COLUNA ###" >> "$OUT"
for N in 512 1024 2048 4096 8192; do
    run ./bin/varredura_linha_O3 $N
    run ./bin/varredura_coluna_O3 $N
done

echo ""
echo "### 3. ITEM 3 - MATMUL PADRAO vs BLOCO (-O0 e -O3) ###"
echo "### 3. ITEM 3 - MATMUL PADRAO vs BLOCO (-O0 e -O3) ###" >> "$OUT"
for N in 512 1024 1536; do
    run ./bin/matmul_padrao_O0 $N
    run ./bin/matmul_bloco_O0 $N 64
    run ./bin/matmul_padrao_O3 $N
    run ./bin/matmul_bloco_O3 $N 64
done

echo ""
echo "### 4. ITEM 3 - PROFILING DE CACHE (Cachegrind) ###"
echo "### 4. PROFILING DE CACHE ###" >> "$OUT"
command -v valgrind >/dev/null 2>&1 || { echo "Instalando valgrind..."; sudo apt-get update -qq && sudo apt-get install -y valgrind; }
run valgrind --tool=cachegrind --cache-sim=yes --branch-sim=no ./bin/matmul_padrao_O3 512
run valgrind --tool=cachegrind --cache-sim=yes --branch-sim=no ./bin/matmul_bloco_O3 512 64

echo ""
echo "### 5. ITEM 4 - ESCALABILIDADE PTHREADS (1,2,4,8,16 threads) ###"
echo "### 5. ITEM 4 - ESCALABILIDADE PTHREADS ###" >> "$OUT"
for T in 1 2 4 8 16; do
    run ./bin/matmul_pthreads_O3 1024 $T
done

echo ""
echo "### 6. ITEM 4 - PTHREADS + BLOCAGEM (1,2,4,8,16 threads) ###"
echo "### 6. ITEM 4 - PTHREADS + BLOCAGEM ###" >> "$OUT"
for T in 1 2 4 8 16; do
    run ./bin/matmul_pthreads_bloco_O3 1024 $T 64
done

echo ""
echo "===== FIM DOS TESTES - $(date) =====" | tee -a "$OUT"
echo ""
echo "Tudo salvo em $OUT"
