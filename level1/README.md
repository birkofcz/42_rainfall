**Basic examination**

Again, only one file, "level1" to run. Let's try it.
~~~bash
./level1
|
~~~
Exacutable waits for the user input and than exits. 

**Assembly dump analysis**

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
As we can see, the main allocates 80 bytes on the stack, calculates the pointer to the buffer for gets, calls gets and returns, with no other procedures.   0x08048444 <+0>:	push   %ebp
   0x08048445 <+1>:	mov    %esp,%ebp
   0x08048447 <+3>:	sub    $0x18,%esp
   0x0804844a <+6>:	mov    0x80497c0,%eax
   0x0804844f <+11>:	mov    %eax,%edx
   0x08048451 <+13>:	mov    $0x8048570,%eax
   0x08048456 <+18>:	mov    %edx,0xc(%esp)
   0x0804845a <+22>:	movl   $0x13,0x8(%esp)
   0x08048462 <+30>:	movl   $0x1,0x4(%esp)
   0x0804846a <+38>:	mov    %eax,(%esp)
   0x0804846d <+41>:	call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:	movl   $0x8048584,(%esp)
   0x08048479 <+53>:	call   0x8048360 <system@plt>
   0x0804847e <+58>:	leave  
   0x0804847f <+59>:	ret  
This looks like a dead end and we need to examine furthe, so we list all the functions in gdb:
~~~bash
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
0x08048350  fwrite
0x08048350  fwrite@plt
0x08048360  system
0x08048360  system@plt
0x08048370  __gmon_start__
0x08048370  __gmon_start__@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  run                <<--- THIS LOOKS PROMISING!!!
0x08048480  main
0x080484a0  __libc_csu_init
0x08048510  __libc_csu_fini
0x08048512  __i686.get_pc_thunk.bx
0x08048520  __do_global_ctors_aux
~~~
**run()** looks like our way forward, as the assembly dump suggest by calling a system function:
~~~assembly
   0x08048444 <+0>:	push   %ebp
   0x08048445 <+1>:	mov    %esp,%ebp
   0x08048447 <+3>:	sub    $0x18,%esp
   0x0804844a <+6>:	mov    0x80497c0,%eax
   0x0804844f <+11>:	mov    %eax,%edx
   0x08048451 <+13>:	mov    $0x8048570,%eax
   0x08048456 <+18>:	mov    %edx,0xc(%esp)
   0x0804845a <+22>:	movl   $0x13,0x8(%esp)
   0x08048462 <+30>:	movl   $0x1,0x4(%esp)
   0x0804846a <+38>:	mov    %eax,(%esp)
   0x0804846d <+41>:	call   0x8048350 <fwrite@plt>      ;writing a message
   0x08048472 <+46>:	movl   $0x8048584,(%esp)
   0x08048479 <+53>:	call   0x8048360 <system@plt>      ;calling system function, pattern that we identified in the                                                            ;previous level
   0x0804847e <+58>:	leave  
   0x0804847f <+59>:	ret  
~~~
