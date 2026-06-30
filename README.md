# Bibliothèque universitaire — Projet LMI2

## Objectif
Ce projet implémente trois représentations de la même base d’ouvrages :
- tableau statique
- tableau dynamique de pointeurs vers structures
- liste chaînée simple

Cette version suit l’architecture retenue pour le projet et permet de comparer les performances de ces trois structures.

## Prérequis et dépendances
Pour compiler le projet C et générer les courbes de benchmark, il faut installer :
- un compilateur C (`gcc`)
- Python 3
- les dépendances Python listées dans `requirements.txt`

Installation des dépendances Python :
```sh
python3 -m pip install -r requirements.txt
```

## Compilation
```sh
make clean
make
```

Exécutables générés :
- `bibliotheque_stat`
- `bibliotheque_dyn`
- `bibliotheque_lc`

## Exécution
```sh
./bibliotheque_stat
./bibliotheque_dyn
./bibliotheque_lc
```

## Benchmark
```sh
make benchmark
./benchmark
```
Le fichier CSV généré est `data/benchmark.csv`.

## Graphes
```sh
python3 scripts/plot_benchmark.py
```
Les images sont générées dans `graphs/`.
Les fichiers produits comprennent :
- les graphiques de comparaison par opération (`*_comparison.png`)
- les graphiques par profil (`*_uniforme.png`, `*_gaussienne.png`, `*_tri.png`)

## Organisation du dépôt
- `src/` : code source C des programmes principaux et des implémentations
- `include/` : déclarations des structures et prototypes
- `scripts/` : scripts de benchmark et de visualisation
- `data/` : fichiers de données et résultats de benchmark
- `graphs/` : courbes générées
- `rapport/` : rapport LaTeX
