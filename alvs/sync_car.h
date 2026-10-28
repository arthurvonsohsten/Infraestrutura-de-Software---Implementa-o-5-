#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

/*
 * Estado compartilhado da estacao.
 * Todos os campos sao protegidos por um UNICO lock (lock).
 */
struct station {
	pthread_mutex_t lock;          /* unico mutex da estacao */
	pthread_cond_t chegou_carro;   /* passageiros esperam aqui por um assento livre */
	pthread_cond_t embarque_fim;   /* o carro espera aqui o embarque terminar */

	int passageiros_esperando;     /* passageiros na plataforma sem assento */
	int assentos_livres;           /* assentos do carro atual ainda nao reservados */
	int passageiros_embarcando;    /* reservaram assento mas ainda nao sentaram */
};

void station_init(struct station *station);
void station_load_car(struct station *station, int count);
void station_wait_for_car(struct station *station);
void station_on_board(struct station *station);

#endif
