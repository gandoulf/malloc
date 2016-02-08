//
// main.c for  in /home/agor_m/github/malloc/newMalloc
// 
// Made by Maxime Agor
// Login     <agor_m@epitech.net>
// 
// Started on   Sat Feb 6 13:56:51 2016 Maxime Agor
// Last update  Sat Feb 6 13:56:51 2016 Maxime Agor
//

#include "my_malloc.h"
#include <assert.h>
#include <stdio.h>

#define ALLOC_SIZE 100
#define ALLOC_NB 100


int main()
{
    void    *firstPtr;
    void    *ptr;
    int     i;

    firstPtr = malloc(ALLOC_SIZE);
    assert(firstPtr != 0);

    // Alloc ALLOC_NB blocs
    ptr = firstPtr;
    for(i = 0; i < ALLOC_NB; ++i)
    {
        ptr = malloc(ALLOC_SIZE);
        assert(ptr != 0);
    }

    printf("after %d alloc of %d bytes, pointer = %p. Initial pointer was %p\n",
        ALLOC_NB,
        ALLOC_SIZE,
        ptr,
        firstPtr);

    size_t  paddedSize = ALLOC_SIZE +
        ((ALLOC_SIZE % sizeof(int)) ?
            sizeof(int) - (ALLOC_SIZE % sizeof(int)) :
            0);

    printf("paddedSize: %zu\n", paddedSize);

    // Check the last pointer position
    assert(ptr - firstPtr == (paddedSize + sizeof(t_metadata)) * ALLOC_NB);


    // Free 10% to 20% of malloc'd memory
    ptr = firstPtr + (paddedSize + sizeof(t_metadata)) * (1 + ALLOC_NB / 10);
    for (i = 0; i < ALLOC_NB / 10; ++i)
    {
        free(ptr + (paddedSize + sizeof(t_metadata)) * i);
    }
    printf("%p to %p freed (%d bytes)\n",
        ptr,
        ptr + (paddedSize + sizeof(t_metadata)) * i,
        i * ALLOC_SIZE);

    show_alloc_mem();

    // Alloc 10% to 15% of memory
    ptr = malloc(ALLOC_SIZE * (ALLOC_NB / 20));
    // ptr = malloc(ALLOC_SIZE / 2);
    show_alloc_mem();

    // Check the pointer's position
    assert(ptr == firstPtr + (paddedSize + sizeof(t_metadata)) * (1 + ALLOC_NB / 10));

    void *tmp = ptr;
    // Realloc the pointer
    ptr = realloc(ptr, ALLOC_SIZE * (1 + ALLOC_NB / 20));
    show_alloc_mem();

    assert(ptr == tmp);

    return 0;
}
