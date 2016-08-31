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
  
The last thing is to translate these two descriptions into HCL format and add it into [seq-full.hcl](https://github.com/CtheSky/CSAPP-lab/blob/master/architecture_lab/sim/seq/seq-full.hcl)  (too long to put here)  
