#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#include "piece_values.h"
#include "cut_list.h"
#include "vec.h"
#include "cache.h"

void usage(char *program_name);

int main(int argc, char *argv[]) {
    if (argc != 3 || !strcmp(argv[1], "-h"))
        usage(argv[0]);

    const char* policy = argv[1];
    const char* filename = argv[2];

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    Vec value_list = new_vec(sizeof(PieceLengthValue));

    PieceLengthValue item;
    while (fscanf(file, "%d, %d\n", &item.length, &item.value) == 2) {
        if (item.length < 1 || item.value < 1 || _list_contains_length(value_list, item.length)) {
            fprintf(stderr, "Error in value list.\n");
            fclose(file);
            vec_free(value_list);
            return 1;
        }
        vec_add(value_list, &item);
    }
    fclose(file);

    if (value_list->length == 0) {
        fprintf(stderr, "Error: No valid length/value pairs read.\n");
        vec_free(value_list);
        return 1;
    }

    Cache* cache = create_cache(10, policy); // Cache capacity of 10

    PieceLength rod_length;
    while (scanf("%d", &rod_length) == 1) {
        if (rod_length < 1) {
            fprintf(stderr, "Error: Invalid rod length.\n");
            continue;
        }

        CutList* cl = get_cached_cutlist(cache, rod_length, value_list);
        cutlist_print(cl);
        cutlist_free(cl);
    }

    free_cache(cache);
    vec_free(value_list);

    return 0;
}

noreturn void usage(char *program_name) {
    fprintf(stderr,
            "Usage:\n"
            "%s [-h] policy filename\n"
            "policy = LRU or FIFO\n"
            "filename = file containing length/value pairs\n",
            program_name);
    exit(1);
}