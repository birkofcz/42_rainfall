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
3) What **Global Offset Table** and **Procedure Linkage Table**
4) Litle training in HEX values reading and how to navigate in memory
5) Work with various debugging and reverse engineering tools, namely **gdb**, **Ghidra** and **Wiremask**.

Let's quickly explore some of those mentioned above:

## ELF Binary 
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

## Stack Frame  
A stack frame is a structure that the program uses to manage function calls and local variables in a stack-based memory architecture. Every time a function is called, a new stack frame is created and pushed onto the stack. This frame contains several key components: the function's return address (so the program knows where to return after the function finishes), the saved frame pointer (which points to the previous stack frame), and space for the function’s local variables and any arguments passed to it. The stack frame also handles the dynamic allocation of memory for variables that are created during the function's execution.

For example, when a function foo() calls another function bar(), a new stack frame for bar() is pushed onto the stack. This frame will include the return address for foo(), so when bar() finishes, control can be returned to the correct location in foo(). After bar() completes, its stack frame is popped off the stack, and the stack pointer is adjusted to point back to foo()'s frame. This organized structure allows for nested function calls and recursion, ensuring that each function has its own isolated environment in memory, while also keeping track of the sequence of function calls.

**EBP (Base Pointer) and ESP (Stack Pointer)**  
The EBP (Base Pointer) and ESP (Stack Pointer) are key registers in the management of stack frames during function calls in x86 architecture. The ESP points to the top of the current stack, constantly changing as the stack grows and shrinks with function calls and returns. The EBP, on the other hand, is used as a stable reference point within a stack frame. When a function is called, the current value of EBP is pushed onto the stack and then EBP is updated to point to the current top of the stack, effectively marking the beginning of the new stack frame. This allows the function to easily access its parameters and local variables using offsets from EBP.

The typical first lines of assembly in a function that establish this setup are as follows:

~~~assembly
push ebp       ; Save the old base pointer
mov ebp, esp   ; Set EBP to the current top of the stack (ESP)
sub esp, N     ; Allocate space for local variables by decreasing ESP
~~~
In this code, push ebp saves the previous base pointer, and mov ebp, esp sets the base pointer to the current stack pointer, effectively creating a new stack frame. The sub esp, N line adjusts the stack pointer to allocate space for local variables within the new stack frame.

After the function completes, the stack frame is dismantled with instructions like:

~~~assembly
mov esp, ebp   ; Restore the stack pointer to the base pointer
pop ebp        ; Restore the old base pointer
ret            ; Return to the calling function
~~~

These instructions ensure that the stack is properly cleaned up and that control is returned to the correct place in the calling function, using the saved return address. This structure maintains order in function calls, supporting recursion and nested calls efficiently.

**The EBP (Base Pointer)** acts as a constant reference point within the current stack frame. Once it's set at the beginning of a function, it doesn't change until the function is finished. This allows the program to use fixed offsets from EBP to access function parameters and local variables, making it easy to navigate within the stack frame.

**The ESP (Stack Pointer)**, on the other hand, is more dynamic. It points to the top of the stack and changes as the stack grows or shrinks during the execution of the function. For example, when local variables are allocated or when function calls push return addresses onto the stack, ESP adjusts to reflect the new top of the stack. This makes ESP a flexible tool for managing the current position in the stack, while EBP provides a stable anchor for accessing data within the current stack frame.

So, in summary:

**EBP** is a stable pointer used to reference the fixed structure of the current stack frame (like parameters and local variables).
**ESP** is a dynamic pointer that tracks the top of the stack as it changes during function execution.

**Return Address**
The return address is a critical component of a stack frame. When a function is called, the address of the instruction immediately following the call (i.e., where the program should return to after the function completes) is pushed onto the stack. This return address is stored just above the base pointer (EBP) in the stack frame. When the function finishes, the return address is popped off the stack, and the program jumps back to this address to continue execution. This mechanism ensures that after a function completes, control is returned to the correct location in the calling function.

**Function Parameters**
Function parameters are passed to a function via the stack (or registers, depending on the calling convention), and they are usually stored in the caller’s stack frame. When the function is called, these parameters are accessible via positive offsets from the base pointer (EBP), since EBP points to the caller’s stack frame. The function can then use these offsets to retrieve the values of the parameters passed to it, which are necessary for its operations.

