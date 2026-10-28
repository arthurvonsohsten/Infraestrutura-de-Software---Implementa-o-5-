/*
 * sync_car.c - Sincronizacao do Metro (METROREC)
 *
 * Usa apenas um mutex e duas variaveis de condicao por estacao.
 * Nenhuma funcao faz busy-wait: quem precisa esperar dorme em
 * pthread_cond_wait, sempre dentro de um while.
 */

#include <pthread.h>
#include "sync_car.h"

/* A struct station esta declarada em sync_car.h (incluido acima). */

void station_init(struct station *station)
{
	pthread_mutex_init(&station->lock, NULL);
	pthread_cond_init(&station->chegou_carro, NULL);
	pthread_cond_init(&station->embarque_fim, NULL);

	station->passageiros_esperando = 0;
	station->assentos_livres = 0;
	station->passageiros_embarcando = 0;
}

/*
 * Chamada pelo carro quando abre as portas. So retorna quando:
 * ninguem esta no meio do embarque E (o carro lotou OU nao ha mais
 * passageiros esperando).
 */
void station_load_car(struct station *station, int count)
{
	pthread_mutex_lock(&station->lock);

	station->assentos_livres = count;

	/* acorda todos os passageiros: varios podem embarcar ao mesmo tempo */
	if (count > 0 && station->passageiros_esperando > 0)
		pthread_cond_broadcast(&station->chegou_carro);

	while ((station->assentos_livres > 0 && station->passageiros_esperando > 0) ||
	       station->passageiros_embarcando > 0)
		pthread_cond_wait(&station->embarque_fim, &station->lock);

	/* o carro vai partir: assentos que sobraram nao podem mais ser usados */
	station->assentos_livres = 0;

	pthread_mutex_unlock(&station->lock);
}

/*
 * Chamada pelo passageiro ao chegar. So retorna quando ha um carro
 * na estacao com assento livre; nesse momento o assento fica reservado.
 */
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

/*
 * Chamada quando o passageiro ja se sentou. O ultimo a sentar
 * avisa o carro, que confere se ja pode partir.
 */
void station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->lock);

	station->passageiros_embarcando--;

	if (station->passageiros_embarcando == 0)
		pthread_cond_signal(&station->embarque_fim);

	pthread_mutex_unlock(&station->lock);
}
