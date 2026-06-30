CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lm

all: bibliotheque_dyn bibliotheque_stat bibliotheque_lc

bibliotheque_dyn: src/main_dyn.c src/tableau_dynamique.c src/utils.c src/persistence.c src/tableau_statique.c src/liste_chainee.c
	$(CC) $(CFLAGS) src/main_dyn.c src/tableau_dynamique.c src/utils.c src/persistence.c src/tableau_statique.c src/liste_chainee.c $(LDFLAGS) -o bibliotheque_dyn

bibliotheque_stat: src/main_stat.c src/tableau_statique.c src/utils.c src/persistence.c src/tableau_dynamique.c src/liste_chainee.c
	$(CC) $(CFLAGS) src/main_stat.c src/tableau_statique.c src/utils.c src/persistence.c src/tableau_dynamique.c src/liste_chainee.c $(LDFLAGS) -o bibliotheque_stat

bibliotheque_lc: src/main_lc.c src/liste_chainee.c src/utils.c src/persistence.c
	$(CC) $(CFLAGS) src/main_lc.c src/liste_chainee.c src/utils.c src/persistence.c $(LDFLAGS) -o bibliotheque_lc

benchmark: src/benchmark.c src/tableau_statique.c src/tableau_dynamique.c src/utils.c src/persistence.c src/liste_chainee.c
	$(CC) $(CFLAGS) src/benchmark.c src/tableau_statique.c src/tableau_dynamique.c src/utils.c src/persistence.c src/liste_chainee.c $(LDFLAGS) -o benchmark

clean:
	rm -f bibliotheque_dyn bibliotheque_stat bibliotheque_lc benchmark
