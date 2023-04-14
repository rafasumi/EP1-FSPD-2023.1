#include "trio.h"

void init_trio(trio_t* t) {
  pthread_mutex_init(&(t->mutex), NULL);
  pthread_cond_init(&(t->end_trio), NULL);
  pthread_cond_init(&(t->new_type), NULL);
  for (int i = 0; i < 3; i++) {
    t->reserved_type[i] = 0;
  }
  t->leave_count = 0;
}

void destroy_trio(trio_t* t) {
  pthread_mutex_destroy(&(t->mutex));
  pthread_cond_destroy(&(t->end_trio));
  pthread_cond_destroy(&(t->new_type));
}

void trio_enter(trio_t* t, int my_type) {
  pthread_mutex_lock(&(t->mutex));
  while (t->trio_running || t->reserved_type[my_type - 1]) {
    pthread_cond_wait(&(t->end_trio), &(t->mutex));
  }

  t->reserved_type[my_type - 1] = 1;
  int next_type = my_type % 3;
  int prev_type = (my_type + 1) % 3;
  pthread_cond_broadcast(&(t->new_type));
  while (!t->reserved_type[next_type] || !t->reserved_type[prev_type]) {
    pthread_cond_wait(&(t->new_type), &(t->mutex));
  }
  t->trio_running = 1;
  pthread_mutex_unlock(&(t->mutex));
}

void trio_leave(trio_t* t, int my_type) {
  pthread_mutex_lock(&(t->mutex));
  t->leave_count++;
  t->reserved_type[my_type - 1] = 0;
  if (t->leave_count == 3) {
    t->leave_count = 0;
    t->trio_running = 0;
    pthread_cond_broadcast(&(t->end_trio));
  }
  pthread_mutex_unlock(&(t->mutex));
}
