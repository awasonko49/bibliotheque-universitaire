#ifndef STRUCTURES_H
#define STRUCTURES_H

/*
 * Définition des structures principales de la bibliothèque.
 * Ce fichier centralise les types Ouvrage, Lecteur et Emprunt,
 * ainsi que les deux représentations de tableaux utilisés par le projet.
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes
#define MAX_STATIQUE     100000
#define CAPACITE_INIT    8
#define FACTEUR_GROWTH   2

#define TITRE_LEN        100
#define AUTEUR_LEN       80
#define ISBN_LEN         20
#define GENRE_LEN        40
#define NOM_LEN          50
#define EMAIL_LEN        100
#define TEL_LEN          20
#define STATUT_LEN       20

// Declarations anticipees
typedef struct Ouvrage  Ouvrage;
typedef struct Lecteur  Lecteur;
typedef struct Emprunt  Emprunt;
typedef struct Noeud    Noeud;
typedef struct ListeChainee ListeChainee;

struct Emprunt {
    int     id_emprunt;
    time_t  date_emprunt;
    time_t  date_retour_prevue;
    time_t  date_retour_reelle; // -1 si pas encore rendu
    char    statut[STATUT_LEN]; // "en_cours", "rendu", "retard"

    Ouvrage *ouvrage;
    Lecteur *lecteur;
    Emprunt *suivant;
};

struct Ouvrage {
    int     id_ouvrage;
    char    titre[TITRE_LEN];
    char    auteur[AUTEUR_LEN];
    char    isbn[ISBN_LEN];
    int     annee_publication;
    char    genre[GENRE_LEN];
    int     nb_exemplaires_total;
    int     nb_exemplaires_dispo;

    Emprunt *emprunts;
};

struct Lecteur {
    int     id_lecteur;
    char    nom[NOM_LEN];
    char    prenom[NOM_LEN];
    char    email[EMAIL_LEN];
    char    telephone[TEL_LEN];
    time_t  date_inscription;
    int     nb_emprunts_actifs;

    Emprunt *emprunts;
};

struct Noeud {
    Ouvrage valeur;
    Noeud *suivant;
};

struct ListeChainee {
    Noeud *tete;
    int taille;
};

// Tableau statique
typedef struct {
    Ouvrage data[MAX_STATIQUE];
    int     taille;
} TabStatique;

TabStatique *ts_creer(void);
int          ts_inserer(TabStatique *ts, Ouvrage o);
Ouvrage     *ts_rechercher_id(TabStatique *ts, int id);
Ouvrage     *ts_rechercher_titre(TabStatique *ts, const char *prefixe);
Ouvrage    **ts_rechercher_intervalle_annee(TabStatique *ts, int min, int max, int *count);
int          ts_maj_ouvrage(TabStatique *ts, int id, Ouvrage nouvelle_valeur);
int          ts_supprimer(TabStatique *ts, int id);
void         ts_tri_insertion(TabStatique *ts);        // tri par titre
void         ts_tri_rapide(TabStatique *ts, int g, int d); // tri par annee
double       ts_moyenne_annee(TabStatique *ts);
double       ts_mediane_annee(TabStatique *ts);
double       ts_ecart_type_annee(TabStatique *ts);
int          ts_max_exemplaires(TabStatique *ts);
int          ts_min_exemplaires(TabStatique *ts);
void         ts_afficher(TabStatique *ts);
void         ts_detruire(TabStatique *ts);

// Tableau dynamique
typedef struct {
    Ouvrage **data;
    int       taille;
    int       capacite;
} TabDynamique;

TabDynamique *td_creer(void);
int           td_inserer(TabDynamique *td, Ouvrage *o);
Ouvrage      *td_rechercher_id(TabDynamique *td, int id);
Ouvrage      *td_rechercher_titre(TabDynamique *td, const char *prefixe);
Ouvrage     **td_rechercher_intervalle_annee(TabDynamique *td, int min, int max, int *count);
int           td_maj_ouvrage(TabDynamique *td, int id, Ouvrage *nouvelle_valeur);
int           td_supprimer(TabDynamique *td, int id);
void          td_tri_insertion(TabDynamique *td);           // tri par titre
void          td_tri_rapide(TabDynamique *td, int g, int d);    // tri par annee
double        td_moyenne_annee(TabDynamique *td);
double        td_mediane_annee(TabDynamique *td);
double        td_ecart_type_annee(TabDynamique *td);
int           td_max_exemplaires(TabDynamique *td);
int           td_min_annee(TabDynamique *td);
void          td_afficher(TabDynamique *td);
void          td_detruire(TabDynamique *td);

#endif
