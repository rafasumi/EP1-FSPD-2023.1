#ifndef TRIO_H
#define TRIO_H

#include <pthread.h>

typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t end_trio;
  pthread_cond_t new_type;
  int reserved_type[3];
  int leave_count;
  int trio_running;
} trio_t;

void init_trio(trio_t* t);
void destroy_trio(trio_t* t);
void trio_enter(trio_t* t, int my_type);
void trio_leave(trio_t* t, int my_type);

#endif