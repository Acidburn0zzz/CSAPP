# Part A
In this part our task is to optimizing `rotate` function, this one is more memory-sensitive and less compute-intensive
than `smooth` in next part.

#### Naive Rotate
```c
/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}
```

#### Naive ji Rotate
My first try is to access order from ij to ji, we will get more write hit and less read hit.
Because write is more expensive than read and we can also benefit from WriteBack, there is an obviouus improvement.
```c
/*
 * ji naive rotate: more write hit, less read hit
 */
char naive_ji_rotate_descr[] = "ji naive rotate: more write hit, less read hit- benefit from write back";
void naive_ji_rotate(int dim, pixel *src, pixel *dst)
{
  int i, j;

  for (j = 0; j < dim; j++)
    for (i = 0; i < dim; i++)
      dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}
```

#### Unroll ji Rotate
My next step is to unroll the ji rotate. Since N is a multiple of 32, I choose to unroll by 32 but 
it turns out that 16 is a better choice and I can't tell the exact reason.
```c
/*
 * ji naive rotate with unrolling
 */
#define copy(a) dst[RIDX(dim-1-j, i+(a), dim)] = src[RIDX(i+(a), j, dim)];
char unroll_ji_rotate_descr[] = "ji unrolling rotate";
void unroll_ji_rotate(int dim, pixel *src, pixel *dst)
{
   int i, j;

    for (i = 0; i < dim; i += 16) 
        for (j = 0; j < dim; j++) {
            copy(0);copy(1);copy(2);copy(3);
            copy(4);copy(5);copy(6);copy(7);
            copy(8);copy(9);copy(10);copy(11);
            copy(12);copy(13);copy(14);copy(15);
        }
}
```

# Part B
In this part our task is to optimizing `smooth` function, this one is more compute-intensive and less memory-sensitive
than `rotate`.
  
My solution is a little bruteforce that I use hard code to replace all that function calls, but it works well.
```c
/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j, rij;
    int rindex = dim;
    
    // Outmost layer (corner cases)
    dst[0].red = (src[0].red+src[1].red+src[dim].red+src[dim+1].red)>>2;
    dst[0].blue = (src[0].blue+src[1].blue+src[dim].blue+src[dim+1].blue)>>2;
    dst[0].green = (src[0].green+src[1].green+src[dim].green+src[dim+1].green)>>2;
    dst[dim-1].red = (src[dim-1].red+src[dim-2].red+src[dim*2-1].red+src[dim*2-2].red)>>2;
    dst[dim-1].blue = (src[dim-1].blue+src[dim-2].blue+src[dim*2-1].blue+src[dim*2-2].blue)>>2;
    dst[dim-1].green = (src[dim-1].green+src[dim-2].green+src[dim*2-1].green+src[dim*2-2].green)>>2;

    dst[dim*(dim-1)].red = (src[dim*(dim-1)].red+src[dim*(dim-1)+1].red+src[dim*(dim-2)].red+src[dim*(dim-2)+1].red)>>2;
    dst[dim*(dim-1)].blue = (src[dim*(dim-1)].blue+src[dim*(dim-1)+1].blue+src[dim*(dim-2)].blue+src[dim*(dim-2)+1].blue)>>2;
    dst[dim*(dim-1)].green = (src[dim*(dim-1)].green+src[dim*(dim-1)+1].green+src[dim*(dim-2)].green+src[dim*(dim-2)+1].green)>>2;

    dst[dim*dim-1].red = (src[dim*dim-1].red+src[dim*dim-2].red+src[dim*(dim-1)-1].red+src[dim*(dim-1)-2].red)>>2;
    dst[dim*dim-1].blue = (src[dim*dim-1].blue+src[dim*dim-2].blue+src[dim*(dim-1)-1].blue+src[dim*(dim-1)-2].blue)>>2;
    dst[dim*dim-1].green = (src[dim*dim-1].green+src[dim*dim-2].green+src[dim*(dim-1)-1].green+src[dim*(dim-1)-2].green)>>2;
    for (j = 1; j < dim-1; j++)
    {
        dst[j].red = (src[j].red+src[j-1].red+src[j+1].red+src[j+dim].red+src[j+1+dim].red+src[j-1+dim].red)/6;
        dst[j].green = (src[j].green+src[j-1].green+src[j+1].green+src[j+dim].green+src[j+1+dim].green+src[j-1+dim].green)/6;
        dst[j].blue = (src[j].blue+src[j-1].blue+src[j+1].blue+src[j+dim].blue+src[j+1+dim].blue+src[j-1+dim].blue)/6;
    }
    for (j = dim*(dim-1)+1; j < dim*dim-1; j++)
    {
        dst[j].red = (src[j].red+src[j-1].red+src[j+1].red+src[j-dim].red+src[j+1-dim].red+src[j-1-dim].red)/6;
        dst[j].green = (src[j].green+src[j-1].green+src[j+1].green+src[j-dim].green+src[j+1-dim].green+src[j-1-dim].green)/6;
        dst[j].blue = (src[j].blue+src[j-1].blue+src[j+1].blue+src[j-dim].blue+src[j+1-dim].blue+src[j-1-dim].blue)/6;
    }
    for (j = dim; j < dim*(dim-1); j+=dim)
    {
        dst[j].red = (src[j].red+src[j-dim].red+src[j+1].red+src[j+dim].red+src[j+1+dim].red+src[j-dim+1].red)/6;
        dst[j].green = (src[j].green+src[j-dim].green+src[j+1].green+src[j+dim].green+src[j+1+dim].green+src[j-dim+1].green)/6;
        dst[j].blue = (src[j].blue+src[j-dim].blue+src[j+1].blue+src[j+dim].blue+src[j+1+dim].blue+src[j-dim+1].blue)/6;
    }
    for (j = dim+dim-1; j < dim*dim-1; j+=dim)
    {
        dst[j].red = (src[j].red+src[j-1].red+src[j-dim].red+src[j+dim].red+src[j-dim-1].red+src[j-1+dim].red)/6;
        dst[j].green = (src[j].green+src[j-1].green+src[j-dim].green+src[j+dim].green+src[j-dim-1].green+src[j-1+dim].green)/6;
        dst[j].blue = (src[j].blue+src[j-1].blue+src[j-dim].blue+src[j+dim].blue+src[j-dim-1].blue+src[j-1+dim].blue)/6;
    }

    // Inner layers
    for (i = 1; i < dim-1; i++)
    {
        for (j = 1; j < dim-1; j++)
        {
            rij = rindex+j;
            dst[rij].red = (src[rij].red+src[rij-1].red+src[rij+1].red+src[rij-dim].red+src[rij-dim-1].red+src[rij-dim+1].red+src[rij+dim].red+src[rij+dim+1].red+src[rij+dim-1].red)/9;
            dst[rij].green = (src[rij].green+src[rij-1].green+src[rij+1].green+src[rij-dim].green+src[rij-dim-1].green+src[rij-dim+1].green+src[rij+dim].green+src[rij+dim+1].green+src[rij+dim-1].green)/9;
            dst[rij].blue = (src[rij].blue+src[rij-1].blue+src[rij+1].blue+src[rij-dim].blue+src[rij-dim-1].blue+src[rij-dim+1].blue+src[rij+dim].blue+src[rij+dim+1].blue+src[rij+dim-1].blue)/9;
        }
        rindex += dim;
    }
}
```
