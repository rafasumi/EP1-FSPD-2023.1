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
  thread_args args[MAX_THREADS];

  trio_t trio;
  init_trio(&trio);

  while (fgets(line, sizeof line, stdin) != NULL) {
    int index = atoi(strtok(line, " ")) - 1;
    args[index].tid = index + 1;
    args[index].ttype = atoi(strtok(NULL, " "));
    args[index].tsolo = atoi(strtok(NULL, " "));
    args[index].ttrio = atoi(strtok(NULL, " "));
    args[index].trio = &trio;

    pthread_create(&threads[index], NULL, trio_thread_routine,
                   (void*)&args[index]);
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  destroy_trio(&trio);

  return 0;
}
