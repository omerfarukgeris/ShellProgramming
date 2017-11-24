#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <sys/time.h>
#include <ctype.h>

int lineSize;
int initialPid;
int num = 0;
int batch = 0;

struct childinfo{
  int back;
  int pid;
  char* name;
};

struct childinfo* children;

void argError() {
  fprintf(stderr, "Usage: mysh [batchFile]\n");
  fflush(stdout);
}

void fileError(const char* str) {
  fprintf(stderr, "Error: Cannot open file %s\n", str);
  fflush(stdout);
}

char** parseLine(char* input) {
  int length = 10;
  char** out = malloc(length * sizeof(char*));
  if (out == NULL) {
    perror("malloc failed");
    exit(0);
  }
  char* token = strtok(input, " \t");

  int pos = 0;
  while (token != NULL) {
    if (pos >= length) {
      length += 10;
      out = realloc(out, length * sizeof(char*));
      if (out == NULL) {
        perror("realloc failed");
        exit(0);
      }
    }
    out[pos] = strdup(token);
    pos++;
    token = strtok(NULL, " \t");
  }
  lineSize = pos;
  if (pos >= length) {
    length++;
    out = realloc(out, length * sizeof(char*));
    if (out == NULL) {
      perror("realloc failed");
      exit(0);
    }
  }
  out[pos] = NULL;
  out = realloc(out, (pos+1) * sizeof(char*));
  if (out == NULL) {
    perror("realloc failed");
    exit(0);
  }
  return out;
}

void handle_j() {
  int i;
  for (i = 0; i < num; i++) {
    int pid = children[i].pid;
    if ((waitpid(pid, NULL, WNOHANG) == 0) && (children[i].back == 1)) {
      char str1[5];
      sprintf(str1, "%d", i + 1);
      str1[strlen(str1)] = '\0';
      printf("%s : %s\n", str1, children[i].name);
      fflush(stdout);
    }
  }
}

void handle_myw(int pidcount) {
  if (pidcount > num) {
    fprintf(stderr, "Invalid jid %d\n", pidcount);
    fflush(stdout);
    return;
  }
  int realpid = children[pidcount - 1].pid;
  if (waitpid(realpid, NULL, WNOHANG) != 0) {
    printf("0 : Job %d terminated\n", pidcount);
    fflush(stdout);
    return;
  }
  struct timeval tv;
  gettimeofday(&tv, NULL);
  unsigned long time_micros1 = 1000000 * tv.tv_sec + tv.tv_usec;
  waitpid(realpid, NULL, 0);
  gettimeofday(&tv, NULL);
  unsigned long time_micros2 = 1000000 * tv.tv_sec + tv.tv_usec;
  printf("%ld : Job %d terminated\n", time_micros2 - time_micros1, pidcount);
  fflush(stdout);
}

int digitString(char* String) {
  int length = strlen(String);
  int i;
  for (i = 0; i < length; i++) {
    if (isdigit(String[i]) == 0)
      return 0;
  }
  return 1;
}

void freeAll(char** trimmedLine) {
  int i;
  for (i = 0; i < lineSize; i++) {
    free(trimmedLine[i]);
  }
  free(trimmedLine);
}

