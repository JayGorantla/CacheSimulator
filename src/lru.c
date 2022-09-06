#include "lru.h"
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "math.h"

void lru_init_queue(Set *set) {
  LRUNode *s = NULL;
  LRUNode **pp = &s;  // place to chain in the next node
  for (int i = 0; i < set->line_count; i++) {
    Line *line = &set->lines[i];
    LRUNode *node = (LRUNode *)(malloc(sizeof(LRUNode)));
    node->line = line;
    node->next = NULL;
    (*pp) = node;
    pp = &((*pp)->next);
  }
  set->lru_queue = s;
}

void lru_init(Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    lru_init_queue(&sets[i]);
  }
}

void lru_destroy(Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    LRUNode *p = sets[i].lru_queue;
    LRUNode *n = p;
    while (p != NULL) {
      p = p->next;
      free(n);
      n = p;
    }
    sets[i].lru_queue = NULL;
  }
}

void lru_fetch(Set *set, unsigned int tag, LRUResult *result) {
  // TODO:
  // Implement the LRU algorithm to determine which line in
  // the cache should be accessed.
  //
  LRUNode *node = set->lru_queue;
  LRUNode *prev = NULL;
  int flag = 0;

  while (node != NULL) {
    if (node->line->tag == tag && node->line->valid == 'T') {
      result->line = node->line;
      result->access = HIT;
      flag = 1;
      break;
    } else {
      prev = node;
      node = node->next;
    }
  }

  if (flag == 0) {
    node = set->lru_queue;
    prev = NULL;

    while (node != NULL && node->line->valid == 'T') {
      node = node->next;
    }

    if (node != NULL) {
      //cold miss
      result->line = node->line;
      result->access = COLD_MISS;
      node->line->tag = tag;
      node->line->valid = 'T';
    } else {
      //conflict miss
      node = set->lru_queue;
      prev = NULL;
      while (node->next != NULL) {
        prev = node;
        node = node->next;
      }

      result->line = node->line;
      result->access = CONFLICT_MISS;
      node->line->tag = tag;
      node->line->valid = 'T';
    }

  }

  if (prev != NULL) {
    prev->next = node->next;
    node->next = set->lru_queue;
    set->lru_queue = node;
  }

}
