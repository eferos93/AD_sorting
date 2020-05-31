#include "select.h"
#include "quick_sort.h"
#include "swap.h"
#define ADDR(A, i, key_size) (A + (i) * (key_size))

int equal(const void *a, const void *b, total_order ord)
{
    return ord(a, b) && ord(b, a);
}

struct pair partition_3(void *A, size_t pivot, size_t left, size_t right, const size_t elem_size, total_order leq)
{

    struct pair bounds;
    swap(ADDR(A, pivot, elem_size), ADDR(A, left, elem_size), elem_size);
    pivot = left;
    left++;
    int repetition = 0;
    while (left <= right)
    {
        if (leq(ADDR(A, left, elem_size), ADDR(A, pivot, elem_size)) &&
            //!((leq(ADDR(A, i, elem_size), ADDR(A, pivot, elem_size))) && (leq(ADDR(A, pivot, elem_size), ADDR(A, i, elem_size))))
            !equal(ADDR(A, left, elem_size), ADDR(A, pivot, elem_size), leq))
        {
            swap(ADDR(A, left, elem_size), ADDR(A, pivot - repetition, elem_size), elem_size);
            pivot = left;
            left++;
        }
        else if (!(leq(ADDR(A, left, elem_size), ADDR(A, pivot, elem_size))))
        {
            swap(ADDR(A, left, elem_size), ADDR(A, right, elem_size), elem_size);
            right--;
        }
        else
        {
            pivot = left;
            left++;
            repetition++;
        }
    }

    swap(ADDR(A, pivot, elem_size), ADDR(A, right, elem_size), elem_size);
    bounds.second = right;
    bounds.first = right - repetition;
    return bounds;
}

unsigned int select_pivot(void *A, const unsigned int n,
                          const size_t elem_size, total_order leq)
{

    if (n <= 10)
    {
        quick_sort(A, n, elem_size, leq);
        return n / 2;
    }

    unsigned int chunks = n / 5, c_l;

    for (unsigned int c = 0; c < chunks; c++)
    {
        c_l = c * 5;
        quick_sort(A + c_l * elem_size, 5, elem_size, leq);
        swap(ADDR(A, c_l + 2, elem_size), ADDR(A, c, elem_size), elem_size);
    }

    return select_index(A, chunks, chunks / 2, elem_size, leq);
}

unsigned int select_index(void *A, const unsigned int n,
                          const unsigned int i,
                          const size_t elem_size,
                          total_order leq)
{
    if (n <= 10)
    {
        quick_sort(A, n, elem_size, leq);
        return i;
    }

    unsigned int j = select_pivot(A, n, elem_size, leq);
    struct pair k = partition_3(A, j, 0, n - 1, elem_size, leq);
    if (i < k.first)
    {
        return select_index(A, k.first - 1, i, elem_size, leq);
    }

    if (i > k.second)
    {
        return select_index(ADDR(A, k.second, elem_size), n - k.second - 1, i, elem_size, leq);
    }
    return i;
    
    /*
    unsigned int k = partition(A, j, 0, n-1, elem_size, leq);
    if(i==k)
    {
        return i;
    }

    if(i < k)
    {
        return select_index(A, k-1, i, elem_size, leq);
    }

    return select_index(A, n-1, k+1, elem_size, leq);
    */
}

void quick_sort_select_aux(void *A, size_t left, size_t right,
                           const size_t elem_size, total_order leq)
{
    while (left < right)
    {
        unsigned int pivot_index = left + select_pivot(ADDR(A, left, elem_size), right - left, elem_size, leq);
        struct pair k = partition_3(A, pivot_index, left, right - 1, elem_size, leq);
        quick_sort_select_aux(A, left, k.first, elem_size, leq);
        left = k.second + 1;
    }
}

void quick_sort_select(void *A, const unsigned int n,
                       const size_t elem_size,
                       total_order leq)
{
    quick_sort_select_aux(A, 0, n, elem_size, leq);
}
