#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Utilitaires de génération et d'affichage des ouvrages.
 * Ce module fournit les données de test aléatoires et l'affichage
 * formaté utilisé par les interfaces de la bibliothèque.
 */

static const char *const TITRES[] = {
    "Le Petit Prince",
    "Les Miserables",
    "Dune",
    "1984",
    "La Peste",
    "Le Rouge et le Noir"
};

static const char *const AUTEURS[] = {
    "Antoine de Saint-Exupery",
    "Victor Hugo",
    "Frank Herbert",
    "George Orwell",
    "Albert Camus",
    "Stendhal"
};

static const char *const GENRES[] = {
    "Roman",
    "Classique",
    "Science-fiction",
    "Dystopie",
    "Philosophie",
    "Historique"
};

Ouvrage ouvrage_aleatoire(int id) {
    Ouvrage o;
    memset(&o, 0, sizeof(o));

    o.id_ouvrage = id;
    snprintf(o.titre, sizeof(o.titre), "%s", TITRES[rand() % (sizeof(TITRES) / sizeof(TITRES[0]))]);
    snprintf(o.auteur, sizeof(o.auteur), "%s", AUTEURS[rand() % (sizeof(AUTEURS) / sizeof(AUTEURS[0]))]);
    snprintf(o.isbn, sizeof(o.isbn), "%03d-%03d-%03d", rand() % 1000, rand() % 1000, rand() % 1000);
    o.annee_publication = 1900 + (rand() % 125);
    snprintf(o.genre, sizeof(o.genre), "%s", GENRES[rand() % (sizeof(GENRES) / sizeof(GENRES[0]))]);
    o.nb_exemplaires_total = 1 + (rand() % 20);
    o.nb_exemplaires_dispo = 0 + (rand() % (o.nb_exemplaires_total + 1));

    return o;
}

void ouvrage_afficher(const Ouvrage *ouvrage) {
    if (!ouvrage) return;

    printf("ID: %d | Titre: %s | Auteur: %s | ISBN: %s | Annee: %d | Genre: %s | Exemplaires: %d/%d\n",
           ouvrage->id_ouvrage,
           ouvrage->titre,
           ouvrage->auteur,
           ouvrage->isbn,
           ouvrage->annee_publication,
           ouvrage->genre,
           ouvrage->nb_exemplaires_dispo,
           ouvrage->nb_exemplaires_total);
}
