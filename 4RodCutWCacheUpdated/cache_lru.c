#include "cache.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    int* access_times;
    int current_time;
} LRUPolicyData;

Cache* create_cache(int capacity, const char* policy) {
    Cache* cache = malloc(sizeof(Cache));
    cache->entries = malloc(capacity * sizeof(CacheEntry));
    cache->capacity = capacity;
    cache->size = 0;

    if (strcmp(policy, "LRU") == 0) {
        LRUPolicyData* lru_data = malloc(sizeof(LRUPolicyData));
        lru_data->access_times = malloc(capacity * sizeof(int));
        lru_data->current_time = 0;
        cache->policy_data = lru_data;
    } else {
        // FIFO policy data (if needed)
        cache->policy_data = NULL;
    }

    return cache;
}

CutList* get_cached_cutlist(Cache* cache, PieceLength length, Vec pv) {
    LRUPolicyData* lru_data = (LRUPolicyData*)cache->policy_data;

    // Check if the cutlist is already in the cache
    for (int i = 0; i < cache->size; i++) {
        if (cache->entries[i].length == length) {
            lru_data->access_times[i] = lru_data->current_time++;
            return cutlist_copy(cache->entries[i].cutlist);
        }
    }

    // If not in cache, compute the cutlist and add it to the cache
    CutList* cutlist = optimal_cutlist_for(pv, length);

    if (cache->size < cache->capacity) {
        cache->entries[cache->size].length = length;
        cache->entries[cache->size].cutlist = cutlist_copy(cutlist);
        lru_data->access_times[cache->size] = lru_data->current_time++;
        cache->size++;
    } else {
        // Find the least recently used entry
        int lru_index = 0;
        for (int i = 1; i < cache->size; i++) {
            if (lru_data->access_times[i] < lru_data->access_times[lru_index]) {
                lru_index = i;
            }
        }

        // Replace the LRU entry
        cutlist_free(cache->entries[lru_index].cutlist);
        cache->entries[lru_index].length = length;
        cache->entries[lru_index].cutlist = cutlist_copy(cutlist);
        lru_data->access_times[lru_index] = lru_data->current_time++;
    }

    return cutlist;
}

void free_cache(Cache* cache) {
    for (int i = 0; i < cache->size; i++) {
        cutlist_free(cache->entries[i].cutlist);
    }
    free(cache->entries);

    if (cache->policy_data) {
        LRUPolicyData* lru_data = (LRUPolicyData*)cache->policy_data;
        free(lru_data->access_times);
        free(lru_data);
    }

    free(cache);
}