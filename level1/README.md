##Basic examination##

Again, only one file, "level1" to run. Let's try it.
~~~bash
./level1
|
~~~
Exacutable waits for the user input and than exits. 

##Assembly dump analysis##

The assembly dump for a main function in **gdb** tool will show us more:
~~~assembly
   0x08048480 <+0>:	push   %ebp
   0x08048481 <+1>:	mov    %esp,%ebp
   0x08048483 <+3>:	and    $0xfffffff0,%esp       ;stack allignment up to this point.
   0x08048486 <+6>:	sub    $0x50,%esp             ;allocates 80 bytes (0x50 in hex) to the stack
                                                  ;there would be some variable declared with memory allocated.
   0x08048489 <+9>:	lea    0x10(%esp),%eax    
   0x0804848d <+13>:	mov    %eax,(%esp)           ;moves the addressin EAX (buffer) to the top of the stack
   0x08048490 <+16>:	call   0x8048340 <gets@plt>  ;calls the gets function (reads the line of input from user
                                                   ;and stores it into the buffer.
   0x08048495 <+21>:	leave
   0x08048496 <+22>:	ret                          ;pops the return address from the stack and jumps to it
~~~
As we can see, the main allocates 80 bytes on the stack, calculates the pointer to the buffer for gets, calls gets and returns, with no other procedures.
This looks like a dead end and we need to examine further.
