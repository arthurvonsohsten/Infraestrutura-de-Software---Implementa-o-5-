#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

/* estado da estacao, protegido por um unico lock */
struct station {
	pthread_mutex_t lock;
	pthread_cond_t chegou_carro;   /* passageiros esperam assento */
	pthread_cond_t embarque_fim;   /* carro espera o embarque */

	int passageiros_esperando;     /* na plataforma, sem assento */
	int assentos_livres;           /* livres no carro atual (0 = sem carro) */
	int passageiros_embarcando;    /* com assento, ainda nao sentaram */
};

void station_init(struct station *station);
void station_load_car(struct station *station, int count);
void station_wait_for_car(struct station *station);
void station_on_board(struct station *station);

#endif
