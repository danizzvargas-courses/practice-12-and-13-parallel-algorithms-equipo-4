#include <stdio.h>
#include <omp.h>

int main() {
    int first = 100; // Valor inicial antes de la región paralela

    printf("Valor inicial fuera de la región paralela: first = %d\n", first);

    // Cada hilo tendrá una copia local inicializada en 100
    #pragma omp parallel firstprivate(first) num_threads(4)
    {
        int id = omp_get_thread_num();
        
        // Modificamos la copia local de cada hilo
        first = first + id;
        
        printf("Hilo %d: copia local de first = %d\n", id, first);
    }

    // El valor original no cambia después de la región paralela
    printf("Después de la región paralela: first = %d\n", first);

    return 0;
}
