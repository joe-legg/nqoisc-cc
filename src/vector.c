#include <stdlib.h>
#include "vector.h"

Vector *new_vector()
{
    Vector *vector = malloc(sizeof(Vector));
    // Initial capacity
    vector->capacity = 4;
    vector->items = malloc(sizeof(void *) * vector->capacity);
    vector->length = 0;
    return vector;
}

void vector_append(Vector *vect, void *item)
{
    if (vect == NULL) return;

    // Resize the vector
    if (vect->length == vect->capacity) {
        vect->capacity *= 2;
        vect->items = realloc(vect->items, vect->capacity * sizeof(void *));
    }

    vect->items[vect->length++] = item;
}

void vector_delete(Vector *vect, int index)
{
    if (vect == NULL) return;
    for (int i = index; i < vect->length; i++)
        vect->items[i] = vect->items[i + 1];
    vect->length--;
}

void vector_free(Vector *vect)
{
    if (vect == NULL) return;
    free(vect->items);
    free(vect);
}
