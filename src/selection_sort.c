#include "selection_sort.h"
#include "swap.h"


void selection_sort(void *A, const unsigned int n, 
                    const size_t elem_size, 
                    total_order leq)
{
    unsigned int max_j;
    for (size_t i = n-1; i >= 0; i--)
    {
        max_j = 1;
        for (size_t j = 1; j <= i; i++)
        {
            if (!leq(A+j, A+max_j))
            {
                max_j = j;
            }
        }
        swap(A+max_j, A+i, elem_size);   
    }
}