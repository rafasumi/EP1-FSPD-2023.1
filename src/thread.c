/***************************************************************************
 *  thread.c
 *  Implementação da função a ser usada como start routine no ptread_create.
 ***************************************************************************/

#include "thread.h"
#include "spend_time.h"

void* trio_thread_routine(void* args) {
  thread_args* t_args = ((thread_args*)args);
  spend_time(t_args->tid, t_args->ttype, "S", t_args->tsolo);
  trio_enter(t_args->trio, t_args->ttype);
  spend_time(t_args->tid, t_args->ttype, "T", t_args->ttrio);
  trio_leave(t_args->trio, t_args->ttype);
  pthread_exit(NULL);
}