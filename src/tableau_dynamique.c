#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tableau_dynamique.h"

/*
 * Implémentation du tableau dynamique pour la bibliothèque.
 * Ce module gère les opérations sur les ouvrages avec redimensionnement.
 */

// Création d’un tableau dynamique de pointeurs vers Ouvrage
TabDynamique *td_creer(void) {
    TabDynamique *td = malloc(sizeof(*td));
    if (!td) return NULL;

    td->capacite = CAPACITE_INIT;
    td->taille = 0;
    td->data = calloc((size_t)td->capacite, sizeof(*td->data));
    if (!td->data) {
        free(td);
        return NULL;
    }

    return td;
}

// Destruction du tableau dynamique
void td_detruire(TabDynamique *td) {
    if (!td) return;
    for (int i = 0; i < td->taille; i++) {
        free(td->data[i]);
    }
    free(td->data);
    free(td);
}

// Insertion d’un ouvrage O(n) car on a agrandit avec realloc
int td_inserer(TabDynamique *td, Ouvrage *o) {
    if (!td || !o) return 0;

    if (td->taille >= td->capacite) {
        int nouvelle_capacite = td->capacite * 2;
        Ouvrage **nouveau = realloc(td->data, (size_t)nouvelle_capacite * sizeof(*nouveau));
        if (!nouveau) return 0;
        td->data = nouveau;
        td->capacite = nouvelle_capacite;
    }

    td->data[td->taille++] = o;
    return 1;
}

// Recherche par ID, O(n)
Ouvrage *td_rechercher_id(TabDynamique *td, int id) {
    if (!td) return NULL;
    for (int i = 0; i < td->taille; i++) {
        if (td->data[i]->id_ouvrage == id) return td->data[i];
    }
    return NULL;
}

// Recherche par titre (préfixe) , O(n)
Ouvrage *td_rechercher_titre(TabDynamique *td, const char *prefixe) {
    if (!td || !prefixe) return NULL;
    size_t len = strlen(prefixe);
    for (int i = 0; i < td->taille; i++) {
        if (strncmp(td->data[i]->titre, prefixe, len) == 0) {
            return td->data[i];
        }
    }
    return NULL;
}

Ouvrage **td_rechercher_intervalle_annee(TabDynamique *td, int min, int max, int *count) {
    if (!td || !count) return NULL;

    int n = 0;
    for (int i = 0; i < td->taille; i++) {
        if (td->data[i]->annee_publication >= min && td->data[i]->annee_publication <= max)
            n++;
    }

    *count = n;
    if (n == 0) return NULL;

    Ouvrage **resultats = calloc((size_t)n, sizeof(*resultats));
    if (!resultats) return NULL;

    int pos = 0;
    for (int i = 0; i < td->taille; i++) {
        if (td->data[i]->annee_publication >= min && td->data[i]->annee_publication <= max)
            resultats[pos++] = td->data[i];
    }

    return resultats;
}

int td_maj_ouvrage(TabDynamique *td, int id, Ouvrage *nouvelle_valeur) {
    if (!td || !nouvelle_valeur) return 0;

    for (int i = 0; i < td->taille; i++) {
        if (td->data[i]->id_ouvrage == id) {
            *td->data[i] = *nouvelle_valeur;
            return 1;
        }
    }
    return 0;
}

// Suppression par ID , O(n)
int td_supprimer(TabDynamique *td, int id) {
    if (!td) return 0;
    for (int i = 0; i < td->taille; i++) {
        if (td->data[i]->id_ouvrage == id) {
            free(td->data[i]);
            for (int j = i; j < td->taille - 1; j++) {
                td->data[j] = td->data[j + 1];
            }
            td->data[td->taille - 1] = NULL;
            td->taille--;
            return 1;
        }
    }
    return 0;
}

