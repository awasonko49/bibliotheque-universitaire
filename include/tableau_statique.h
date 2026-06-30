#ifndef TABLEAU_STATIQUE_H
#define TABLEAU_STATIQUE_H

/*
 * Interface du tableau statique.
 * Les opérations de base sont définies ici pour gérer un ensemble
 * d'ouvrages à capacité fixe.
 */

#include "structure.h"

TabStatique *ts_creer(void);
int          ts_inserer(TabStatique *ts, Ouvrage o);
Ouvrage     *ts_rechercher_id(TabStatique *ts, int id);
Ouvrage     *ts_rechercher_titre(TabStatique *ts, const char *prefixe);
Ouvrage    **ts_rechercher_intervalle_annee(TabStatique *ts, int min, int max, int *count);
int          ts_maj_ouvrage(TabStatique *ts, int id, Ouvrage nouvelle_valeur);
int          ts_supprimer(TabStatique *ts, int id);
void         ts_tri_insertion(TabStatique *ts);
void         ts_tri_rapide(TabStatique *ts, int g, int d);
double       ts_moyenne_annee(TabStatique *ts);
double       ts_mediane_annee(TabStatique *ts);      
double       ts_ecart_type_annee(TabStatique *ts);   
int          ts_max_exemplaires(TabStatique *ts);
int          ts_min_exemplaires(TabStatique *ts);
void         ts_afficher(TabStatique *ts);
void         ts_detruire(TabStatique *ts);

#endif