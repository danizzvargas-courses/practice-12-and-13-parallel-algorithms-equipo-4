# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

EDA II – Parallel Algorithms exposition using OpenMP in C. Implementations go in `src/pruebas.c` and tests in `test/test.c`.

## Build & Run

```bash
# Compilar e implementación
gcc -fopenmp src/pruebas.c -o pruebas
./pruebas

# Compilar y correr tests
gcc -fopenmp test/test.c -o test_runner && ./test_runner
```

## Testing

Tests usan `assert.h` directamente. Cada función de test llama `assert()` e imprime un resultado. `main()` en `test/test.c` llama todas las funciones de test.

## OpenMP

- Siempre compilar con `-fopenmp`
- Regiones paralelas con `#pragma omp parallel`
- Controlar número de hilos: `OMP_NUM_THREADS=4 ./pruebas`
