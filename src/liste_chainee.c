#include "liste_chainee.h"
#include "utils.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * Implémentation d'une liste chaînée simple pour stocker les ouvrages.
 */

static Noeud *creer_noeud(Ouvrage valeur) {
    Noeud *n = malloc(sizeof(*n));
    if (!n) return NULL;
    n->valeur = valeur;
    n->suivant = NULL;
    return n;
}

static int comparer_annee(const void *a, const void *b) {
    const Ouvrage *oa = (const Ouvrage *)a;
    const Ouvrage *ob = (const Ouvrage *)b;
    if (oa->annee_publication < ob->annee_publication) return -1;
    if (oa->annee_publication > ob->annee_publication) return 1;
    return 0;
}

ListeChainee *lc_creer(void) {
    ListeChainee *lc = malloc(sizeof(*lc));
    if (!lc) return NULL;
    lc->tete = NULL;
    lc->taille = 0;
    return lc;
}

void lc_detruire(ListeChainee *lc) {
    if (!lc) return;
    Noeud *courant = lc->tete;
    while (courant) {
        Noeud *suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    free(lc);
}

int lc_inserer(ListeChainee *lc, Ouvrage o) {
    if (!lc) return 0;

    Noeud *n = creer_noeud(o);
    if (!n) return 0;

    n->suivant = lc->tete;
    lc->tete = n;
    lc->taille++;
    return 1;
}

Ouvrage *lc_rechercher_id(ListeChainee *lc, int id) {
    if (!lc) return NULL;
    for (Noeud *n = lc->tete; n; n = n->suivant) {
        if (n->valeur.id_ouvrage == id) return &n->valeur;
    }
    return NULL;
}

Ouvrage *lc_rechercher_titre(ListeChainee *lc, const char *prefixe) {
    if (!lc || !prefixe) return NULL;
    size_t len = strlen(prefixe);
    for (Noeud *n = lc->tete; n; n = n->suivant) {
        if (strncmp(n->valeur.titre, prefixe, len) == 0) return &n->valeur;
    }
    return NULL;
}

Ouvrage **lc_rechercher_intervalle_annee(ListeChainee *lc, int min, int max, int *count) {
    if (!lc || !count) return NULL;

    int n = 0;
    for (Noeud *courant = lc->tete; courant; courant = courant->suivant) {
        if (courant->valeur.annee_publication >= min && courant->valeur.annee_publication <= max)
            n++;
    }

    *count = n;
    if (n == 0) return NULL;

    Ouvrage **resultats = calloc((size_t)n, sizeof(*resultats));
    if (!resultats) return NULL;

    int pos = 0;
    for (Noeud *courant = lc->tete; courant; courant = courant->suivant) {
        if (courant->valeur.annee_publication >= min && courant->valeur.annee_publication <= max)
            resultats[pos++] = &courant->valeur;
    }

    return resultats;
}

int lc_maj_ouvrage(ListeChainee *lc, int id, Ouvrage nouvelle_valeur) {
    if (!lc) return 0;
    for (Noeud *n = lc->tete; n; n = n->suivant) {
        if (n->valeur.id_ouvrage == id) {
            n->valeur = nouvelle_valeur;
            return 1;
        }
    }
    return 0;
}

int lc_supprimer(ListeChainee *lc, int id) {
    if (!lc || !lc->tete) return 0;

    Noeud *precedent = NULL;
    Noeud *courant = lc->tete;

    while (courant && courant->valeur.id_ouvrage != id) {
        precedent = courant;
        courant = courant->suivant;
    }

    if (!courant) return 0;

    if (precedent) {
        precedent->suivant = courant->suivant;
    } else {
        lc->tete = courant->suivant;
    }

    free(courant);
    lc->taille--;
    return 1;
}

void lc_tri_insertion(ListeChainee *lc) {
    if (!lc || lc->taille < 2) return;

    Ouvrage *tab = calloc((size_t)lc->taille, sizeof(*tab));
    if (!tab) return;

    int i = 0;
    for (Noeud *n = lc->tete; n; n = n->suivant)
        tab[i++] = n->valeur;

    for (int j = 1; j < lc->taille; j++) {
        Ouvrage cle = tab[j];
        int k = j - 1;
        while (k >= 0 && strcmp(tab[k].titre, cle.titre) > 0) {
            tab[k + 1] = tab[k];
            k--;
        }
        tab[k + 1] = cle;
    }

    i = 0;
    for (Noeud *n = lc->tete; n; n = n->suivant)
        n->valeur = tab[i++];

    free(tab);
}

void lc_tri_rapide(ListeChainee *lc, int g, int d) {
    if (!lc || lc->taille < 2 || g >= d) return;

    Ouvrage *tab = calloc((size_t)lc->taille, sizeof(*tab));
    if (!tab) return;

    int i = 0;
    for (Noeud *n = lc->tete; n; n = n->suivant)
        tab[i++] = n->valeur;

    qsort(tab + g, (size_t)(d - g + 1), sizeof(*tab), comparer_annee);

    i = 0;
    for (Noeud *n = lc->tete; n; n = n->suivant)
        n->valeur = tab[i++];

    free(tab);
}

double lc_moyenne_annee(ListeChainee *lc) {
    if (!lc || lc->taille == 0) return 0.0;
    long somme = 0;
    for (Noeud *n = lc->tete; n; n = n->suivant)
        somme += n->valeur.annee_publication;
    return (double)somme / lc->taille;
}

double lc_mediane_annee(ListeChainee *lc) {
    if (!lc || lc->taille == 0) return 0.0;

    Ouvrage *tab = calloc((size_t)lc->taille, sizeof(*tab));
    if (!tab) return 0.0;

    int i = 0;
    for (Noeud *n = lc->tete; n; n = n->suivant)
        tab[i++] = n->valeur;

    qsort(tab, (size_t)lc->taille, sizeof(*tab), comparer_annee);

    double mediane;
    if (lc->taille % 2 == 1) {
        mediane = tab[lc->taille / 2].annee_publication;
    } else {
        int mid = lc->taille / 2;
        mediane = (tab[mid - 1].annee_publication + tab[mid].annee_publication) / 2.0;
    }
    free(tab);
    return mediane;
}

double lc_ecart_type_annee(ListeChainee *lc) {
    if (!lc || lc->taille == 0) return 0.0;

    double moyenne = lc_moyenne_annee(lc);
    double variance = 0.0;
    for (Noeud *n = lc->tete; n; n = n->suivant) {
        double diff = n->valeur.annee_publication - moyenne;
        variance += diff * diff;
    }
    variance /= lc->taille;
    return sqrt(variance);
}

int lc_max_exemplaires(ListeChainee *lc) {
    if (!lc || lc->taille == 0) return 0;
    int max = lc->tete->valeur.nb_exemplaires_total;
    for (Noeud *n = lc->tete->suivant; n; n = n->suivant) {
        if (n->valeur.nb_exemplaires_total > max)
            max = n->valeur.nb_exemplaires_total;
    }
    return max;
}

int lc_min_annee(ListeChainee *lc) {
    if (!lc || lc->taille == 0) return 0;
    int min = lc->tete->valeur.annee_publication;
    for (Noeud *n = lc->tete->suivant; n; n = n->suivant) {
        if (n->valeur.annee_publication < min)
            min = n->valeur.annee_publication;
    }
    return min;
}

void lc_afficher(ListeChainee *lc) {
    if (!lc) return;

    const char *sep = "+------+--------------------------------------+-----------+-------------+";
    printf("\n=== Liste des ouvrages (liste chainee) ===\n");
    printf("%s\n", sep);
    printf("| %-4s | %-36s | %-9s | %-11s |\n", "ID", "Titre", "Annee", "Exemplaires");
    printf("%s\n", sep);

    for (Noeud *n = lc->tete; n; n = n->suivant) {
        printf("| %-4d | %-36.36s | %-9d | %-11d |\n",
               n->valeur.id_ouvrage,
               n->valeur.titre,
               n->valeur.annee_publication,
               n->valeur.nb_exemplaires_total);
    }

    printf("%s\n", sep);
}
