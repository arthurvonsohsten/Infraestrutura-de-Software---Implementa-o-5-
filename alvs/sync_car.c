/* sync_car.c - sincronizacao do metro: 1 mutex, 2 conds, sem busy-wait */

#include <pthread.h>
#include "sync_car.h"

void station_init(struct station *station)
{
	pthread_mutex_init(&station->lock, NULL);
	pthread_cond_init(&station->chegou_carro, NULL);
	pthread_cond_init(&station->embarque_fim, NULL);

	station->passageiros_esperando = 0;
	station->assentos_livres = 0;
	station->passageiros_embarcando = 0;
}

/* carro: so parte quando ninguem embarca e (lotou ou fila vazia) */
void station_load_car(struct station *station, int count)
{
	pthread_mutex_lock(&station->lock);

	if (count < 0)
		count = 0;

	station->assentos_livres = count;

	/* acorda todos: embarque simultaneo */
	if (count > 0 && station->passageiros_esperando > 0)
		pthread_cond_broadcast(&station->chegou_carro);

	while ((station->assentos_livres > 0 && station->passageiros_esperando > 0) ||
	       station->passageiros_embarcando > 0)
		pthread_cond_wait(&station->embarque_fim, &station->lock);

	/* carro partiu: sobras nao valem mais */
	station->assentos_livres = 0;

	pthread_mutex_unlock(&station->lock);
}

/* passageiro: espera assento livre e o reserva */
void station_wait_for_car(struct station *station)
{
	pthread_mutex_lock(&station->lock);

	station->passageiros_esperando++;

	while (station->assentos_livres == 0)
		pthread_cond_wait(&station->chegou_carro, &station->lock);

	station->assentos_livres--;
	station->passageiros_esperando--;
	station->passageiros_embarcando++;

	pthread_mutex_unlock(&station->lock);
}

/* passageiro sentou: o ultimo avisa o carro */
void station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->lock);

	station->passageiros_embarcando--;

	if (station->passageiros_embarcando == 0)
		pthread_cond_signal(&station->embarque_fim);

	pthread_mutex_unlock(&station->lock);
}
