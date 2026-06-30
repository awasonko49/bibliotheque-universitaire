#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H

/*
 * Interface de la liste chainee simple.
 * Cette structure stocke les ouvrages dans une liste chainee
 * permet de comparer les performances et les principes de base.
 */

#include "structure.h"

ListeChainee *lc_creer(void);
int           lc_inserer(ListeChainee *lc, Ouvrage o);
Ouvrage      *lc_rechercher_id(ListeChainee *lc, int id);
Ouvrage      *lc_rechercher_titre(ListeChainee *lc, const char *prefixe);
Ouvrage     **lc_rechercher_intervalle_annee(ListeChainee *lc, int min, int max, int *count);
int           lc_maj_ouvrage(ListeChainee *lc, int id, Ouvrage nouvelle_valeur);
int           lc_supprimer(ListeChainee *lc, int id);
void          lc_tri_insertion(ListeChainee *lc);
void          lc_tri_rapide(ListeChainee *lc, int g, int d);
double        lc_moyenne_annee(ListeChainee *lc);
double        lc_mediane_annee(ListeChainee *lc);
double        lc_ecart_type_annee(ListeChainee *lc);
int           lc_max_exemplaires(ListeChainee *lc);
int           lc_min_annee(ListeChainee *lc);
void          lc_afficher(ListeChainee *lc);
void          lc_detruire(ListeChainee *lc);

#endif
