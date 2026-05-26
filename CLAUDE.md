# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

EDA II – Exposición sobre alcance de variables en programación paralela con OpenMP en C.
Tema: diferencias entre `private`, `firstprivate`, `lastprivate` y `threadprivate`.

Alumnos:
- David Hernández Hernandez (threadprivate)
- Erik Yair Nogueira Lopez
- Diego Omar Sánchez de Tagle Caballero

## Estructura

```
src/
  FirstPrivate.c   — ejemplo de firstprivate
  LastPrivate.c    — ejemplo de lastprivate
  ThreadPrivate.c  — ejemplo didáctico de threadprivate (persistencia entre regiones)
test/
  pruebas.c        — pruebas varias
  test.c           — tests con assert.h
Makefile           — compilación con clang + libomp (macOS Homebrew)
```

## Build & Run

En macOS, `gcc` no soporta OpenMP nativamente. Se usa `clang` con `libomp` instalado via Homebrew.

```bash
# Instalar libomp si no está instalado
brew install libomp

# Compilar y correr cada implementación con Make
make run-thread   # ThreadPrivate
make run-first    # FirstPrivate
make run-last     # LastPrivate

# Compilar todo sin correr
make

# Limpiar binarios
make clean
```

### Compilación manual (sin Make)

```bash
clang -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include \
      -L/opt/homebrew/opt/libomp/lib -lomp \
      src/ThreadPrivate.c -o ThreadPrivate

./ThreadPrivate
```

### Controlar número de hilos

```bash
OMP_NUM_THREADS=4 ./ThreadPrivate
```

## OpenMP — conceptos del proyecto

| Cláusula/Directiva | Tipo | Inicialización | Persiste entre regiones |
|---|---|---|---|
| `private` | cláusula | indefinida | No |
| `firstprivate` | cláusula | hereda valor original | No |
| `lastprivate` | cláusula | indefinida | No (escribe al salir) |
| `threadprivate` | directiva | persiste del hilo | Sí |

- `threadprivate` solo aplica a variables globales o estáticas.
- Usar `copyin` para inicializar todos los hilos desde el hilo maestro.
- El número de hilos debe ser consistente entre regiones para garantizar persistencia.

## Testing

Tests usan `assert.h` directamente. Cada función de test llama `assert()` e imprime un resultado.

```bash
clang -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include \
      -L/opt/homebrew/opt/libomp/lib -lomp \
      test/test.c -o test_runner && ./test_runner
```
