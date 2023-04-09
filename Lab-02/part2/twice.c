#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Please provide a number as a command-line argument\n");
    return 1;
  }
  int number = atoi(argv[argc - 1]);

  // printf(" ** In twice.c ** \n");
  // printf("PID of twice.c : %d\n", getpid());

  number = number * 2;
  char number_str[100];
  sprintf(number_str, "%d", number);

  // for (int i = 0; i < 4; i++){
  // char *args[] = {"twice", number_str, NULL};
  // execvp("twice", args);
  // }
  // ./half .... ./twice 4

  char *args[argc-1];
  for(int i=0; i<argc-1; i++){
    args[i] = argv[i+1];
  } // 0 to argc-2
  args[argc-2] = number_str;
  args[argc-1] = NULL;            // <-------
  // for(int i=0; i<argc-1; i++){
  //   printf("%s\n",args[i]);
  // } 
  // printf("result is %s\n", number_str);
  printf("Twice: Current process id: %d, Current result: %s\n", getpid(), number_str);

  execvp(args[0], args);

  // pid_t pid = getpid();
  // char number[100];
  // sprintf(number, "%s", argv[1]);
  // char *args[] = {argv[2], number, NULL};
  // execvp(argv[2], args);

  return 0;
}
