#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
  double *b;
  double d;
  int i;
  int it;
  int m;
  int n;
  double r;
  double t;
  double *x;
  double *xnew;
  double start, end;

  m = 10000;
  n = 50000;

  b = (double *)malloc(n * sizeof(double));
  x = (double *)malloc(n * sizeof(double));
  xnew = (double *)malloc(n * sizeof(double));

  printf("\n");
  printf("JACOBI_OPENMP:\n");
  printf("  C/OpenMP version\n");
  printf("  Jacobi iteration to solve A*x=b.\n");
  printf("\n");
  printf("  Number of variables  N = %d\n", n);
  printf("  Number of iterations M = %d\n", m);
  printf("\n");

  #pragma omp parallel private(i)
  {
    /**
     *  Set up the right hand side.
    */
    #pragma omp for
    for (i = 0; i < n; i++)
    {
      b[i] = 0.0;
    }

    b[n - 1] = (double)(n + 1);

    /**
     * Initialize the solution estimate to 0.
    */
    #pragma omp for
    for (i = 0; i < n; i++)
    {
      x[i] = 0.0;
    }
  }

  start = omp_get_wtime();
  for (it = 0; it < m; it++)
  {
    /**
     * Create a parallel omp implementation
     * make private i and t
     * reduction on d
     * reudction on r
    */
    #pragma omp parallel private(i, t) reduction(+:d) reduction(+:r)
    {
      /**
       * Jacobi update.
      */
      #pragma omp for
      for (i = 0; i < n; i++)
      {
        xnew[i] = b[i];
        if (0 < i)
        {
          xnew[i] += x[i - 1];
        }
        if (i < n - 1)
        {
          xnew[i] += x[i + 1];
        }
        xnew[i] /= 2.0;
      }

      /**
       * Difference.
      */
      #pragma omp for
      for (i = 0; i < n; i++)
      {
        d += pow(x[i] - xnew[i], 2);
      }

      /**
       * Overwrite old solution.
      */
      #pragma omp for
      for (i = 0; i < n; i++)
      {
        x[i] = xnew[i];
      }

      /**
       * Residual.
      */
      #pragma omp for
      for (i = 0; i < n; i++)
      {
        t = b[i] - 2.0 * x[i];
        if (0 < i)
        {
          t += x[i - 1];
        }
        if (i < n - 1)
        {
          t += x[i + 1];
        }
        r += t * t;
      }
    }
  }
  end = omp_get_wtime();
  printf("Time for Jacobi iteration: %f seconds\n", end - start);

  /**
   *  Write part of final estimate.
  */
  printf("\n");
  printf("  Part of final solution estimate:\n");
  printf("\n");
  for (i = 0; i < 10; i++)
  {
    printf("  %8d  %14.6g\n", i, x[i]);
  }
  printf("...\n");
  for (i = n - 11; i < n; i++)
  {
    printf("  %8d  %14.6g\n", i, x[i]);
  }

  /**
   * Free memory.
  */
  free(b);
  free(x);
  free(xnew);

  /**
   * Terminate.
  */
  printf("\n");
  printf("JACOBI_OPENMP:\n");
  printf("  Normal end of execution.\n");

  return 0;
}
