Function | bitAnd
------------ | -------------
Example | bitAnd(6, 5) = 4
Legal ops | ~ \|
Max ops | 8
Rating | 1
Solution | Use De Morgan's laws :   x Λ y = ¬(¬(xy)) = ¬(¬x V ¬y)
```c
int bitAnd(int x, int y) {
  return ~(~x | ~y);
}
```

Function | getByte - Extract byte n from word x
------------ | -------------
Example | getByte(0x12345678,1) = 0x56
Legal ops | ! ~ & ^ \| + << >>
Max ops | 6
Rating | 2
Solution | Right shift x by 8 * n to move required byte to rightmost then take that byte by & 0xff
```c
int getByte(int x, int n) {
  return (x >> (n << 3)) & 0xff;
}
```

Function | logicalShift - shift x to the right by n, using a logical shift
------------ | -------------
Example | logicalShift(0x87654321,4) = 0x08765432
Legal ops | ! ~ & ^ \| + << >>
Max ops | 20
Rating | 3 
Solution | See comment in the code
```c
int logicalShift(int x, int n) {
  /* construct a mask with starting n positions filled with 0 and 1 for the rest 
     and then use & to set starting n positions to 0 
     split 1 << (32 - n) to 1 << (31 - n) << 1 for n = 0 */
  int mask = (1 << (32 + ~n) << 1) + ~0;
  return x >> n & mask;
}
```
Function | bitCount - returns count of number of 1's in word
------------ | -------------
Example | bitCount(5) = 2, bitCount(7) = 3
Legal ops | ! ~ & ^ \| + << >>
Max ops | 40
Rating | 4
Solution | See comment in the code
```c
int bitCount(int x) {
/*
It's easier to read as:  
  x = (x & 0x55555555) + ((x >> 1) & 0x55555555);  
  x = (x & 0x33333333) + ((x >> 2) & 0x33333333);  
  x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);  
  x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);  
  x = (x & 0x0000FFFF) + ((x >> 16)& 0x0000FFFF);  
Each step adds neighbouring bits together in groups of 1, then 2, then 4 etc. The method is based in divide and conquer.
If I have number 395 in binary    0000000110001011 (0 0 0 0 0 0 0 1 1 0 0 0 1 0 1 1)
After the first step I have:      0000000101000110 (0+0 0+0 0+0 0+1 1+0 0+0 1+0 1+1) = 00 00 00 01 01 00 01 10
In the second step I have:        0000000100010011 ( 00+00   00+01   01+00   01+10 ) = 0000 0001 0001 0011
In the fourth step I have:        0000000100000100 (   0000+0001       0001+0011   ) = 00000001 00000100
In the last step I have:          0000000000000101 (       00000001+00000100       )
which is equal to 5, which is the correct result*/
  int mask1,mask2,mask3,mask4,mask5;

  mask1 = 0x55 << 8 | 0x55;
  mask1 = mask1 << 16 | mask1;
  mask2 = 0x33 << 8 | 0x33;
  mask2 = mask2 << 16 | mask2;
  mask3 = 0x0F << 8 | 0x0F;
  mask3 = mask3 << 16 | mask3;
  mask4 = 0xFF << 16 | 0xFF;
  mask5 = 0xFF << 8 | 0xFF;

  x = (x & mask1) + ((x >> 1) & mask1);
  x = (x & mask2) + ((x >> 2) & mask2);
  x = (x & mask3) + ((x >> 4) & mask3);
  x = (x & mask4) + ((x >> 8) & mask4);
  x = (x & mask5) + ((x >> 16) & mask5);
  return x;
}
```
Function | bang - Compute !x without using !
------------ | -------------
Example | bang(3) = 0, bang(0) = 1
Legal ops | ~ & ^ \| + << >>
Max ops | 12
Rating | 4 
Solution | Exploit the fact that ~0 + 1 = 0 which makes it the only one starting with 0 but get 1 as first bit after ~(~x + 1), to filter out those nums starting with 1, just &(~x)
```c
int bang(int x) {
  return (((~x)&(~(~x + 1)))>>31) & 0x01;
}
```
Function | tmin - return minimum two's complement integer 
------------ | -------------
Example | bang(3) = 0, bang(0) = 1
Legal ops | ! ~ & ^ \| + << >>
Max ops | 4
Rating | 1
Solution | left shft 1 by 31 and get an int starting with an only 1-bit
```c
int tmin(void) {
  return 1 << 31;
}
```
Function | fitsBits - return 1 if x can be represented as an n-bit, two's complement integer. (1 <= n <= 32)
------------ | -------------
Example | fitsBits(5,3) = 0, fitsBits(-4,3) = 1
Legal ops | ! ~ & ^ \| + << >>
Max ops | 15
Rating | 2
Solution | See comment in the code
```c
int fitsBits(int x, int n) {
  /* The range with n bit is from -2^(n-1) to 2^(n-1)-1 
     and that means for the left 32 - n bits, we dont allow:
     it has 1 when positive and 0 when negative
     to check this condition, we first left shift by 32-n, then right shift 32 - n 
     it will reset left 32 - n bits to 0 when positive and 1 when negative
     and if result equals x, it's ok. We use a^a = 0 to check that instead of ==
     finally there is one more thing shouldn't be allowed which is 1-bit appears on nth bit
     when positive, but since we use arithmetical shift, our test includes this case */
  int shift = 33 + ~n;
  return !((x << shift >> shift) ^ x);
}
```
Function | divpwr2 - Compute x/(2^n), for 0 <= n <= 30 (Round toward zero)
------------ | -------------
Example | divpwr2(15,1) = 7, divpwr2(-33,4) = -2
Legal ops | ! ~ & ^ \| + << >>
Max ops | 15
Rating | 2
Solution | See comment in the code
```c
int divpwr2(int x, int n) {
  /* For negative x, right shift will round down while integer division requires round up
     to solve this problem, we could use the fact: 
     for any x and y > 0, ceil(x/y) = floor((x + y - 1) / y) */
  int sign = x >> 31 & 0x01;
  return (x + (sign << n) + ~sign + 1) >> n;
}
```
Function | negate - return -x 
------------ | -------------
Example | negate(1) = -1.
Legal ops | ! ~ & ^ \| + << >>
Max ops | 5
Rating | 2
Solution | See comment in the code
```c
int negate(int x) {
  /* Consider x + ~x + 1, if rightmost 1 in x is position n, ~x + 1 will have 1 in n
     and every bit reverse to x in left positions which means we will get a carry for all
     left positions. So the result will be 10000...(w of 0), the first bit will be omit 
     and we get zero. */
  return ~x + 1;
}
```
Function | isPositive - return 1 if x > 0, return 0 otherwise 
------------ | -------------
Example | isPositive(-1) = 0.
Legal ops | ! ~ & ^ \| + << >>
Max ops | 8
Rating | 3
Solution | See comment in the code
```c
int isPositive(int x) {
  /* use ~x to filter out negative numbers
     use ~x + 1 to filter out zero, just like what we did in bang()*/
  return (~x & (~x + 1)) >> 31 & 0x01;
}
```
Function | isLessOrEqual - if x <= y  then return 1, else return 0 
------------ | -------------
Example | isLessOrEqual(4,5) = 1.
Legal ops | ! ~ & ^ \| + << >>
Max ops | 24
Rating | 3
Solution | See comment in the code
```c
int isLessOrEqual(int x, int y) {
  /* when x >= 0 and y < 0 return 0 -- ~(~x & y) & 
     when x < 0 and y >= 0 return 1 -- (x & ~y) |
     when x and y have same sign, we check whether y -x >= 0 */
  return (~(~x & y) & ((x & ~y) | ~(y + ~ x + 1))) >> 31 & 0x01;
}
```
Function | ilog2 - return floor(log base 2 of x), where x > 0
------------ | -------------
Example | ilog2(16) = 4
Legal ops | ! ~ & ^ \| + << >>
Max ops | 90
Rating | 4
Solution | See comment in the code
```c
int ilog2(int x) {
  /* If the leftmost 1 is in nth position, result should be n - 1. 16 is ...10000, so it gets 4
     to count that length, we make bits from 0 ~ n all 1, and use bit count*/
  int mask1, mask2, mask3, mask4, mask5;
  // Make all 0 ~ MSB to 1
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >> 16);
  
  mask1 = 0x55 << 8 | 0x55;
  mask1 = mask1 << 16 | mask1;
  mask2 = 0x33 << 8 | 0x33;
  mask2 = mask2 << 16 | mask2;
  mask3 = 0x0F << 8 | 0x0F;
  mask3 = mask3 << 16 | mask3;
  mask4 = 0xFF << 16 | 0xFF;
  mask5 = 0xFF << 8 | 0xFF;

  x = (x & mask1) + ((x >> 1) & mask1);
  x = (x & mask2) + ((x >> 2) & mask2);
  x = (x & mask3) + ((x >> 4) & mask3);
  x = (x & mask4) + ((x >> 8) & mask4);
  x = (x & mask5) + ((x >> 16) & mask5);
  return x + ~1 + 1;
}
```
Function | float_neg - Return bit-level equivalent of expression -f for floating point argument f.
------------ | -------------
Example | ilog2(16) = 4
Legal ops |  Any integer/unsigned operations incl. \|\|, &&. also if, while
Max ops | 10
Rating | 2
Solution | See comment in the code
```c
unsigned float_neg(unsigned uf) {
  /* If exp is 0xff and M != 0, return uf since it's a NaN
     otherwise reverse the sign and return*/
  unsigned exp = uf >> 23 & 0xff;
  if (!(exp ^ 0xff) && uf << 9 >> 9)
    return uf;
  else
    return 1 << 31 ^ uf;
}
```
Function | float_i2f - Return bit-level equivalent of expression (float) x
------------ | -------------
Example | ilog2(16) = 4
Legal ops |  Any integer/unsigned operations incl. \|\|, &&. also if, while
Max ops | 30
Rating | 4
Solution | See comment in the code
```c
unsigned float_i2f(int x) {
  /* Fisrt we filter out two special cases: 
     1. 0 is only denormalized number for int
     2. INT_MIN is hard to deal with since -x = x 
     Then we set exp and frac by moving MSB to leftmost,
     and due to sign(1 bit), exp(8 bit) and nomalization, it should be rightshfted by 8.
     So rightmost 8bit,we call it tail, would be num to round.
     Then we do round to even, and add exp by one if it is needed.*/
  unsigned sign = 0, exp, frac, tail;
 
  if (x == 0x80000000)
    return 0xcf000000;    
  if (x == 0)
    return 0;
  
  if (x < 0) {
    sign = 1;
    x = -x;
  }
  frac = x;
  
  for (exp = 31 + 127; !(frac & 0x80000000); exp--)
    frac <<= 1;
  tail = frac & 0xFF;
  frac >>= 8;
  
  if ((tail > 0x80) || (tail == 0x80 && frac & 1))
    frac += 1;
  if (frac == 0x01000000)
    exp++;
  frac &= 0x7fffff;
  
  return (sign << 31) | (exp << 23) | frac;
}
```
Function | float_twice - Return bit-level equivalent of expression 2*f for floating point argument f.
------------ | -------------
Example | ilog2(16) = 4
Legal ops |  Any integer/unsigned operations incl. \|\|, &&. also if, while
Max ops | 30
Rating | 4
Solution | See comment in the code
```c
unsigned float_twice(unsigned uf) {
  /*
  If there is an overflow, return uf itself
  If it's normalized, exp += 1
  If it's denomalized, leftshit frac by 1, and add 1 to exp if there is a carry
  */
  unsigned sign = uf & 0x80000000;
  unsigned exp = uf & 0x7f800000;
  unsigned frac = uf & 0x7fffff;

  if (uf == 0 || uf == 0x80000000) return uf;
  if (exp == 0x7f800000) return uf;
  if (exp) exp += 0x800000;
  else{
    frac <<= 1;
    if (frac & 0x800000) {
      exp += 0x800000;
      frac &= 0x7fffff;
    }
  }
  return sign | exp | frac ;
}
```
