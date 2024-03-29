/*************************************************************************
 *  main.c
 *  Contém a função main do programa para o Exerício de Programação. É
 *  responsável por fazer a leitura da entrada e chamar os procedimentos
 *  adequados.
 *************************************************************************/

#include "thread.h"
#include "trio.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Número máximo de threads no programa
#define MAX_THREADS 1000

// Tamanho máximo para a linha de entrada
#define MAX_LINE 42

int main() {
  char line[MAX_LINE];
  pthread_t threads[MAX_THREADS];

  // Além de alocar as threads estaticamente, foi preciso alocar estaticamente
  // uma estrutura de argumentos para cada thread, uma vez que alocar apenas uma
  // estrurua dentro do while gerou inconsistências devido ao acesso
  // concorrente.
  thread_args args[MAX_THREADS];

  trio_t trio;
  init_trio(&trio);

  while (fgets(line, MAX_LINE, stdin) != NULL) {
    int index = atoi(strtok(line, " ")) - 1;
    args[index].tid = index + 1;
    args[index].ttype = atoi(strtok(NULL, " "));
    args[index].tsolo = atoi(strtok(NULL, " "));
    args[index].ttrio = atoi(strtok(NULL, " "));
    args[index].trio = &trio;

    pthread_create(&threads[index], NULL, trio_thread_routine,
                   (void*)&args[index]);
  }

  // É preciso chamar pthread_join para cada thread para garantir que o programa
  // principal não terminará sua execução antes que todas as threads também
  // terminem.
  for (int i = 0; i < MAX_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  destroy_trio(&trio);

  return 0;
}
