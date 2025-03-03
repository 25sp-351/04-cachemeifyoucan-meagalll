#ifndef CUT_LIST_H
#define CUT_LIST_H

#include "piece_values.h"
#include "vec.h"

typedef struct pg {
    PieceLengthValue pv;  // Changed from PieceLength to PieceLengthValue
    int count;
} PieceGroup;

typedef struct cl {
    Vec groups;
    PieceLength remainder;
    PieceValue total_value;
} CutList;

// pass in "initial remainder"
// CutList *new_cutlist(PieceLength length);
CutList *new_cutlist(Vec length_and_values, PieceLength length);
void cutlist_free(CutList *cl);
void cutlist_print(CutList *cl);
CutList *optimal_cutlist_for(Vec pv, PieceLength total_length);

#endif  // CUT_LIST_H
