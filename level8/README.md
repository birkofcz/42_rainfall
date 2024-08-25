## Basic analysis
Level 8, here we go!
~~~shell
level8@RainFall:~$ ./level8
(nil), (nil) 
42
(nil), (nil) 
42 42
(nil), (nil) 
~~~
Expecting the right input, are we? Hmm? We try to analyze the binary with **Ghidra**. The decompiler was not really clear this time, but giving up hints to be compared with the assembly dump.

Major findings:
1) There are no other function called from main.
2) Main is running in a loop (** 0x08048727 <+451>:	jmp    0x8048574 <main+16>** at the end of the assembly dump)
3) Main is calling a lot  to GOT - printf, free, strdup, fgets, fwrite, strcpy, malloc, **system** - the last one being our goal, as we know from previous levels
4) there are some interesting strings in the flow: "auth", "service", "reset", "login"
5) There are two global variables: "auth" and "service" - as we see from the decompilation, pointers to them being printed each loop. They have no allocated memory at the start.
6) Main flow appers to be a series of condition checks - assuming from the program test run, comparing the input of fgets - to our strings mentioned above.
7) When "auth " is entered, memory is allocated.
8) When "service" is entered, strdup is being called.
9) When "reset" is entered, memory is freed. 
10) When "login" is entered, global "auth" viariable is tested - specificaly if there is something on its 32nd byte. If yes - system si called. If not, "password:" string is printed and loop is started again.

~~~C

~~~