int main(int argc, char *argv[]) {
  FILE* inFile = NULL;
  int back = 0;
  initialPid = getpid();
  num = 0;
  int counter = 0;
  children = (struct childinfo*)malloc(sizeof(struct childinfo));
  if (children == NULL) {
    perror("malloc failed");
    exit(0);
  }
  if ((argc != 1 && argc != 2)) {
    argError();
    exit(1);
  } else if (argc == 2) {
    close(STDIN_FILENO);
    inFile = fopen(argv[1], "r");
    if (inFile == NULL) {
      fileError(argv[1]);
      exit(1);
    }
    batch = 1;
    dup2(fileno(inFile), STDIN_FILENO);
  }
  while (1) {
    back = 0;
    counter++;
    char** trimmedLine;
    if (batch == 0) {
      if (write(STDOUT_FILENO, "mysh> ", strlen("mysh> ")) == -1)
        perror("can not write mysh");
    }
    char input[520];
    if (fgets(input, 520, stdin) == NULL) {
      if (batch == 1) {
        int i;
        for (i = 0; i < num; i++) {
          free(children[i].name);
        }
        free(children);
        fclose(inFile);
        exit(0);
      }
      write(STDERR_FILENO, "cannot read", strlen("cannot read"));
      while (getchar() != '\n') { }
    }
    if (strlen(input) > 513) {
      printf("%s", input);
      fflush(stdout);
      continue;
    }
    input[strlen(input) - 1] = '\0';
    char* input1 = strdup(input);
    trimmedLine = parseLine(input);
    if (lineSize == 0) {
      if (batch == 1)
        write(STDOUT_FILENO, "\n", strlen("\n"));
      freeAll(trimmedLine);
      continue;
    }
    if (batch == 1) {
        printf("%s\n", input1);
        fflush(stdout);
        free(input1);
    }
    if ((trimmedLine[0][0] == 'e') && (trimmedLine[0][1] == 'x')
        && (trimmedLine[0][2] == 'i') && (trimmedLine[0][3] == 't')) {
        if (strcmp(trimmedLine[0], "exit") == 0) {
          if (lineSize == 1) {
            freeAll(trimmedLine);
            int i;
            for (i = 0; i < num; i++) {
              free(children[i].name);
            }
            free(children);
            if (batch == 1)
              fclose(inFile);
            exit(0);
          }
          if (lineSize == 2) {
            if (strcmp(trimmedLine[1], "&") == 0) {
              freeAll(trimmedLine);
              int i;
              for (i = 0; i < num; i++) {
                free(children[i].name);
              }
              free(children);
              if (batch == 1)
                fclose(inFile);
              exit(0);
            }
          }
        } else if (lineSize == 1) {
          if (strlen(trimmedLine[0]) == 4) {
            freeAll(trimmedLine);
            int i;
            for (i = 0; i < num; i++) {
              free(children[i].name);
            }
            free(children);
            if (batch == 1)
              fclose(inFile);
            exit(0);
          }
          if (strlen(trimmedLine[0]) == 5) {
            if (trimmedLine[0][4] == '&') {
              freeAll(trimmedLine);
              int i;
              for (i = 0; i < num; i++) {
                free(children[i].name);
              }
              free(children);
              if (batch == 1)
                fclose(inFile);
              exit(0);
            }
          }
        }
    }
    if (strcmp(trimmedLine[lineSize - 1], "&") == 0) {
      back = 1;
      trimmedLine[lineSize - 1] = NULL;
      lineSize--;
    } else {
      back = 0;
      char* last = trimmedLine[lineSize - 1];
      int length = strlen(last);
      if (length > 1) {
        if (last[length - 1] == '&') {
          last[length - 1] = '\0';
          back = 1;
        }
      }
    }
    if (lineSize == 0) {
      freeAll(trimmedLine);
      continue;
    }
    if (strcmp(trimmedLine[0], "j") == 0) {
        handle_j();
        freeAll(trimmedLine);
        continue;
    }
    if (strcmp(trimmedLine[0], "myw") == 0) {
        int i;
        int illegal = 0;
        for (i = 1; i < lineSize; i++) {
          if (digitString(trimmedLine[i]) == 0) {
            illegal = 1;
            break;
          }
        }
        if (illegal == 0) {
          int pidcount = atoi(trimmedLine[1]);
          freeAll(trimmedLine);
          if (pidcount == 0)
            continue;
          handle_myw(pidcount);
          continue;
        }
    }
    int pid = fork();
    if (pid < 0) {
      perror("cannot fork");
    // child
    } else if (pid == 0) {
      execvp(trimmedLine[0], trimmedLine);
      fprintf(stderr, "%s: Command not found\n", trimmedLine[0]);
      fflush(stdout);
      freeAll(trimmedLine);
      exit(1);
    // parent
    } else {
      if (num != 0)
        children = realloc(children, (num+1)*sizeof(struct childinfo));
      if (children == NULL) {
        perror("realloc failed");
        exit(0);
      }
      children[num].back = back;
      children[num].pid = pid;
      int size = 0;
      int j;
      for (j = 0; j < lineSize; j++) {
        size += (strlen(trimmedLine[j]) + 1);
      }
      children[num].name = (char*)malloc((size)*sizeof(char));
      if (children[num].name == NULL) {
        perror("malloc failed");
        exit(0);
      }
      strcpy(children[num].name, trimmedLine[0]);
      for (j = 1; j < lineSize; j++) {
        strcat(children[num].name, " ");
        strcat(children[num].name, trimmedLine[j]);
      }
      num++;
      if (back == 0) {
        waitpid(pid, NULL, 0);
      }
      freeAll(trimmedLine);
      continue;
    }
  }
  fclose(inFile);
  int i;
  for (i = 0; i < num; i++) {
    free(children[i].name);
  }
  free(children);
  return 0;
}

