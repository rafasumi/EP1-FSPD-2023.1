#include "thread.h"
#include "trio.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_THREADS 1000

int main() {
  char line[42];
  pthread_t threads[MAX_THREADS];

  thread_args args;
  trio_t trio;
  init_trio(&trio);
  args.trio = &trio;

  while (fgets(line, sizeof line, stdin) != NULL) {
    args.tid = atoi(strtok(line, " "));
    args.ttype = atoi(strtok(NULL, " "));
    args.tsolo = atoi(strtok(NULL, " "));
    args.ttrio = atoi(strtok(NULL, " "));

    printf("%d %d %d %d\n", args.tid, args.ttype, args.tsolo, args.ttrio);
    pthread_create(&threads[args.tid - 1], NULL, trio_thread_routine,
                   (void*)&args);
  }

  return 0;
}
