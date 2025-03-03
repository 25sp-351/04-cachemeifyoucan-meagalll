#ifndef CACHE_H
#define CACHE_H

#include "cut_list.h"

typedef struct {
    PieceLength length;
    CutList* cutlist;
} CacheEntry;

typedef struct {
    CacheEntry* entries;
    int capacity;
    int size;
    // Policy-specific data
    void* policy_data;
} Cache;

Cache* create_cache(int capacity, const char* policy);
CutList* get_cached_cutlist(Cache* cache, PieceLength length, Vec pv);
void free_cache(Cache* cache);

#endif // CACHE_H