#include <stdio.h>
#include <omp.h>
int fib(int n) {
 int x, y;
 if (n < 2)
 return n;
 #pragma omp task shared(x)
 x = fib(n - 1);
 #pragma omp task shared(y)
 y = fib(n - 2);
 #pragma omp taskwait
 return x + y;
}
int main() {
 int n;
 printf("Enter number of Fibonacci terms: ");
 scanf("%d", &n);
 printf("Fibonacci Series:\n");
 for (int i = 0; i < n; i++) {
 int result;
 #pragma omp parallel
 {
 #pragma omp single
 {
 result = fib(i);
 }
 }
 printf("%d ", result);
 }
 printf("\n");
 return 0;
}