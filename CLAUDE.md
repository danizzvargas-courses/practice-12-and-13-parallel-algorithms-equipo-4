# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

EDA II – Parallel Algorithms exposition using OpenMP in C. Implementations go in `src/pruebas.c` and tests in `test/test.c`.

## Build & Run

```bash
# Compile with OpenMP
gcc -fopenmp src/pruebas.c -o pruebas

# Run
./pruebas

# Compile and run tests
gcc -fopenmp test/test.c -o test_runner && ./test_runner
```

## Testing

Tests use `assert.h` directly — no external framework. Each test function calls `assert()` and prints a result line. `main()` in `test/test.c` calls all test functions.

## OpenMP Notes

- Always compile with `-fopenmp`
- Use `omp_get_thread_num()` and `omp_get_num_threads()` for thread identification
- Parallel regions are declared with `#pragma omp parallel`
- Control thread count at runtime: `OMP_NUM_THREADS=4 ./pruebas`
