/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  /* use De Morgan's laws: x Λ y = ¬(¬(xy)) = ¬(¬x V ¬y) */
  return ~(~x | ~y);
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  /* right shift x by 8 * n to move the required byte to rightmost
     then take that byte by &0xff */
  return (x >> (n << 3)) & 0xff;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  /* construct a mask with starting n positions filled with 0 and 1 for the rest 
     and then use & to set starting n positions to 0 
     split 1 << (32 - n) to 1 << (31 - n) << 1 for n = 0 */
  int mask = (1 << (32 + ~n) << 1) + ~0;
  return x >> n & mask;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  /* It's easier to read as:
     x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
     x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
     x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
     x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
     x = (x & 0x0000FFFF) + ((x >> 16)& 0x0000FFFF);
     Each step adds neighbouring bits together in groups of 1, then 2, then 4 etc.
     The method is based in divide and conquer. */
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
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  /* exploit the fact that ~0 + 1 = 0 which makes it the only one starting with 0 
     but get 1 as first bit after ~(~x + 1),
     to filter out those x starting with 1, just &(~x) */
  return (((~x)&(~(~x + 1)))>>31) & 0x01;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /* left shft 1 by 31 and get an int starting with an only 1-bit */
  return 1 << 31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
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
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  /* For negative x, right shift will round down while integer division requires round up
     to solve this problem, we could use the fact: 
     for any x and y > 0, ceil(x/y) = floor((x + y - 1) / y) */
  int sign = x >> 31 & 0x01;
  return (x + (sign << n) + ~sign + 1) >> n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  /* Consider x + ~x + 1, if rightmost 1 in x is position n, ~x + 1 will have 1 in n
     and every bit reverse to x in left positions which means we will get a carry for all
     left positions. So the result will be 10000...(w of 0), the first bit will be omit 
     and we get zero. */
  return ~x + 1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  /* use ~x to filter out negative numbers
     use ~x + 1 to filter out zero, just like what we did in bang()*/
  return (~x & (~x + 1)) >> 31 & 0x01;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /* when x >= 0 and y < 0 return 0 -- ~(~x & y) & 
     when x < 0 and y >= 0 return 1 -- (x & ~y) |
     when x and y have same sign, we check whether y -x >= 0 */
  return (~(~x & y) & ((x & ~y) | ~(y + ~ x + 1))) >> 31 & 0x01;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
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
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  /* If exp is 0xff and M != 0, return uf since it's a NaN
     otherwise reverse the sign and return*/
  unsigned exp = uf >> 23 & 0xff;
  if (!(exp ^ 0xff) && uf << 9 >> 9)
    return uf;
  else
    return 1 << 31 ^ uf;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
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
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
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
