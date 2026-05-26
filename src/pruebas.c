#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();
        printf("Hola EDA II desde el hilo %d de %d\n", id, total);
    }
    return 0;
}

// Private

#include <stdio.h>
#include <omp.h>

int main() {
    int mi_variable = 10;

    printf("Antes de la región: %d\n", mi_variable);

    // Cada hilo crea su propia copia sin inicializar
    #pragma omp parallel private(mi_variable)
    {
        int id = omp_get_thread_num();
        
        // Se debe asignar un valor antes de usarla
        mi_variable = id + 100; 
        
        printf("Hilo %d: mi_variable = %d\n", id, mi_variable);
    }

    // La variable original mantiene su valor inicial
    printf("Después de la región: %d\n", mi_variable);

    return 0;
}
