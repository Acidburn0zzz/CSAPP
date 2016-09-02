# Notice
If you have a problem with how to run simulator in GUI mode, see my answer to this question:
http://stackoverflow.com/questions/38885053/how-to-run-y86-simulator-gui-csapp/39252055#39252055

# Part A
Solution to this Part is quite straight as long as we follow the function prototype in `example.c`:  
#### sum.ys: Iteratively sum linked list elements
```asm
# Execution begins at address 0
	.pos 0
init:	irmovl Stack, %esp # Set up stack pointer
	irmovl Stack, %ebp     # Set up base pointer
	call Main              # Execute main program
	halt                   # Terminate program

# Sample linked list
.align 4
ele1:
	.long 0x00a
	.long ele2
ele2:
	.long 0x0b0
	.long ele3
ele3:
	.long 0xc00
	.long 0

Main:	pushl %ebp
	rrmovl %esp, %ebp
	irmovl ele1, %eax
	pushl %eax
	call sum_list
	rrmovl %ebp, %esp
	popl %ebp
	ret

	# int sum_list(list_ptr ls)
sum_list:
	pushl %ebp
	rrmovl %esp, %ebp
	rrmovl %ebp, %esp
	mrmovl 8(%ebp), %ecx     # ecx = ls
	xorl %eax, %eax          # eax <- val = 0
	andl %ecx, %ecx	         # Set condition codes
	je End
Loop:	mrmovl (%ecx), %edx  # get ls->val
	addl %edx, %eax          # val += ls->val
	mrmovl 4(%ecx), %ecx     # ls = ls->next
	andl %ecx, %ecx
	jne Loop
End:	rrmovl %ebp, %esp
	popl %ebp
	ret

# The stack starts here and grows to lower addresses
	.pos 0x100
Stack:
```
#### rsum.ys: Recursively sum linked list elements
```asm
# Execution begins at address 0
	.pos 0
init:	irmovl Stack, %esp # Set up stack pointer
	irmovl Stack, %ebp     # Set up base pointer
	call Main              # Execute main program
	halt                   # Terminate program

# Sample linked list
.align 4
ele1:
	.long 0x00a
	.long ele2
ele2:
	.long 0x0b0
	.long ele3
ele3:
	.long 0xc00
	.long 0

Main:	pushl %ebp
	rrmovl %esp, %ebp
	irmovl ele1, %eax
	pushl %eax
	call rsum_list
	rrmovl %ebp, %esp
	popl %ebp
	ret

	# int rsum_list(list_ptr ls)
rsum_list:
	pushl %ebp
	rrmovl %esp, %ebp
	rrmovl %ebp, %esp
	mrmovl 8(%ebp), %ecx   # ecx = ls
	andl %ecx, %ecx
	je Basic
	mrmovl (%ecx), %edx    # edx = ls->val
	pushl %edx
	mrmovl 4(%ecx), %ecx   # get ls->next
	pushl %ecx	           # push ls->next
	call rsum_list
	popl %ecx
	popl %edx
	addl %edx, %eax        # return val + rest
	jmp End
Basic:	xorl %eax, %eax    # return 0
End:	rrmovl %ebp, %esp
	popl %ebp
	ret

# The stack starts here and grows to lower addresses
	.pos 0x100
Stack:
```
#### copy.ys: Copy a source block to a destination block
```asm
# Execution begins at address 0
	.pos 0
init:	irmovl Stack, %esp # Set up stack pointer
	irmovl Stack, %ebp     # Set up base pointer
	call Main              # Execute main program
	halt                   # Terminate program

# Source block
.align 4
src:
	.long 0x00a
	.long 0x0b0
	.long 0xc00
# Destination block
dest:
	.long 0x111
	.long 0x222
	.long 0x333

Main:	pushl %ebp
	rrmovl %esp, %ebp
	irmovl $3, %eax
	pushl %eax
	irmovl dest, %eax
	pushl %eax
	irmovl src, %eax
	pushl %eax
	call copy_block
	rrmovl %ebp, %esp
	popl %ebp
	ret

	# int copy_block(int *src, int *dest, int len)
copy_block:
	pushl %ebp
	rrmovl %esp, %ebp
	rrmovl %ebp, %esp
	mrmovl 8(%ebp), %ecx     # ecx = src
	mrmovl 12(%ebp), %edx    # edx = dest
	mrmovl 16(%ebp), %ebx    # ebx = len
	xorl %eax, %eax          # eax <- result = 0
	andl %ebx, %ebx
	jle End
Loop:	irmovl $4, %edi      # edi = 4
	mrmovl (%ecx), %esi      # val = *src
	addl %edi, %ecx          # src++
	rmmovl %esi, (%edx)      # *dest = val
	addl %edi, %edx          # edx++
	xorl %esi, %eax          # result ^= val
	irmovl $-1, %edi
	addl %edi, %ebx          # len--
	jg Loop
End:	rrmovl %ebp, %esp
	popl %ebp
	ret

# The stack starts here and grows to lower addresses
	.pos 0x100
Stack:
```

# Part B
In this part we are required to add two new instructions into SEQ simulator, iaddl and leave, here are descriptions for them:  

Stage     | iaddl V, rB
---       | ---
Fetch     | icode : ifun <- M1[PC] </br>  rA : rB <- M1[PC + 1] </br> valC <- M4[PC + 2]</br> valP <- PC + 6
Decode    | valB <- R[rB]
Execute   | valE <- valC + valB
Memory    |   
WriteBack | R[rB] <- valE
PC Update | PC <- valP  

