#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int parallel_program(int threads, int random_seed)
{
    const int count = 10000000;     ///< Number of array elements
    //const int threads = 16;         ///< Number of parallel threads to use
    //const int random_seed = 920215; ///< RNG seed

    int* array = 0;                 ///< The array we need to find the max in
    int  max   = -1;                ///< The maximal element

    /* Initialize the RNG */
    srand(random_seed);

    /* Determine the OpenMP support */
    printf("OpenMP: %d;\n======\n", _OPENMP);

    /* Generate the random array */
    array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { array[i] = rand(); }

    /* Find the maximal element */
    double start_time = omp_get_wtime();
    #pragma omp parallel num_threads(threads) shared(array, count) reduction(max: max) default(none)
    {
        #pragma omp for
        for(int i=0; i<count; i++)
        {
            if(array[i] > max) { max = array[i]; };
        }
        //printf("-- My lmax is: %d;\n", max);
        // printf("-%d- My max is: %d;\n", omp_get_thread_num() + 1, max);
    }
    double end_time = omp_get_wtime();
    //printf("======\nMax is: %d;\n", max);

    printf("%f\n", end_time - start_time);
    free(array);
    return(0);
}

int main(){
    printf("%d\n", omp_get_max_threads());


    for (int i = 1; i <= 12; i++) {
        int random_seed = 920215;
        for (int j = 0; j < 10; j++) {
            parallel_program(i, random_seed + j);
        }
    }

    return 0;
}