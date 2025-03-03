#include "cache.h"
#include <stdlib.h>

typedef struct {
    int next_replace_index;
} FIFOPolicyData;

Cache* create_cache(int capacity, const char* policy) {
    Cache* cache = malloc(sizeof(Cache));
    cache->entries = malloc(capacity * sizeof(CacheEntry));
    cache->capacity = capacity;
    cache->size = 0;

    if (strcmp(policy, "FIFO") == 0) {
        FIFOPolicyData* fifo_data = malloc(sizeof(FIFOPolicyData));
        fifo_data->next_replace_index = 0;
        cache->policy_data = fifo_data;
    } else {
        // LRU policy data (if needed)
        cache->policy_data = NULL;
    }

    return cache;
}

CutList* get_cached_cutlist(Cache* cache, PieceLength length, Vec pv) {
    FIFOPolicyData* fifo_data = (FIFOPolicyData*)cache->policy_data;

    // Check if the cutlist is already in the cache
    for (int i = 0; i < cache->size; i++) {
        if (cache->entries[i].length == length) {
            return cutlist_copy(cache->entries[i].cutlist);
        }
    }

    // If not in cache, compute the cutlist and add it to the cache
    CutList* cutlist = optimal_cutlist_for(pv, length);

    if (cache->size < cache->capacity) {
        cache->entries[cache->size].length = length;
        cache->entries[cache->size].cutlist = cutlist_copy(cutlist);
        cache->size++;
    } else {
        // Replace the oldest entry
        int replace_index = fifo_data->next_replace_index;
        cutlist_free(cache->entries[replace_index].cutlist);
        cache->entries[replace_index].length = length;
        cache->entries[replace_index].cutlist = cutlist_copy(cutlist);
        fifo_data->next_replace_index = (replace_index + 1) % cache->capacity;
    }

    return cutlist;
}

void free_cache(Cache* cache) {
    for (int i = 0; i < cache->size; i++) {
        cutlist_free(cache->entries[i].cutlist);
    }
    free(cache->entries);

    if (cache->policy_data) {
        FIFOPolicyData* fifo_data = (FIFOPolicyData*)cache->policy_data;
        free(fifo_data);
    }

    free(cache);
}