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
  void *unaff_retaddr;
  char local_50 [76];
  
  fflush(stdout);
  gets(local_50);
  if (((uint)unaff_retaddr & 0xb0000000) == 0xb0000000) {
    printf("(%p)\n",unaff_retaddr);
                    /* WARNING: Subroutine does not return */
    _exit(1);
  }
  puts(local_50);
  strdup(local_50);
  return;
}
~~~
We can already see that the program is **allocating 80 bytes, 76 for the string and 4 bytes for uint variable (4 bytes on 32 bit systems)**. From what we already know about **stack frame** for program execution, this will be our potential offset for force execution of what we might need on the place of the return memory address.


