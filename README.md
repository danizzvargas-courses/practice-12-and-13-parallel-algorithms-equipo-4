# EDA II – Exposición

Alumnos:
- 322246698 - Nogueira Lopez Erik Yair
- 322166910 - Sánchez de Tagle Caballero Diego Omar
- 425094484 - Hernández Hernández David

---

## Objetivo

El estudiante implementará y expondrá los temas asignados de Estructuras de Datos y Algoritmos II.
Controlar con precisión qué valor toma una variable al entrar a una región paralela y qué valor queda al salir, según cómo se declare.

---

## Descripción

Durante la práctica se implementará la herramienta estándar para programación en paralelo en sistemas de memoria compartida OpenMP, dentro de la práctica se observará los comportamientos y diferencia entre private, firstprivate y lastprivate (con qué valor entran y salen las variables).

Así como threadprivate y la manera en la que se distingue de las anteriores.

---

## Investigación
# Gestión de Datos en OpenMP

OpenMP (**Open Multi-Processing**) es una API diseñada para la computación paralela en sistemas de memoria compartida. A continuación, se detalla cómo se gestiona el alcance y la visibilidad de las variables entre hilos.

## 1. Tipos de Alcance de Variables (Data Sharing)

El alcance determina qué hilos pueden ver o modificar una variable:

*   **Shared (Compartida):** Todos los hilos acceden a la misma dirección de memoria. Es el valor por defecto para la mayoría de las variables declaradas fuera de una región paralela.
*   **Private (Privada):** Cada hilo tiene su propia copia local de la variable. Los cambios realizados por un hilo no son visibles para los demás.

---

## 2. Funcionamiento de Cláusulas de Privacidad

Para controlar con precisión la inicialización y el retorno de los valores en variables privadas, se utilizan las siguientes cláusulas:


| Cláusula | Valor al entrar (Entrada) | Valor al salir (Salida) |
| :--- | :--- | :--- |
| `private` | No definido (valor residual/basura). | Se pierde el valor local; la variable original no cambia. |
| `firstprivate` | Se inicializa con el valor que tenía antes de la región paralela. | Se pierde el valor local del hilo. |
| `lastprivate` | Igual que `private` (o `firstprivate` si se combinan). | El valor de la **última iteración lógica** (en un bucle) se copia a la variable original. |

---

## 3. Directiva `threadprivate`

A diferencia de las anteriores, que actúan sobre regiones paralelas específicas, `threadprivate` se aplica a variables **globales o estáticas**.

*   **Persistencia:** El valor de la variable persiste entre diferentes regiones paralelas a lo largo de la vida del hilo.
*   **Uso:** Ideal para mantener estados globales únicos por hilo sin necesidad de pasar la variable como argumento constantemente.

---

## 4. Casos de Uso: ¿Cuándo usar cada una?


| Si necesitas... | Usa... |
| :--- | :--- |
| Una variable temporal o un índice de bucle interno. | `private` |
| Que cada hilo lea una configuración inicial común. | `firstprivate` |
| Recuperar el resultado del último paso de un bucle paralelo. | `lastprivate` |
| Mantener copias locales de variables globales/estáticas. | `threadprivate` |



---

## Implementación

### LastPrivate

1. Directiva
   
#pragma omp parallel for
 - La directiva le dice al compilador que el bucle for que sigue debe dividirse entre los hilos disponibles en el sistema. 

2. Cláusula
   
lastprivate(last)
 - Aislamiento: Crea una copia local de last para cada hilo, esto evita una condición de carrera, donde varios hilos intentarían escribir en la misma memoria al mismo tiempo.
  - Sincronización final: A diferencia de una variable private normal (que se destruye al terminar), lastprivate toma el valor del hilo que ejecutó la última iteración lógica y lo copia de vuelta a la variable original del main. 

3. Funciones de la Librería
   
#include <omp.h>: Necesario para usar las funciones de OpenMP.
  - omp_get_thread_num(): Devuelve el identificador único del hilo que está ejecutando ese código en ese momento (ej. Hilo 0, Hilo 1, etc.).

4. Flujo de ejecución
 
  - Fuera del bloque: last vale -1.
  - Dentro del bloque: Cada hilo trabaja con su propia versión de last, pero cada uno reporta su propio valor de i.
  - Salida del bloque: El sistema identifica qué hilo hizo i = 7 y "rescata" ese valor.
  - Final: El printf final muestra 7, confirmando que la comunicación entre el mundo paralelo y el secuencial fue exitosa.


### FirstPrivate

### Threadprivate

1. Directiva

#pragma omp threadprivate(contador)
 - A diferencia de private, firstprivate y lastprivate que son cláusulas dentro de un #pragma omp parallel, threadprivate es una directiva independiente. Se declara una sola vez fuera de cualquier región paralela y se aplica únicamente a variables globales o estáticas, ya que estas necesitan un espacio permanente en memoria por cada hilo.

2. Cláusula copyin

copyin(contador)
 - Inicialización controlada: Sin copyin, cada hilo retoma el valor que tenía su copia en la región paralela anterior (o un valor indefinido la primera vez). Con copyin, el compilador copia el valor actual de la variable en el hilo maestro (hilo 0) hacia las copias de todos los demás hilos al inicio de la región paralela.
 - Diferencia clave: Mientras firstprivate inicializa desde la variable original del main, copyin inicializa desde la copia del hilo maestro, que puede haber sido modificada previamente.

3. Funciones de la Librería

