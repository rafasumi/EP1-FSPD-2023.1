/*************************************************************************
 *  trio.c
 *  Implementação das funções associadas à abstração de sincronização de
 *  trio.
 *************************************************************************/

#include "trio.h"

void init_trio(trio_t* t) {
  pthread_mutex_init(&(t->mutex), NULL);
  pthread_cond_init(&(t->end_trio), NULL);
  pthread_cond_init(&(t->new_type), NULL);
  for (int i = 0; i < 3; i++) {
    t->reserved_type[i] = 0;
  }
  t->leave_count = 0;
  t->trio_running = 0;
}

void destroy_trio(trio_t* t) {
  pthread_mutex_destroy(&(t->mutex));
  pthread_cond_destroy(&(t->end_trio));
  pthread_cond_destroy(&(t->new_type));
}

void trio_enter(trio_t* t, int my_type) {
  pthread_mutex_lock(&(t->mutex));
  // A thread deve esperar até que o trio atual termine sua execução caso já
  // esteja executando no momento ou se uma outra thread do mesmo tipo já
  // "reservou" um lugar no trio
  while (t->trio_running || t->reserved_type[my_type - 1]) {
    pthread_cond_wait(&(t->end_trio), &(t->mutex));
  }

  // Depois que a thread passa pela primeira espera, ela já pode reservar o seu
  // lugar no trio, já que é garantido que o trio não está executando e que não
  // há uma thread do mesmo tipo com o lugar reservado.
  t->reserved_type[my_type - 1] = 1;
  // Após a thread reservar o seu lugar no trio, é preciso sinalizar que uma
  // thread de um novo tipo entrou no trio. É preciso sinalizar via broadcast
  // pois podem ter threads de diferentes tipos esperando.
  pthread_cond_broadcast(&(t->new_type));
  int next_type = my_type % 3;
  int prev_type = (my_type + 1) % 3;
  // Para que o trio possa executar, é preciso que threads dos três tipos já
  // tenham reservado um lugar no trio. Portanto, enquanto isso não ocorrer, é
  // preciso esperar até que seja sinalizado que um novo tipo reservou seu lugar
  // no trio. Entretanto, pode ser necessário continuar a espera caso ainda não
  // tenha uma thread de alguns dos tipos.
  while (!t->reserved_type[next_type] || !t->reserved_type[prev_type]) {
    pthread_cond_wait(&(t->new_type), &(t->mutex));
  }
  t->trio_running = 1;
  pthread_mutex_unlock(&(t->mutex));
}

void trio_leave(trio_t* t, int my_type) {
  pthread_mutex_lock(&(t->mutex));
  // Quando a thread sai do trio, o lugar do seu tipo deixa de ser reservado e
  // é incrementado o contador do número de threads que terminaram de executar.
  t->leave_count++;
  t->reserved_type[my_type - 1] = 0;
  // Após a última thread do trio terminar de executar (ou seja, quando
  // leave_count é igual a 3), o contador leave_count e a flag trio_running são
  // atualizados e é sinalizada que o trio terminou de executar.
  if (t->leave_count == 3) {
    t->leave_count = 0;
    t->trio_running = 0;
    pthread_cond_broadcast(&(t->end_trio));
  }
  pthread_mutex_unlock(&(t->mutex));
}
