/*
 * teste_limites.c - casos limite e invalidos de sync_car.c
 * Compilar e rodar: make limites
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "sync_car.h"

static struct station estacao;
static int embarcados = 0;

static void *passageiro(void *arg)
{
	station_wait_for_car(&estacao);
	__sync_add_and_fetch(&embarcados, 1);
	station_on_board(&estacao);
	return NULL;
}

static void cria_passageiros(int n)
{
	for (int i = 0; i < n; i++) {
		pthread_t t;
		pthread_create(&t, NULL, passageiro, NULL);
		pthread_detach(t);
	}
	usleep(100000); /* da tempo de todos chegarem na plataforma */
}

static void travou(int sig)
{
	(void) sig;
	printf("FALHOU: station_load_car travou (timeout)\n");
	exit(1);
}

static int falhas = 0;

static void confere(const char *caso, int esperado)
{
	int obtido = __sync_add_and_fetch(&embarcados, 0);
	printf("%-50s total esperado=%d obtido=%d %s\n", caso, esperado, obtido,
	       obtido == esperado ? "OK" : "FALHOU");
	if (obtido != esperado)
		falhas++;
}

int main(void)
{
	station_init(&estacao);
	signal(SIGALRM, travou);
	alarm(5);

	station_load_car(&estacao, -5);
	confere("invalido: carro com -5 assentos, sem passageiros", 0);

	station_load_car(&estacao, 0);
	confere("limite: carro com 0 assentos, sem passageiros", 0);

	cria_passageiros(3);
	station_load_car(&estacao, 0);
	confere("limite: 3 esperando, carro com 0 assentos", 0);

	station_load_car(&estacao, -1);
	confere("invalido: 3 esperando, carro com -1 assentos", 0);

	station_load_car(&estacao, 10);
	confere("3 esperando, carro com 10 assentos", 3);

	cria_passageiros(5);
	station_load_car(&estacao, 2);
	confere("5 esperando, carro com 2 assentos", 5);

	station_load_car(&estacao, 3);
	confere("3 restantes, carro com exatamente 3 assentos", 8);

	station_load_car(&estacao, 4);
	confere("limite: ninguem esperando, carro com 4 assentos", 8);

	if (falhas == 0)
		printf("Todos os casos passaram\n");
	return falhas ? 1 : 0;
}
