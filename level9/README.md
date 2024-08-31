## Basic analysis
All right, lets give this a try:
~~~shell
level9@RainFall:~$ ./level9
level9@RainFall:~$ ./level9 42
level9@RainFall:~$ ./level9 42 42
level9@RainFall:~$ ./level9 42 42 42
level9@RainFall:~$ 
~~~
Doing nothing, not really helpful...you deserve to be eaten by **Ghidra**!
~~~C
void main(int param_1,int param_2)
{
  N *this;
  undefined4 *this_00;
  
  if (param_1 < 2) {
                    /* WARNING: Subroutine does not return */
    _exit(1);
  }
  this = (N *)operator.new(0x6c);
  N::N(this,5);
  this_00 = (undefined4 *)operator.new(0x6c);
  N::N((N *)this_00,6);
  N::setAnnotation(this,*(char **)(param_2 + 4));
  (**(code **)*this_00)(this_00,this);
  return;
}
void __thiscall N::N(N *this,undefined4 param_1)

{
  *(undefined ***)this = &PTR_operator+_08048848;
  *(undefined4 *)(this + 0x68) = param_1;
  return;
}

void __thiscall N::setAnnotation(N *this,char *param_1)
{
  size_t __n;
  
  __n = strlen(param_1);
  memcpy(this + 4,param_1,__n);
  return;
}
~~~
As we see, this binary is C++, memory is being allocated when the class in created. Also, the function **setAnnotation()** is called, expecting the argument from main, measuring it and calling **memcpy(*dest, *src, int length)**.

