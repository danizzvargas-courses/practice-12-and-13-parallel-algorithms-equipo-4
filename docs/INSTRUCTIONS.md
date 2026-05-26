# Exposición EDA II

## Archivos a modificar

- `src/pruebas.c` - Implementa el código en C con OpenMP
- `src/LastPrivate.c` - Ejemplo de `lastprivate` en OpenMP

## Ejecutar tests

```bash
gcc -fopenmp test/test.c -o test_runner
./test_runner
```

## Entrega

1. Implementa el código en `src/pruebas.c` y los ejemplos en `src/LastPrivate.c`
2. Verifica que `test/test.c` compile y ejecute correctamente
3. Registra los resultados y responde las preguntas en `README.md`
