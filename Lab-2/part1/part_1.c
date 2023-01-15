#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>  // for fork()
#include<sys/wait.h>  // for wait()

int main(){
  int p_id = fork();
  // int p_id;
  int sleep_time;
  char str[] = "Hello World";
  int length = strlen(str);
  for (int i = 0; i < length; i++){
    if(p_id < 0){
      printf("** child not created **\n");
    }
    else if( p_id == 0) {
      // printf("*** Child Process ***\n");
      sleep_time = 1 + (rand() % 4);
      sleep(sleep_time);
      // printf("%d\n", sleep_time);
      // printf("pid of process : %d & letter : %c\n", getpid(), str[i]);
      printf("%c %d\n", str[i], getpid());
      p_id = fork();
    }
    else{
      wait(NULL);
      // printf("*** Parent Process ***\n");
      // printf("pid of process : %d\n", getpid());
    }
  }
  
  return 0;
}