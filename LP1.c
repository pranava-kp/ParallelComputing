#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define MAX 100000
void fill_arr(int *arr, int n)
{
    for(int i=0;i<n;i++)
        arr[i]=rand() % 1000;
}
void copy_array(int *arr1, int *arr2, int n)
{
    for(int i=0;i<n;i++)
        arr2[i]=arr1[i];
}
void merge(int arr[], int l, int m, int r)
{
    int n1=m-l+1;
    int n2=r-m;
    int *L=(int*)malloc(n1*(sizeof(int)));
    int *R=(int*)malloc(n1*(sizeof(int)));
    for(int i=0;i<n1;i++)
        L[i]=arr[l+i];
    for(int j=0;j<n2;j++)
        R[j]=arr[m+1+j];
    int i=0,j=0,k=l;
    while(i<n1 && j<n2)
        if(L[i]<=R[i])
            arr[k++]=L[i++];
        else
            arr[k++]=R[j++];
    while(i<n1)
        arr[k++]=L[i++];
    while(j<n2)
        arr[k++]=R[j++];
    free(L);
    free(R);
}
void sequentialMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
void parallelMergeSort(int arr[], int l, int r,int depth) {
    if (l < r) {
        int m = (l + r) / 2;
        if(depth<=0)
        {
            sequentialMergeSort(arr,l,m);
            sequentialMergeSort(arr,m+1,r);
        }
        else
        {
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, l, m,depth-1);
                #pragma omp section
                parallelMergeSort(arr, m + 1, r,depth-1);
            }
        }
        merge(arr, l, m, r);
    }
}
int main() {
    int n;
    printf("Enter number of elements upto MAX\n");
    scanf("%d",&n);
    // if(n>MAX)
    // {
    //     printf("Array is too large\n");
    //     return 1;
    // }
    int* arr_seq=(int*)malloc(n*sizeof(int));
    int* arr_par=(int*)malloc(n*sizeof(int));

    fill_arr(arr_seq,n);
    copy_array(arr_seq,arr_par,n);
    
    printf("N = %d\n",n);

    double start_seq=omp_get_wtime();
    sequentialMergeSort(arr_seq, 0, n - 1);
    double end_seq=omp_get_wtime();

    printf("Sequential Merge Sort Time: %f seconds\n", end_seq - start_seq);

    int depth=4;
    double start_par = omp_get_wtime();
    parallelMergeSort(arr_par, 0, n - 1,depth);
    double end_par = omp_get_wtime();

    printf("Parallel Merge Sort Time: %f seconds\n", end_par - start_par);

    return 0;
}