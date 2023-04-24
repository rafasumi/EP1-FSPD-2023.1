/*************************************************************************
 *  trio.h
 *  Declaração da estrutura trio_t, que é responsável pela abstração de
 *  sincronização, além das funções necessárias para ter acesso às
 *  funcionalidades da estrutura.
 *************************************************************************/

#ifndef TRIO_H
#define TRIO_H

#include <pthread.h>

/**
 *  struct trio_t
 *  Estrutura usada para representar um trio e gerenciar toda a lógica de
 *  execução das threads em trio.
 */
typedef struct {
  /**
   *  Trava mutex usada para garantir exclusão mútua.
   */
  pthread_mutex_t mutex;

  /**
   *  Variável de condição que indica se o trio terminou de executar.
   */
  pthread_cond_t end_trio;

  /**
   *  Variável de condição que indica se uma thread de um novo tipo entrou no
   *  trio.
   */
  pthread_cond_t new_type;

  /**
   *  Array usado para indicar se uma thread de um determinado tipo já
   *  "reservou" um lugar no trio. Se reserved_type[i] = 1, então uma thread do
   *  tipo i + 1 já está aguardando para executar no trio.
   */
  int reserved_type[3];

  /**
   *  Representa o número de threads do trio que já terminaram de executar o seu
   *  tempo em conjunto.
   */
  int leave_count;

  /**
   *  Flag que indica se o trio está executando, ou seja, se ainda há alguma
   *  thread que não terminou o seu tempo ttrio.
   */
  int trio_running;
} trio_t;

/**
 *  Inicializa os atributos da estrutura trio_t, como a trava mutex e as
 *  variáveis de condição usadas.
 */
void init_trio(trio_t* t);

/**
 *  Função usada para destruir a trava mutex e as variáveis de condição usadas
 *  pelo trio.
 */
void destroy_trio(trio_t* t);

/**
 *  Função usada para que uma thread possa peça para entrar em um trio,
 *  indicando seu tipo.
 */
void trio_enter(trio_t* t, int my_type);

/**
 *  Função usada para que uma thread que está no trio sinalize que completou sua
 *  parte.
 */
void trio_leave(trio_t* t, int my_type);

#endif