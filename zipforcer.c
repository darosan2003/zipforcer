#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define FAIL 13
#define MAX 256
#define DISCARD "/dev/null"

void handle_sigint(int sig) {
  puts("\n[!] Exiting the program");
  exit(1);
}

void bruteforce(char *zipfile, char *passlist) {

  FILE *fp;
  char passwd[MAX];
  int fd[2]; int i = 1;
  int bin, found;
  pid_t pid;

  if(pipe(fd) == -1) {
    fprintf(stderr, "[-] Unable to open pipe\n");
    exit(1);
  }

  if((fp = fopen(passlist, "r")) == NULL) {
    fprintf(stderr, "[-] Unable to open passlist\n");
    exit(2);
  }

  if((bin = open(DISCARD, O_WRONLY)) == -1) {
    fprintf(stderr, "[-] Couldn't open /dev/null\n");
    exit(3);
  }

  while(1) {

    if((pid = fork()) == -1) {
      fprintf(stderr, "[-] Failed to fork process\n");
      fclose(fp);
      exit(4);
    }

    if(pid == 0) {

      close(fd[1]);
      char *args[] = {"unzip", "-o", "-P", passwd, zipfile, NULL};

      dup2(bin, STDOUT_FILENO);
      dup2(bin, STDERR_FILENO);
      close(bin);

      read(fd[0], passwd, strlen(passwd));
      close(fd[0]);

      execvp("unzip", args);
      exit(FAIL);

    }else{

      struct sigaction sa;
      sa.sa_handler = &handle_sigint;
      sigaction(SIGINT, &sa, NULL);

      close(fd[0]);
      int wstat;

      if(fgets(passwd, sizeof(passwd), fp) == NULL) {
        kill(pid, SIGKILL);
        break;
      }
     
      passwd[strlen(passwd) - 1] = '\0';
      printf("[%d] Trying [%s]...\n", i, passwd);
      write(fd[1], passwd, strlen(passwd));

      wait(&wstat);

      if(WIFEXITED(wstat)) {
        int status = WEXITSTATUS(wstat);
        if(status == 0) {
          puts("\n[+] Password Found");
          found = 1;
          break;
        }
      }else{
        fprintf(stderr, "[-] Command terminated abruptly\n");
        fclose(fp);
        exit(5);
      }

      i++;
      close(fd[1]);

    }

  }

  if(!found)
    puts("\n[-] Password not found");

  fclose(fp);

}

int main(int argc, char **argv) {

  if(argc != 3) {
    fprintf(stderr, "[-] Incorrect number of arguments specified. Expected 3\n");
    fprintf(stderr, "[?] Usage: %s <zipfile> <passlist>\n", argv[0]);
    return 1;
  }

  bruteforce(argv[1], argv[2]);

  return EXIT_SUCCESS;

}
