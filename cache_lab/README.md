# Part A
In this part our task is to implement a cache simulator which could handle following command line options:
```
Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>
• -h: Optional help flag that prints usage info
• -v: Optional verbose flag that displays trace info
• -s <s>: Number of set index bits (S = 2s
is the number of sets)
• -E <E>: Associativity (number of lines per set)
• -b <b>: Number of block bits (B = 2b
is the block size)
• -t <tracefile>: Name of the valgrind trace to replay
```

`getopt` is very useful for parsing command line arguments. To simulate LRU strategy, 
I add a time field in every line in cache and update it when it's touched:  
```c
struct line {
  unsigned time;
  int valid;
  long long tag;
};
```
See more [here](https://github.com/CtheSky/CSAPP-lab/blob/master/cache_lab/csim.c)

# Part B
In this part our task is to optimize matrix transpose, cache misses will be counted and scored as following:
```
• 32 × 32: 8 points if m < 300, 0 points if m > 600
• 64 × 64: 8 points if m < 1, 300, 0 points if m > 2, 000
• 61 × 67: 10 points if m < 2, 000, 0 points if m > 3, 000
```
Our cache will run with s = 5, E = 1, b = 5  

### 32 x 32
TODO  

### 64 x 64
TODO 

### 61 x 67
TODO 
