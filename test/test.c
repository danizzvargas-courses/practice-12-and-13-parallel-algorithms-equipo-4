#include <stdio.h>
#include <assert.h>
#include <omp.h>

void test_suma() {
    assert(1 + 1 == 2);
    printf("test_suma: OK\n");
}

int main() {
    test_suma();
    printf("Todos los tests pasaron.\n");
    return 0;
}
