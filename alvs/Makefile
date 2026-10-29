CC = gcc
CFLAGS = -Wall -pthread
ALVO = sync_car
FONTES = sync_car.c sync_car_test.c

all: $(ALVO)

$(ALVO): $(FONTES) sync_car.h
	$(CC) $(CFLAGS) -o $(ALVO) $(FONTES)

# roda o teste 100 vezes seguidas; para no primeiro erro
test: $(ALVO)
	@for i in $$(seq 1 100); do \
		./$(ALVO) > /dev/null || { echo "Falhou na execucao $$i"; exit 1; }; \
	done; echo "100 execucoes OK"

# casos limite e invalidos
limites: sync_car.c teste_limites.c sync_car.h
	$(CC) $(CFLAGS) -o teste_limites sync_car.c teste_limites.c
	./teste_limites

clean:
	rm -f $(ALVO) teste_limites *.o

.PHONY: all test limites clean
