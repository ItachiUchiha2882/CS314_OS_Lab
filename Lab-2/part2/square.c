#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Please provide a number as a command-line argument\n");
    return 1;
  }
  int number = atoi(argv[argc - 1]);

  // printf(" ** In square.c ** \n");
  // printf("PID of square.c : %d\n", getpid());

  number = number * number;
  char number_str[100];
  sprintf(number_str, "%d", number);

  char *args[argc-1];
  for(int i=0; i<argc-1; i++){
    args[i] = argv[i+1];
  } // 0 to argc-2
  args[argc-2] = number_str;
  args[argc-1] = NULL;       

  printf("Square: Current process id: %d, Current result: %s\n", getpid(), number_str);
  execvp(args[0], args);

  return 0;
}
