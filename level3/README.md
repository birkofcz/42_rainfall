## Basic analysis

As we see, **level3** binary prints out what is being fed to it via stdin:
~~~shell
.level3 
42 let me pass
42 let me pass
~~~
Let's make a secure copy of the executable and examine it with **Ghidra**. We will discover that **main()** is just calling **v()** and returns.  
**v()** is using **fgets()** to get the input from user, saves it into the premade **char xxx[520]** string and prints it using **printf()**. That is what we see as program execution.
~~~C
void main(void)

{
  v();
  return;
}

void v(void)

{
  char local_20c [520];
  
  fgets(local_20c,0x200,stdin);
  printf(local_20c);
  if (m == 0x40) {
    fwrite("Wait what?!\n",1,0xc,stdout);
    system("/bin/sh");
  }
  return;
}
~~~
That's not all of course - there is condition comparing variable **m** to number **64** (0x40 in hexa). If true, it will let us pass to next level.
Thus, we need to somehow modify the variable m. It is not defined in the scope of v(), nor passed through from main, so it must be a global variable.
We use **gdb**'s "info variables" to get its memaddress: **0x0804988c**

## Format string attack
**print() can be utilized for **format string attack** as described [HERE](). 
