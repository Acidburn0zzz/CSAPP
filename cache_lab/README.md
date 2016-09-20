# Part A
In this part our task is to implement a cache simulator which could handle following command line options:
```
Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>
• -h: Optional help flag that prints usage info
• -v: Optional verbose flag that displays trace info
• -s <s>: Number of set index bits (S = 2s is the number of sets)
• -E <E>: Associativity (number of lines per set)
• -b <b>: Number of block bits (B = 2b is the block size)
• -t <tracefile>: Name of the valgrind trace to replay
```

`getopt` is very useful for parsing command line arguments. To simulate LRU strategy, 
I add a time field in every line in cache and update it when it's touched. Time is incremented every instruction
so when I have to evict a line, I could search for the line with smallest time to evict:  
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
Notice b = 5 that means each block has 32 bytes which could hold 8 integers, and s = 5 means we have 32 sets.
So each row will map to 4 lines and their line index will look like following:
  
0 x8 | 1 x8 | 2 x8 | 3 x8 |
--- | --- | --- | --- | ---
4 x8 | 5 x8 | 6 x8 | 7 x8 |
8 x8 | 9 x8 | 10 x8 | 11 x8 |
... | ... | ... | ... |

So a good way to get space  locality is divide matrix in to 8x8 block because copy first row in A to B will 
repare B for the coming 7 rows' copy.
  
And there is a special case, notice this cache map duplicates every 8 rows and thus our two matrices will have exactly
same map pattern.
And when we do the copy on diagonal elements like this:
```c
B[j][i] = A[i][j];
```
Write in B will evict A's line in cache and cause a lot of misses. So we could use 8 temp variables to store those
elements and copy them at last.
```c
for (n = 0; n < N; n +=8)
      for (m = 0; m < M; m += 8)
	for (i = n; i < n + 8; i++) {
	  for (j = m; j < m + 8; j++) {
	    if (i == j)
	      temp[i % 8] = A[i][j];
	    else
	      B[j][i] = A[i][j];
	  }
	  if (n == m) {
	    for (j = n; j < n + 8; j++)
	      B[j][j] = temp[j % 8];
	  }
	} 
```
### 64 x 64
Dividing into 8x8 block doesn't work well here because this time each row has 64 integers 
which means we have 8 lines for each row and cache map duplicates every 4 rows. So when  we copy a line of 8 integers
the later 4 will evict first 4 lines.   
  
We could try 4x4 bolck but it's not good enough. The reason is that when we copy first 4 elements from a line, 
actually we could get next 4 elements with no miss but we don't exploit this fact.  
  
To exploit that, divide matrix into 8x8 blocks and in each 8x8 block, divide it into 4x4 blocks.
Suppose there is a, b, c, d and each of this is a 4x4 block.
```
For A:     For B:
|a0|b0|     |a1|b1|
|c0|d0|     |c1|d1|

   t(a) means transpose of a
```
Steps:  
1. copy t(a0) -> a1.  
2. copy t(b0) -> b1 (b0 and b1 are actually in cache so there is no miss)  
3. copy b1 -> c1, t(c0) -> b1 (for each line in b1, use existing value to fill c1 and read c0 to fill this line)   
4. copy t(d0) -> d1  

See more [here](https://github.com/CtheSky/CSAPP-lab/blob/master/cache_lab/trans.c)

### 61 x 67
It costs time to calculate every exact cache mapping so I reuse the method used in 64x64 and apply to a 56x64 block.
I try to put in 4 corners of matrix and when it's in top right there is only 2103 misses. And I change rest parts
many times and finally get an exact 2000 misses. It's more like trying luck than coding. 
See more [here](https://github.com/CtheSky/CSAPP-lab/blob/master/cache_lab/trans.c)
