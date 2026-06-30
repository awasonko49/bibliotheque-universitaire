#ifndef UTILS_H
#define UTILS_H

/*
 * Déclarations des utilitaires de génération et d'affichage.
 * Elles servent aux programmes principaux et au benchmark.
 */

#include "structure.h"

Ouvrage ouvrage_aleatoire(int id);
void    ouvrage_afficher(const Ouvrage *ouvrage);

#endif
