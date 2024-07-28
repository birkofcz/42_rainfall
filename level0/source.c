#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv)
{
  int password;
  char *execv_args[2];
  __uid_t user_id;
  __gid_t group_id;
  
  password = atoi(argv[1]);
  if (password == 423) 
  {
    execv_args[0] = strdup("/bin/sh");
    execv_args[1] = NULL; 
    user_id = getegid();
    group_id = geteuid();
    setresgid(group_id,group_id, group_id);
    setresuid(user_id, user_id, user_id);
    execv("/bin/sh", execv_args);
  }
  else 
  {
    fwrite("No !\n", 1, 5, stderr);
  }

  return 0;
}