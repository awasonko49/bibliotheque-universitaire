#ifndef TABLEAU_DYNAMIQUE_H
#define TABLEAU_DYNAMIQUE_H

/*
 * Interface du tableau dynamique.
 * Ce module offre les opérations sur un tableau extensible
 * pour stocker les ouvrages de la bibliothèque.
 */

#include "structure.h"

TabDynamique *td_creer(void);
int           td_inserer(TabDynamique *td, Ouvrage *o);
Ouvrage      *td_rechercher_id(TabDynamique *td, int id);
Ouvrage      *td_rechercher_titre(TabDynamique *td, const char *prefixe);
Ouvrage     **td_rechercher_intervalle_annee(TabDynamique *td, int min, int max, int *count);
int           td_maj_ouvrage(TabDynamique *td, int id, Ouvrage *nouvelle_valeur);
int           td_supprimer(TabDynamique *td, int id);
void          td_tri_insertion(TabDynamique *td);
void          td_tri_rapide(TabDynamique *td, int g, int d);
double        td_moyenne_annee(TabDynamique *td);
double        td_mediane_annee(TabDynamique *td);
double        td_ecart_type_annee(TabDynamique *td);
int           td_max_exemplaires(TabDynamique *td);
int           td_min_annee(TabDynamique *td);
void          td_afficher(TabDynamique *td);
void          td_detruire(TabDynamique *td);
#endif