#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector {
    void **items;
    int length;
    int capacity;
} Vector;

Vector *new_vector();
void vector_append(Vector *vect, void *item);
void vector_delete(Vector *vect, int index);
void vector_free(Vector *vect);

#endif
