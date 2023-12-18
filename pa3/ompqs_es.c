// OpenMP quicksort; not necessarily efficient
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void swap(int *yi, int *yj)
{
    int tmp = *yi;
    *yi = *yj;
    *yj = tmp;
}

int separate(int *x, int low, int high)
{
    int i, pivot, last;
    pivot = x[low];
    swap(x+low,x+high);
    last = low;
    for (i = low; i < high; i++) {
        if (x[i] <= pivot) {
            swap(x+last,x+i);
            last += 1;
        }
    }
    swap(x+last,x+high);
    return last;
}

// quicksort of the array z, elements zstart through zend; set the
// latter to 0 and m-1 in first call, where m is the length of z;
// firstcall is 1 or 0, according to whether this is the first of the
// recursive calls
void qs(int *z, int zstart, int zend, int firstcall)
{
    #pragma omp parallel
    {
        int part;
        if (firstcall == 1) {
            #pragma omp single nowait
            qs(z,0,zend,0);
        } else {
            if (zstart < zend) {
                part = separate(z,zstart,zend);

                /**
                 * switch from an orientation of every recursive call
                 * being made into a task to a a simple switch to 
                 * sequential fo the smaller subarrays
                */
                if ((part - 1 - zstart) <= 100){
                    qs(z,zstart,part - 1, 0);
                } else {
                    #pragma omp task
                    qs(z,zstart,part-1,0);
                }

                if ((zend - part - 1) <= 100)
                {
                    qs(z, part + 1, zend, 0);
                }
                else
                {
                    #pragma omp task
                    qs(z, part + 1, zend, 0);
                }
            }
        }
    }
}

/**
 * Comparison for the qsort
 * Takes the pointers to two elements as parameters
 * type cast both as int
 * if a > b then positive return
 * if a < b then negative return
 * if a = b then return 0
*/
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// test code
int main(int argc, char **argv)
{
    //W copy ensures comparisons are done with identical data
    int i, n, *w, *w_copy;
    double start,end;

    if (argc < 2) {
        printf("Usage: omp_quicksort <n>, where n is the number of items to be sorted\n");
	    exit(-1);
    }
    n = atoi(argv[1]);

    /**
     * initialize both w and its copy, but parallelize the allocation
     * using single nowait the following code is executed by one thread
     * and does not wait to execute.
     * 
     * omp task just defines the code lines that follow as a specific task
    */
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            #pragma omp task
            w = malloc(n * sizeof(int));

            #pragma omp task
            w_copy = malloc(n * sizeof(int));
        }
        #pragma omp taskwait
    }


    //For each random assign to w and its copy'
    /**
     * dynamic: Each thread executes a chunk of
     * iterations then requests another chunk until
     * none remain.
     * 
     * using a chunck size of 128
    */
    #pragma omp for schedule(dynamic, 128)
    for (i = 0; i < n; i++){
        w[i] = rand();
        w_copy[i] = w[i];
    }

    //This one for the omp_quicksort implementation
    start = omp_get_wtime();
    qs(w,0,n-1,1);
    end = omp_get_wtime();
    
    if (n < 25)
        for (i = 0; i < n; i++) printf("%d\n",w[i]);
    printf("Elapsed time of omp: %f sec\n", end-start);

    //This copy for the qsort implementation
    start = omp_get_wtime();
    qsort(w,0,n-1,compare);
    end = omp_get_wtime();

    if (n < 25)
        for (i = 0; i < n; i++) printf("%d\n",w[i]);
    printf("Elapsed time qsort: %f sec\n", end-start);
}
