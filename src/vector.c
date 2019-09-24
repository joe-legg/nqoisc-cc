#include <stdlib.h>
#include "vector.h"

Vector *new_vector()
{
    Vector *vector = malloc(sizeof(Vector));
    vector->items = NULL;
    vector->length = 0;
    return vector;
}

void vector_append(Vector *vect, void *item)
{
    vect->items = realloc(vect->items, ++vect->length * sizeof(void *));
    vect->items[vect->length - 1] = item;
}

void vector_delete(Vector *vect, int index)
{
    for (int i = index; i < vect->length; i++)
        vect->items[i] = vect->items[i + 1];
    vect->length--;
}

void vector_free(Vector *vect)
{
    free(vect->items);
    free(vect);
}
