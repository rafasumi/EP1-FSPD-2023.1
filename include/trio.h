#ifndef TRIO_H
#define TRIO_H

#include <pthread.h>

typedef struct {
  // ...
} trio_t;

void init_trio(trio_t* t);
void trio_enter(trio_t* t, int my_type);
void trio_leave(trio_t* t);

#endif