Stage     | leave
---       | ---
Fetch     | icode : ifun <- M1[PC] </br>  valP <- PC + 1
Decode    | valA <- R[esp] </br> valB <- R[ebp]
Execute   | valE <- 4 + valB
Memory    |   
WriteBack | R[ebp] <- valM </br> R[esp] <- valE
PC Update | PC <- valP 
  
The last thing is to translate these two descriptions into HCL format and add it into [seq-full.hcl](https://github.com/CtheSky/CSAPP-lab/blob/master/architecture_lab/sim/seq/seq-full.hcl)  (see commit history)

# Part C
Our task in Part C is to modify ncopy.ys and pipe-full.hcl with the goal of making ncopy.ys run as fast as possible.
Modification must pass `correctness.pl` and `benchmark.pl` will give a score based on CPE, CPE less than 10 gets a full score.  
My solution involes 2 steps:  
- Implement `iaddl` in pipe-simulator
- Use loop unrolling 

#### Implement `iaddl` in pipe-simulator
It's similar to Part B, you can compare changes in commit history in 
[pipe-full.hcl](https://github.com/CtheSky/CSAPP-lab/blob/master/architecture_lab/sim/pipe/pipe-full.hcl).
Then we can change [ncopy.ys](https://github.com/CtheSky/CSAPP-lab/blob/master/architecture_lab/sim/pipe/ncopy.ys) to combine
some instructions, for example:
```asm
//count++
irmovl $1, %edi	
addl %edi, %eax	

//replace by
iaddl $1, %eax	
```

#### Use loop unrolling 
This technique is mentioned in 5.8 section, instead of one element, now we deal with multiple elements every iteration.
First benefit is that there will be less instructions used on controllin iteration. Seconde benefit is that it will be more 
flexible to rearrange the instruction sequence.

Notice these two lines in original version:
```asm
mrmovl (%ebx), %esi	# read val from src...
rmmovl %esi, (%ecx)	# ...and store it to dst
```
It will cause a load/use hazard and waste an intruction cycle, a good way is to insert another instruction between them,
dealing with multiple elements, we can read and prepare next value with that instruction:
```asm
#/* $begin ncopy-ys */
##################################################################
# ncopy.ys - Copy a src block of len ints to dst.
# Return the number of positive ints (>0) contained in src.
#
# Include your name and ID here.
#
# Describe how and why you modified the baseline code.
#
##################################################################
# Do not modify this portion
# Function prologue.
ncopy:	pushl %ebp		# Save old frame pointer
	rrmovl %esp,%ebp	# Set up new frame pointer
	pushl %esi		# Save callee-save regs
	pushl %ebx
	pushl %edi
	mrmovl 8(%ebp),%ebx	# src
	mrmovl 16(%ebp),%edx	# len
	mrmovl 12(%ebp),%ecx	# dst

##################################################################
# You can modify this portion
	# Loop header
	xorl %eax,%eax		# count = 0;
	iaddl $-5, %edx		# len -= 5
	jl Remain		# if len < 0, goto Remain :

Loop:	mrmovl (%ebx), %esi
	mrmovl 4(%ebx), %edi	# read next val, avoid load/use hazard 
	rmmovl %esi, (%ecx)
	andl %esi, %esi
	jle LNpos
	iaddl $1, %eax
LNpos:	mrmovl 8(%ebx), %esi	# read next val, avoid load/use hazard 
	rmmovl %edi, 4(%ecx)
	andl %edi, %edi
	jle LNpos2
	iaddl $1, %eax
LNpos2:	mrmovl 12(%ebx), %edi	# read next val, avoid load/use hazard 
	rmmovl %esi, 8(%ecx)
	andl %esi, %esi
	jle LNpos3
	iaddl $1, %eax
LNpos3:	mrmovl 16(%ebx), %esi	# read next val, avoid load/use hazard 
	rmmovl %edi, 12(%ecx)
	andl %edi, %edi
	jle LNpos4
	iaddl $1, %eax
LNpos4:	rmmovl %esi, 16(%ecx)	# read next val, avoid load/use hazard 
	andl %esi, %esi
	jle Update
	iaddl $1, %eax
Update:	iaddl $20, %ebx
	iaddl $20, %ecx
	iaddl $-5, %edx
	jg Loop
	
	
Remain:	iaddl $5, %edx		# num of elements left is less than 5
	je Done
Rloop:	mrmovl (%ebx), %esi	# read val from src...
	rmmovl %esi, (%ecx)	# ...and store it to dst
	andl %esi, %esi		# val <= 0?
	jle RNpos		# if so, goto Npos:
	iaddl $1, %eax		# count++
RNpos:	iaddl $-1, %edx		# len--
	iaddl $4, %ebx		# src++
	iaddl $4, %ecx		# dst++
	andl %edx,%edx		# len > 0?
	jg Rloop		# if so, goto Loop:
##################################################################
# Do not modify the following section of code
# Function epilogue.
Done:
	popl %edi               # Restore callee-save registers
	popl %ebx
	popl %esi
	rrmovl %ebp, %esp
	popl %ebp
	ret
##################################################################
# Keep the following label at the end of your function
End:
#/* $end ncopy-ys */
```
