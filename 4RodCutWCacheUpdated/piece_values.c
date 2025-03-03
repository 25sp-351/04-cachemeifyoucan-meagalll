#include "piece_values.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "vec.h"

bool _list_contains_length(Vec ptr, PieceLength len);

// by value then length, both descending
int compare_piece_values(PieceLengthValue *a, PieceLengthValue *b) {
    if (a->value != b->value)
        return b->value - a->value;
    return b->length - a->length;
}

Vec read_piece_values() {
    Vec val_list = new_vec(sizeof(PieceLengthValue));

    while (!feof(stdin)) {

        PieceLengthValue item;

        if (2 != fscanf(stdin, "%d, %d\n", &item.length, &item.value) ||
            item.length < 1 || item.value < 1 ||
            _list_contains_length(val_list, item.length)) {
            vec_free(val_list);
            return NULL;
        }

        vec_add(val_list, &item);
    }

    if (val_list->length == 0) {
        vec_free(val_list);
        return NULL;
    }
    qsort(vec_items(val_list), val_list->length, sizeof(PieceLengthValue),
          (int (*)(const void *, const void *))compare_piece_values);

    return val_list;
}

bool _list_contains_length(Vec v, PieceLength len) {
    PieceLengthValue *items = vec_items(v);
    for (int ix = 0; ix < v->length; ix++)
        if (items[ix].length == len)
            return true;

    return false;
}

void print_piece_values(Vec v) {
    PieceLengthValue *items = vec_items(v);
    for (int ix = 0; ix < v->length; ix++)
        printf("length %4d, value %4d\n", items[ix].length, items[ix].value);
}

bool pvs_equal(PieceLengthValue *pv1, PieceLengthValue *pv2) {
    return pv1->length == pv2->length && pv1->value == pv2->value;
}
