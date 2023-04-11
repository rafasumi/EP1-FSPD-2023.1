#ifndef THREAD_H
#define THREAD_H

#include "trio.h"
#include <pthread.h>

typedef struct {
  int tid;
  int ttype;
  int tsolo;
  int ttrio;
  trio_t* trio;
} thread_args;

void* trio_thread_routine(void* args);
#endif