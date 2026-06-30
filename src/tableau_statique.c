#include "tableau_statique.h"
#include "utils.h"  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// Allocation
TabStatique *ts_creer(void) {
    TabStatique *ts = malloc(sizeof(TabStatique));
    if (!ts) return NULL;
    ts->taille = 0;
    return ts;
}

void ts_detruire(TabStatique *ts) {
    free(ts);
}

// Insertion O(1) si non plein
int ts_inserer(TabStatique *ts, Ouvrage o) {
    if (ts->taille >= MAX_STATIQUE) {
        fprintf(stderr, "Tableau plein (%d max)\n", MAX_STATIQUE);
        return -1;
    }
    ts->data[ts->taille++] = o;
    return 0;
}

// Recherche par ID, O(n)
Ouvrage *ts_rechercher_id(TabStatique *ts, int id) {
    for (int i = 0; i < ts->taille; i++)
        if (ts->data[i].id_ouvrage == id)
            return &ts->data[i];
    return NULL;
}

// Recherche par prefixe de titre ;O(n)
Ouvrage *ts_rechercher_titre(TabStatique *ts, const char *prefixe) {
    int len = strlen(prefixe);
    for (int i = 0; i < ts->taille; i++)
        if (strncasecmp(ts->data[i].titre, prefixe, len) == 0)
            return &ts->data[i];
    return NULL;
}

// Suppression : on comble avec le dernier element, O(n)
int ts_supprimer(TabStatique *ts, int id) {
    for (int i = 0; i < ts->taille; i++) {
        if (ts->data[i].id_ouvrage == id) {
            ts->data[i] = ts->data[--ts->taille];
            return 0;
        }
    }
    return -1;
}

// Tri par titre (insertion), O(n^2)
void ts_tri_insertion(TabStatique *ts) {
    for (int i = 1; i < ts->taille; i++) {
        Ouvrage cle = ts->data[i];
        int j = i - 1;
        while (j >= 0 && strcmp(ts->data[j].titre, cle.titre) > 0) {
            ts->data[j + 1] = ts->data[j];
            j--;
        }
        ts->data[j + 1] = cle;
    }
}

// Tri par annee (rapide), O(n log n) moyen
static int partition(TabStatique *ts, int g, int d) {
    int pivot = ts->data[d].annee_publication;
    int i = g - 1;

    for (int j = g; j < d; j++) {
        if (ts->data[j].annee_publication <= pivot) {
            i++;
            Ouvrage tmp = ts->data[i];
            ts->data[i] = ts->data[j];
            ts->data[j] = tmp;
        }
    }
    Ouvrage tmp = ts->data[i + 1];
    ts->data[i + 1] = ts->data[d];
    ts->data[d] = tmp;
    return i + 1;
}

// O(n log n)
void ts_tri_rapide(TabStatique *ts, int g, int d) {
    if (g < d) {
        int p = partition(ts, g, d);
        ts_tri_rapide(ts, g, p - 1);
        ts_tri_rapide(ts, p + 1, d);
    }
}

// Stats O(n)
double ts_moyenne_annee(TabStatique *ts) {
    if (ts->taille == 0) return 0.0;
    long somme = 0;
    for (int i = 0; i < ts->taille; i++)
        somme += ts->data[i].annee_publication;
    return (double)somme / ts->taille;
}

// Fonction pour calculer la mediane des annees O(n)
double ts_mediane_annee(TabStatique *ts) {
    if (ts->taille == 0) return 0.0;

    // Copie des annees dans un tableau temporaire
    int temp[ts->taille];
    for (int i = 0; i < ts->taille; i++) {
        temp[i] = ts->data[i].annee_publication;
    }

    // Tri simple (insertion ou autre) O(n)
    for (int i = 1; i < ts->taille; i++) {
        int cle = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j] > cle) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = cle;
    }

    // Calcul de la mediane O(n)
    if (ts->taille % 2 == 1) {
        return temp[ts->taille / 2]; // impair -> element du milieu
    } else {
        int mid = ts->taille / 2;
        return (temp[mid - 1] + temp[mid]) / 2.0; // pair -> moyenne des deux du milieu
    }
}

// Fonction pour calculer ecart-type des annees O(n)
double ts_ecart_type_annee(TabStatique *ts) {
    if (ts->taille == 0) return 0.0;

    // Calcul de la moyenne
    double somme = 0.0;
    for (int i = 0; i < ts->taille; i++) {
        somme += ts->data[i].annee_publication;
    }
    double moyenne = somme / ts->taille;

    // Calcul de la variance
    double variance = 0.0;
    for (int i = 0; i < ts->taille; i++) {
        double diff = ts->data[i].annee_publication - moyenne;
        variance += diff * diff;
    }
    variance /= ts->taille;

    // ecart-type = racine carree de la variance
    return sqrt(variance);
}
   //  O(n)
int ts_max_exemplaires(TabStatique *ts) {
    if (ts->taille == 0)
     return 0;
    int max = ts->data[0].nb_exemplaires_total;
    for (int i = 1; i < ts->taille; i++)
        if (ts->data[i].nb_exemplaires_total > max)
            max = ts->data[i].nb_exemplaires_total;
    return max;
}
     // O(n)
int ts_min_exemplaires(TabStatique *ts) {
    if (ts->taille == 0) 
    return 0;
    int min = ts->data[0].nb_exemplaires_total;
    for (int i = 1; i < ts->taille; i++)
        if (ts->data[i].nb_exemplaires_total < min)
            min = ts->data[i].nb_exemplaires_total;
    return min;
}

// Affichage
void ts_afficher(TabStatique *ts) {
    const char *sep =
        "+------+----------------------------------------+----------------------+--------------------+------+---------------+";
    printf("\n=== Bibliotheque — %d ouvrage(s) ===\n", ts->taille);
    printf("%s\n", sep);
    printf("| %4s | %-38s | %-20s | %-18s | %4s | %-13s |\n",
           "ID", "Titre", "Auteur", "Genre", "Annee", "Dispo / Total");
    printf("%s\n", sep);
    for (int i = 0; i < ts->taille; i++)
        ouvrage_afficher(&ts->data[i]);
    printf("%s\n", sep);
}