## Reverse Engineering Overview
Reverse engineering is the process of analyzing a system, software, or hardware to understand its design, functionality, and operation, often with the goal of replicating or improving it. In the context of software, reverse engineering typically involves deconstructing a compiled program back into its original source code or an equivalent form, allowing an engineer to study how the software works. This can include examining binary files, analyzing the program’s structure, understanding its algorithms, and identifying potential vulnerabilities or malicious code. Basic concepts in reverse engineering include **disassembly** (converting binary code back into assembly language), **decompilation** (attempting to convert binary code into a higher-level language), **static analysis** (analyzing the code without executing it), and **dynamic analysis** (observing the behavior of the software during execution). Reverse engineering is commonly used in security research, malware analysis, software debugging, and compatibility studies. It requires a strong understanding of computer architecture, programming languages, and the specific system being analyzed.

Here's a table of commonly used registers in reverse engineering, particularly in the context of x86 architecture:
| Register | Name              | Description                                                                 |
|----------|-------------------|-----------------------------------------------------------------------------|
| EAX      | Accumulator        | General-purpose register often used for arithmetic operations. Also serves as return value register            |
| EBX      | Base               | General-purpose register that can be used for indexing and pointer operations.|
| ECX      | Counter            | Often used as a loop counter or for string operations.                        |
| EDX      | Data               | General-purpose register often used in I/O operations and arithmetic operations.|
| ESI      | Source Index       | Used for pointer arithmetic in data segment operations, often in string processing.|
| EDI      | Destination Index  | Used for pointer arithmetic in data segment operations, often in string processing.|
| EBP      | Base Pointer       | Points to the base of the current stack frame, used for stack frame management.|
| ESP      | Stack Pointer      | Points to the top of the current stack, used for managing the stack.         |
| EIP      | Instruction Pointer| Points to the next instruction to be executed by the CPU.                   |
| EFLAGS   | Flags Register     | Holds flags that control CPU operations and indicate the status of operations (e.g., zero, carry, overflow).|

## Vulnerabilities

### Buffer Overflow
A buffer overflow occurs when a program writes more data to a block of memory, or buffer, than it can hold. This excess data can overflow into adjacent memory, potentially overwriting valid data or executable code. For example, if a function expects a user input of 20 characters but receives 50 characters instead, the extra 30 characters might overwrite other critical memory areas. This can cause the program to crash, corrupt data, or even allow an attacker to execute arbitrary code. An attacker might craft the input in such a way that it overwrites the return address of a function, redirecting execution to a payload they control. The classic example is exploiting a stack buffer overflow to inject shellcode and gain unauthorized access to a system.

![Stack overflow](https://www.cameronwickes.co.uk/_nuxt/uploads/9135072-1027.jpg)
Source: [https://www.cameronwickes.co.uk/stack-frames-pointers/](https://www.cameronwickes.co.uk/stack-frames-pointers/)

### ret2libc
ret2libc (return to libc) is an exploit technique that takes advantage of buffer overflow vulnerabilities to bypass certain security measures, such as non-executable stack protections. Instead of injecting malicious code directly, the attacker manipulates the stack to return to an existing function within the libc (standard C library), such as system(), which can be used to execute arbitrary commands. For instance, an attacker might overwrite a return address on the stack with the address of system(), followed by the address of /bin/sh, causing the program to spawn a shell. This technique leverages existing, trusted code in the program's memory space, making it more challenging to detect and prevent compared to traditional code injection attacks.

### Format String Attack
A format string attack occurs when an attacker exploits a vulnerability caused by the improper handling of format strings in functions like printf(). If user-controlled input is passed directly as a format string, an attacker can craft the input to manipulate the program’s memory. For instance, a user could input %x%x%x%x, causing the program to print values from the stack, potentially leaking sensitive information. More dangerously, %n can be used to write arbitrary values to memory addresses, allowing an attacker to overwrite critical data structures, such as function pointers or return addresses. This can ultimately lead to arbitrary code execution, similar to a buffer overflow, but using format string vulnerabilities.
