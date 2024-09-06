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

