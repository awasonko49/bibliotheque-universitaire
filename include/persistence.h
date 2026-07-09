#ifndef PERSISTANCE_H
#define PERSISTANCE_H

/*
 * Interface de persistance de la bibliotheque.
 * Elle permet de sauvegarder et recharger les ouvrages
 * dans les versions statique et dynamique.
 */

#include "structure.h"

#include "tableau_statique.h"
#include "tableau_dynamique.h"
#include "liste_chainee.h"

// === Fonctions pour tableau statique ===
int  sauvegarder_ouvrages(const TabStatique *ts, const char *fichier);
int  charger_ouvrages(TabStatique *ts, const char *fichier);

// === Fonctions pour tableau dynamique ===
int  sauvegarder_ouvrages_dyn(const TabDynamique *td, const char *fichier);
int  charger_ouvrages_dyn(TabDynamique *td, const char *fichier);

// === Fonctions pour liste chainee ===
int  sauvegarder_ouvrages_lc(const ListeChainee *lc, const char *fichier);
int  charger_ouvrages_lc(ListeChainee *lc, const char *fichier);

#endif