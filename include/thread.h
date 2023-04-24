/*************************************************************************
 *  thread.h
 *  Declaração da estrutura para ser enviada como argumento da função de
 *  start routine para o pthread_create, além da função de start routine
 *  em si.
 *************************************************************************/

#ifndef THREAD_H
#define THREAD_H

#include "trio.h"
#include <pthread.h>

/**
 *  struct thread_args
 *  Estrutura de dados usada para enviar múltiplos valores como argumento para a
 *  função de start routine.
 */
typedef struct {
  int tid;
  int ttype;
  int tsolo;
  int ttrio;
  trio_t* trio;
} thread_args;

/**
 *  Função usada como start routine para o pthread_create. Basicamente aplica as
 *  ações descritas na especificação do EP para o "corpo" de cada thread.
 */
void* trio_thread_routine(void* args);

#endif