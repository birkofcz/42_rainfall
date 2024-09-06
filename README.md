# rainfall
<img src="0_resources/rainfall_header.webp" width="100%"/>

This project is a second in a series of three to elaborate on the basics of cybersecurity, letting us play with various vulnerabilities in binaries, teaching basic principles of reverse engineering.
This is the short overview of the levels examined and vulnerabilities used to advance in them:
| level | Exploit |
| ------ | ------- |
| 0 | Simple reverse engineering |
| 1 | Buffer overflow attack |
| 2 | ret2libc attack |
| 3 | format string attack |
| 4 | format string attack |
| 5 | format string attack + GOT overwrite |
| 6 | Buffer overflow attack |
| 7 | Buffer overflow attack + GOT overwrite |
| 8 | Simple(??) reverse engineering |
| 9 | Buffer overflow attack |

Even though presented vulnerabilities are pretty basic and mostly not-existent in modern software, the lessons of this project, as we in **Ones & Zeros Social Club** team acknowlegde, is the broader view of how programs work. We learned much about:
1) What is a ELF binary and how binaries are executed in the system
2) What is the **stack frame** and how it is constructed
3) What **Global Offset Table** and **Process Linkage Table**
4) Litle training in HEX values reading and how to navigate in memory
5) Work with various debugging and reverse engineering tools, namely **gdb**, **Ghidra** and **Wiremask**.

Let's quickly explore some of those mentioned above:

## Concepts

### Buffer Overflow
A buffer overflow occurs when a program writes more data to a block of memory, or buffer, than it can hold. This excess data can overflow into adjacent memory, potentially overwriting valid data or executable code. For example, if a function expects a user input of 20 characters but receives 50 characters instead, the extra 30 characters might overwrite other critical memory areas. This can cause the program to crash, corrupt data, or even allow an attacker to execute arbitrary code. An attacker might craft the input in such a way that it overwrites the return address of a function, redirecting execution to a payload they control. The classic example is exploiting a stack buffer overflow to inject shellcode and gain unauthorized access to a system.

### ret2libc
ret2libc (return to libc) is an exploit technique that takes advantage of buffer overflow vulnerabilities to bypass certain security measures, such as non-executable stack protections. Instead of injecting malicious code directly, the attacker manipulates the stack to return to an existing function within the libc (standard C library), such as system(), which can be used to execute arbitrary commands. For instance, an attacker might overwrite a return address on the stack with the address of system(), followed by the address of /bin/sh, causing the program to spawn a shell. This technique leverages existing, trusted code in the program's memory space, making it more challenging to detect and prevent compared to traditional code injection attacks.

### Format String Attack
A format string attack occurs when an attacker exploits a vulnerability caused by the improper handling of format strings in functions like printf(). If user-controlled input is passed directly as a format string, an attacker can craft the input to manipulate the program’s memory. For instance, a user could input %x%x%x%x, causing the program to print values from the stack, potentially leaking sensitive information. More dangerously, %n can be used to write arbitrary values to memory addresses, allowing an attacker to overwrite critical data structures, such as function pointers or return addresses. This can ultimately lead to arbitrary code execution, similar to a buffer overflow, but using format string vulnerabilities.