#include <omp.h>: Necesario para usar las funciones de OpenMP.
 - omp_get_thread_num(): Devuelve el identificador único del hilo que está ejecutando ese código en ese momento (ej. Hilo 0, Hilo 1, etc.).

4. Flujo de ejecución

 - Antes de la región paralela: El hilo maestro asigna contador = 99.
 - Entrada a Región 1 con copyin: Todos los hilos reciben contador = 99. Cada hilo modifica su propia copia (hilo 0 → 0, hilo 1 → 10, hilo 2 → 20).
 - Entre regiones: Cada hilo conserva su valor en memoria. El hilo maestro (hilo 0) muestra contador = 0.
 - Entrada a Región 2 sin copyin: Cada hilo retoma exactamente el valor que dejó en la región anterior. Esto confirma la persistencia: el valor no se reinicia, no se pierde y no lo sobreescribe otro hilo.
 - Final: Cada hilo acumula += 5 sobre su propio valor persistido, demostrando que el aislamiento y la persistencia funcionan juntos.

---

## Resultados

```
@ErikYair531 →/workspaces/Exposicion-alcance-de-variables (main) $ make run-thread
    make run-first    # FirstPrivate
    make run-last     # LastPrivate
    make              # compilar todo
    make clean        # limpiar binarios
./ThreadPrivate
=== ANTES DE CUALQUIER REGIÓN PARALELA ===
Hilo maestro: contador = 99

=== REGIÓN PARALELA 1 (copyin inicializa todos en 99) ===
Hilo 2: contador al entrar = 99
Hilo 2: contador después de modificar = 20
Hilo 1: contador al entrar = 99
Hilo 1: contador después de modificar = 10
Hilo 0: contador al entrar = 99
Hilo 0: contador después de modificar = 0

=== ENTRE REGIONES (hilo maestro) ===
Hilo maestro: contador = 0
(Cada hilo guarda su propio valor en memoria)

=== REGIÓN PARALELA 2 (sin copyin, cada hilo recuerda el suyo) ===
Hilo 0: contador persistido = 0
Hilo 0: contador después de += 5 → 5
Hilo 2: contador persistido = 20
Hilo 2: contador después de += 5 → 25
Hilo 1: contador persistido = 10
Hilo 1: contador después de += 5 → 15

=== RESULTADO FINAL (hilo maestro) ===
Hilo maestro: contador = 5

./FirstPrivate
Valor inicial fuera de la región paralela: first = 100
Hilo 3: copia local de first = 103
Hilo 2: copia local de first = 102
Hilo 1: copia local de first = 101
Hilo 0: copia local de first = 100
Después de la región paralela: first = 100

./LastPrivate
Valor inicial fuera de la región paralela: last = -1
Hilo 1 procesando iteración 4, last(private) = 4
Hilo 1 procesando iteración 5, last(private) = 5
Hilo 1 procesando iteración 6, last(private) = 6
Hilo 1 procesando iteración 7, last(private) = 7
Hilo 0 procesando iteración 0, last(private) = 0
Hilo 0 procesando iteración 1, last(private) = 1
Hilo 0 procesando iteración 2, last(private) = 2
Hilo 0 procesando iteración 3, last(private) = 3
Después de la región paralela: last = 7
```




---

## Conclusiones

**Nogueira Lopez Erik Yair**

Dentro de la práctica, presentación y código sobre el tema de las cláusulas de OpenMP se logró entender los tipos de comportamientos que tienen First, Last y Thread (private) para ejecuciones en paralelo y procesado de datos dentro de ciclos, a su vez el tipo de proceso que lleva a cabo cada cláusula con las variables que maneja.

---

**Sánchez de Tagle Caballero Diego Omar**

*Por escribir*

---

**Hernández Hernández David**

Antes de esta práctica, la programación paralela me parecía simplemente dividir un problema entre varios hilos. Lo que no tenía claro era que el verdadero reto está en controlar qué pasa con las variables: con qué valor entra cada hilo, qué hace con ella, y qué queda cuando termina.

Lo que más me llamó la atención fue `threadprivate`. A diferencia de las cláusulas, no vive dentro de una región paralela, sino que acompaña al hilo durante toda la ejecución del programa. Ver en el código cómo cada hilo recordaba su propio valor entre dos regiones paralelas distintas, sin que nadie se lo dijera explícitamente, hace que el concepto de "memoria por hilo" deje de ser abstracto y se vuelva algo concreto y visible.

También entendí que los errores en programación paralela no avisan: el programa compila, corre, y da un resultado que parece razonable pero está mal. Eso me enseñó que elegir entre `private`, `firstprivate`, `lastprivate` o `threadprivate` no es un detalle menor, es una decisión de diseño que define si el programa es correcto o no.

---

## Referencias

OpenMP. (s/f). Hpc-Wiki.Info. Recuperado el 26 de mayo de 2026, de https://hpc-wiki.info/hpc/OpenMP
OpenMP: For. (s/f). Jakascorner.com. Recuperado el 26 de mayo de 2026, de http://jakascorner.com/blog/2016/05/omp-for.html
TylerMSFT. (s/f). 2. Directivas. Microsoft.com. Recuperado el 26 de mayo de 2026, de https://learn.microsoft.com/es-es/cpp/parallel/openmp/2-directives?view=msvc-170
XL Fortran for AIX. (2023, julio 13). Ibm.com. https://www.ibm.com/docs/es/xl-fortran-aix/16.1.0?topic=fortran-routines-openmp

