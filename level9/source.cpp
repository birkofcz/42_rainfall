#include <stdlib.h>
#include <string.h>

class N 
{
  public:
    char argument[104];
    int  value;

  N(int value) 
  {
    this->value = value;
  }

  int operator+(N number) 
  {
    return this->value + number.value;
  }

  int operator-(N number) 
  {
    return this->value - number.value;
  }

  void setAnnotation(char *str) 
  {
    int lenght;

    leght = strlen(str);
    memcpy(this->argument, str, lenght);
  }
};

int main(int argc, char **argv) 
{
  if (argc < 2) 
  {
    exit(1);
  }

  N *first = new N(5);
  N *second = new N(6);

  *first->setAnnotation(argv[1])
  
  return (*first + *second);     //here is your 11 on return when it is not segfaulting.
}
