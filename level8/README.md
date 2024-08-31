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
8) When "service" is entered, strdup is being called (means also memory has been alocatted)
9) When "reset" is entered, memory is freed. 
10) When "login" is entered, global "auth" viariable is tested - specificaly if there is something on its 32nd byte: If yes - system si called. If not, "password:" string is printed and loop is started again. The line in assembly dump is: **080486e7 8b 40 20  MOV EAX,dword ptr [EAX + 0x20] **


## Experimentation
As we found out above, there is a specific combination of input that needs to be entered, se when the **login** is entered, the memory space allocated have something at its 32nd byte. There is a quick hint into this - as we can see from assembly (MOV EAX,dword ptr [EAX + 0x20]), 32nd byte is, in hex memory address language, 20 positions after the initilal allocation. From here, we don't need to complicate things further and as long as the program is printing the momory addresses, we can just experiment!

ASs we see, we need the initila allocation - using **auth** to do that (auth + space asi in condition). We can also see, that sending something to auth will not help us - it just realocates into further position in memory. We need to utilize **service** command using strdup. Entering once will get us 16 bytes (10 positions) further into the memory space, that is so convenient, as it as a half-way - so how about we enter it twice? There we go - allocated where we need it - now login for **system()** call.
~~~shell
level8@RainFall:~$ ./level8
(nil), (nil) 
auth 
0x804a008, (nil) 
auth 424242
0x804a018, (nil) 
service
0x804a018, 0x804a028 
service
0x804a018, 0x804a038 
login
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
~~~
Level 9, here we go!
