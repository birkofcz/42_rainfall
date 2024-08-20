## Basic analysis
There is level5 binary waiting for us and again, it behaves just the same - printing what it reads from stdin:

./level5
42 let me pass
42 let me pass

OK, Ghidra it is:
~~~C
void main(void)

{
  n();
  return;
}

void n(void)

{
  char local_20c [520];
  
  fgets(local_20c,0x200,stdin);
  printf(local_20c);
                    /* WARNING: Subroutine does not return */
  exit(1);
}
~~~
This time **main()** calls the function **n()** which just prints the input and exits. Upon further examination, we will find
another function, **o()**:
~~~C
void o(void)

{
  system("/bin/sh");
                    /* WARNING: Subroutine does not return */
  _exit(1);
}
~~~
This, as we can see, will be our target as it will trigger the shell for us. Memory position: **0x080484a4**

## Format string attack
We will be using format string attack again, but there is one big difference here: as we can see, both **o()** and **n()** does not
return, but using **exit()** instead, preventing the overwrite with format string. Thus, we will have to overwrite the call to exit with the **o()** function.

We will do this by overwriting the **GOT** (Global Offset Table] address - we will link it to the **o()** function.
We can use Ghidra's symbol tree to find **.got.plt** object - which is a part of GOT with pointers to funtions from the **PLT** (Procedure Linkage Table). In simple terms, it is a contact list to functions called from outside of the binary runtime. When we examine the .obj.plt, we find out that the address of **exit** is **0x08049838**.
Now, we can prepare the payload:
~~~shell
address of exit() in hex - %addressofoindecimalx + positioning of exit
~~~
We will firstly find the exit() function:
~~~shell
python -c "print '\x08\x04\x98\x38'[::-1] + '%x '*10" | ./level5
~~~



