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

### ELF Binary Basics
The Executable and Linkable Format (ELF) is a standard file format for executables, object code, shared libraries, and core dumps in Unix-like operating systems. An ELF binary contains several sections and segments that define how the program is structured and how it should be loaded into memory. The main sections include the .text section, which contains the executable code, the .data section for initialized global and static variables, and the .bss section for uninitialized variables. ELF also includes headers that describe the file’s organization and how the operating system should load the program.

For example, a basic ELF header might look like this:

~~~plaintext
Copy code
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x400080
  Start of program headers:          64 (bytes into file)
  Start of section headers:          1024 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
~~~

**Program Execution and Build Process**  
When you compile a program, the compiler generates an object file, which is typically in ELF format. The linker then combines these object files with any necessary libraries to create an executable. The executable contains all the information needed to load the program into memory and execute it, including the entry point, where execution starts.

During execution, the operating system’s loader reads the ELF headers to determine how to load the program into memory. It maps the segments into the appropriate memory regions (e.g., .text into executable memory, .data into writable memory), sets up the stack, and jumps to the entry point specified in the ELF header to start executing the program.

The ELF format's flexibility allows it to be used for both static and dynamic linking. In static linking, all code is included directly in the executable, while dynamic linking allows the executable to use shared libraries, reducing the overall size of the binary. This standardization and versatility make ELF a foundational component of program execution in Unix-like systems.

**GOT (Global Offset Table)**  
The Global Offset Table (GOT) is a crucial part of the dynamic linking process in ELF binaries. It is essentially an array of pointers used to resolve addresses of global variables and functions at runtime, particularly when dealing with shared libraries. When a program is compiled, it doesn't know the exact memory addresses of external functions or global variables. Instead, it uses the GOT to store the addresses of these functions and variables. Initially, the GOT entries point to the PLT (Procedure Linkage Table), but as the program runs, these entries are updated with the actual addresses. This allows the program to access functions and variables across different modules efficiently, enabling dynamic linking and position-independent code (PIC).

**PLT (Procedure Linkage Table)**  
The Procedure Linkage Table (PLT) works in conjunction with the GOT to facilitate dynamic function calls in ELF binaries. When a program calls an external function, it first goes through the PLT, which then redirects the call to the correct address. The PLT is essentially a stub that ensures the actual address of the function is resolved and cached in the GOT. The first time a function is called, the PLT jumps to the dynamic linker, which then finds the correct address and updates the GOT. Subsequent calls to the function go directly to the address stored in the GOT, bypassing the PLT. This mechanism provides a balance between flexibility (allowing for dynamic linking) and performance (minimizing the overhead of repeated lookups).

### Buffer Overflow
A buffer overflow occurs when a program writes more data to a block of memory, or buffer, than it can hold. This excess data can overflow into adjacent memory, potentially overwriting valid data or executable code. For example, if a function expects a user input of 20 characters but receives 50 characters instead, the extra 30 characters might overwrite other critical memory areas. This can cause the program to crash, corrupt data, or even allow an attacker to execute arbitrary code. An attacker might craft the input in such a way that it overwrites the return address of a function, redirecting execution to a payload they control. The classic example is exploiting a stack buffer overflow to inject shellcode and gain unauthorized access to a system.

### ret2libc
ret2libc (return to libc) is an exploit technique that takes advantage of buffer overflow vulnerabilities to bypass certain security measures, such as non-executable stack protections. Instead of injecting malicious code directly, the attacker manipulates the stack to return to an existing function within the libc (standard C library), such as system(), which can be used to execute arbitrary commands. For instance, an attacker might overwrite a return address on the stack with the address of system(), followed by the address of /bin/sh, causing the program to spawn a shell. This technique leverages existing, trusted code in the program's memory space, making it more challenging to detect and prevent compared to traditional code injection attacks.

### Format String Attack
A format string attack occurs when an attacker exploits a vulnerability caused by the improper handling of format strings in functions like printf(). If user-controlled input is passed directly as a format string, an attacker can craft the input to manipulate the program’s memory. For instance, a user could input %x%x%x%x, causing the program to print values from the stack, potentially leaking sensitive information. More dangerously, %n can be used to write arbitrary values to memory addresses, allowing an attacker to overwrite critical data structures, such as function pointers or return addresses. This can ultimately lead to arbitrary code execution, similar to a buffer overflow, but using format string vulnerabilities.
