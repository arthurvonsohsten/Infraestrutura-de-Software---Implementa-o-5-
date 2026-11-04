# Infraestrutura de Software – Implementação 4

Sincronização do Metrô (METROREC) com pthreads: mutex e variáveis de condição.

- Código, Makefile e README da entrega: [`alvs/`](alvs/)
- Relatório: [`alvs.pdf`](alvs.pdf)

## Uso rápido (Linux/macOS)

```sh
cd alvs
make        # compila e gera ./sync_car
./sync_car  # executa o teste (termina com "Looks good!")
make test   # executa 100 vezes seguidas
make clean  # remove o executável
```
