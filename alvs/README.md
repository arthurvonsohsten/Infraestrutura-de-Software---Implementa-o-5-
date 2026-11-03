# Implementação 4 – Sincronização do Metrô (METROREC)

Aluno: Arthur de Lima Von Sohsten (alvs)

## Arquivos

| Arquivo | Descrição |
|---|---|
| `sync_car.h` | Declaração da `struct station` (1 mutex, 2 variáveis de condição e 3 contadores) e protótipos das funções. |
| `sync_car.c` | Implementação de `station_init`, `station_load_car`, `station_wait_for_car` e `station_on_board`. |
| `sync_car_test.c` | Programa de teste fornecido pelo professor (contém a `main` do executável `sync_car`). |
| `teste_limites.c` | Teste extra com casos limite (0 assentos, ninguém esperando, lotação exata) e inválidos (assentos negativos). |
| `Makefile` | Compila, testa e limpa o projeto. |
| `evidencias.log` | Log dos comandos e saídas do terminal, gravado com `script`. |

## Sistema operacional

Implementado e testado no **Linux Ubuntu (WSL2)** com **gcc** e **GNU Make**.
Usa apenas pthreads, então também roda em outros Linux/Unix e no macOS.

## Como compilar

```sh
make
```

Gera o executável `sync_car`.

## Como executar

```sh
./sync_car
```

Se tudo estiver certo, a última linha impressa é `Looks good!`.

## Como testar

```sh
make test      # roda o sync_car 100 vezes seguidas; para no primeiro erro
make limites   # compila e roda o teste de casos limite e invalidos
```

## Como limpar

```sh
make clean
```

Remove os executáveis e os arquivos objeto.
