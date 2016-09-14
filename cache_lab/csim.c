#include "cachelab.h"
#include "getopt.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"

#define MAXLINE 32

struct globalArgs_t {
  int s;   /* -s option */
  int e;   /* -E option */
  int b;   /* -b option */
  FILE *fp;   /* -t option */
} globalArgs;

static const char *optString = "s:E:b:t:";

struct line {
  unsigned time;
  int valid;
  long long tag;
};

int main(int argc, char *argv[])
{
  int hits = 0, misses = 0, evictions = 0;

  /* Initialize golbalArgs */
  int opt = 0;
  globalArgs.s = 0;
  globalArgs.e = 0;
  globalArgs.b = 0;
  globalArgs.fp = NULL;
  
  opt = getopt(argc, argv, optString);
  while (opt != -1) {
    switch (opt) {
    case 's':
      globalArgs.s = *optarg - '0';
      break;

    case 'E':
      globalArgs.e = *optarg - '0';
      break;

    case 'b':
      globalArgs.b = *optarg - '0';
      break;

    case 't':
      globalArgs.fp = fopen(optarg, "r");
    }
    opt = getopt(argc, argv, optString);
  }


  /* Initialize cache */
  int sets_num = 1 << globalArgs.s;
  struct line ***cache = malloc(sets_num * sizeof(struct line**));
  if (cache == NULL) return -1;

  int i,j;
  for (i = 0; i < sets_num; i++) {
    cache[i] = malloc(globalArgs.e * sizeof(struct line *));
    if (cache[i] == NULL) return -1;
    for (j = 0; j < globalArgs.e; j++) {
      cache[i][j] = malloc(sizeof(struct line));
      if (cache[i][j] == NULL) return -1;
      cache[i][j]->valid = 0;
    }
  }

  /* Deal with input */
  unsigned time_ticks = 0;
  char input[MAXLINE];
  if (globalArgs.fp == NULL) return -1;
  while (NULL != fgets(input, MAXLINE, globalArgs.fp)) {
    /* Parse instruction */
    if (input[0] != ' ') continue;
    char command_type = input[1];
    char *addr_start = input + 3;
    char *addr_str = strtok(addr_start,",");
    long long address = strtoll(addr_str, (char **)NULL, 16);
    //printf("c: %c, addr: %lld\n", command_type, address);

    int set_index = address >> globalArgs.b & ((1 << globalArgs.s) - 1);
    int tag = address >> (globalArgs.s + globalArgs.b);
    //printf("set_index: %d, tag: %d\n", set_index, tag);

    /* Check if hits in cache */
    struct line **set = cache[set_index];
    int is_hitted = 0;
    for (i = 0; i < globalArgs.e; i++) {
      if (set[i]->valid == 1 && set[i]->tag == tag) {
	set[i]->time = time_ticks++;
	is_hitted = 1;
	break;
      }
    }
    if (command_type == 'M') {
      if (is_hitted) {
	hits += 2;
	continue;
      } else {
	hits++;
	misses++;
      }
    } else {
      if (is_hitted) {
	hits++;
	continue;
      } else
	misses++;
    }
        
    /* Check if there is an empty line */
    int empty_index = -1;
    for (i = 0; i < globalArgs.e; i++) {
      if (set[i]->valid == 0) {
	empty_index = i;
	break;
      }
    }
    if (empty_index != -1) {
      set[empty_index]->time = time_ticks++;
      set[empty_index]->valid = 1;
      set[empty_index]->tag = tag;
      continue;
    }

    /* Evict a line in set */
    int evict_index = 0;
    for (i = 0; i < globalArgs.e; i++)
      if (set[i]->time < set[evict_index]->time)
	evict_index = i;
    set[evict_index]->time = time_ticks++;
    set[evict_index]->tag = tag;
    evictions++;
  }
  
  /* Free cache memory */
  for (i = 0; i < sets_num; i++) {
    for (j = 0; j < globalArgs.e; j++)
      free(cache[i][j]);
    free(cache[i]);
  }
  
  printSummary(hits, misses, evictions);
  return 0;
}