// Tri par insertion (titre) , O(n²)
void td_tri_insertion(TabDynamique *td) {
    if (!td) return;
    for (int i = 1; i < td->taille; i++) {
        Ouvrage *cle = td->data[i];
        int j = i - 1;
        while (j >= 0 && strcmp(td->data[j]->titre, cle->titre) > 0) {
            td->data[j + 1] = td->data[j];
            j--;
        }
        td->data[j + 1] = cle;
    }
}

// Partition pour tri rapide (année) , O(n log n)
static int partition(TabDynamique *td, int g, int d) {
    int pivot = td->data[d]->annee_publication;
    int i = g - 1;
    for (int j = g; j < d; j++) {
        if (td->data[j]->annee_publication <= pivot) {
            i++;
            Ouvrage *tmp = td->data[i];
            td->data[i] = td->data[j];
            td->data[j] = tmp;
        }
    }
    Ouvrage *tmp = td->data[i + 1];
    td->data[i + 1] = td->data[d];
    td->data[d] = tmp;
    return i + 1;
}

// Tri rapide (année), O(n log n)
void td_tri_rapide(TabDynamique *td, int g, int d) {
    if (!td || g >= d) return;
    int p = partition(td, g, d);
    td_tri_rapide(td, g, p - 1);
    td_tri_rapide(td, p + 1, d);
}

// Moyenne des années,  O(n)
double td_moyenne_annee(TabDynamique *td) {
    if (!td || td->taille == 0) return 0.0;
    double somme = 0;
    for (int i = 0; i < td->taille; i++) {
        somme += td->data[i]->annee_publication;
    }
    return somme / td->taille;
}

// Médiane des années , O(n)
double td_mediane_annee(TabDynamique *td) {
    if (!td || td->taille == 0) return 0.0;
    td_tri_rapide(td, 0, td->taille - 1);
    if (td->taille % 2 == 1) {
        return td->data[td->taille / 2]->annee_publication;
    } else {
        int mid = td->taille / 2;
        return (td->data[mid - 1]->annee_publication + td->data[mid]->annee_publication) / 2.0;
    }
}

// Écart-type des années , O(n)
double td_ecart_type_annee(TabDynamique *td) {
    if (!td || td->taille == 0) return 0.0;
    double moyenne = td_moyenne_annee(td);
    double variance = 0.0;
    for (int i = 0; i < td->taille; i++) {
        double diff = td->data[i]->annee_publication - moyenne;
        variance += diff * diff;
    }
    variance /= td->taille;
    return sqrt(variance);
}

// Maximum exemplaires , O(n)
int td_max_exemplaires(TabDynamique *td) {
    if (!td || td->taille == 0) return 0;
    int max = td->data[0]->nb_exemplaires_total;
    for (int i = 1; i < td->taille; i++) {
        if (td->data[i]->nb_exemplaires_total > max) {
            max = td->data[i]->nb_exemplaires_total;
        }
    }
    return max;
}

// Minimum année , O(n)
int td_min_annee(TabDynamique *td) {
    if (!td || td->taille == 0) return 0;
    int min = td->data[0]->annee_publication;
    for (int i = 1; i < td->taille; i++) {
        if (td->data[i]->annee_publication < min) {
            min = td->data[i]->annee_publication;
        }
    }
    return min;
}

// Affichage
void td_afficher(TabDynamique *td) {
    if (!td) return;

    const char *sep = "+------+--------------------------------------+-----------+-------------+";
    printf("\n=== Liste des ouvrages (tableau dynamique) ===\n");
    printf("%s\n", sep);
    printf("| %-4s | %-36s | %-9s | %-11s |\n", "ID", "Titre", "Annee", "Exemplaires");
    printf("%s\n", sep);

    for (int i = 0; i < td->taille; i++) {
        printf("| %-4d | %-36.36s | %-9d | %-11d |\n",
               td->data[i]->id_ouvrage,
               td->data[i]->titre,
               td->data[i]->annee_publication,
               td->data[i]->nb_exemplaires_total);
    }

    printf("%s\n", sep);
}
