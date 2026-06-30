#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "structure.h"
#include "utils.h"
#include "liste_chainee.h"
#include "persistence.h"

/*
 * Programme principal de la version liste chainee.
 * Il fournit une interface interactive pour manipuler les ouvrages.
 */

#define FICHIER_DATA "ouvrages_lc.dat"

static void afficher_menu(void) {
    printf("\n=== Bibliotheque UIDT (Liste chainee) ===\n");
    printf(" 1. Générer des ouvrages\n");
    printf(" 2. Afficher tous les ouvrages\n");
    printf(" 3. Rechercher par ID\n");
    printf(" 4. Rechercher par titre (préfixe)\n");
    printf(" 5. Supprimer un ouvrage\n");
    printf(" 6. Trier par titre (insertion)\n");
    printf(" 7. Trier par année (rapide)\n");
    printf(" 8. Statistiques\n");
    printf(" 9. Sauvegarder\n");
    printf("10. Charger depuis fichier\n");
    printf("11. Rechercher par intervalle d'année\n");
    printf("12. Mettre a jour un ouvrage\n");
    printf(" 0. Quitter\n");
    printf("Choix : ");
}

int main(void) {
    srand((unsigned)time(NULL));

    ListeChainee *lc = lc_creer();
    if (!lc) {
        fprintf(stderr, "Erreur d'allocation\n");
        return 1;
    }

    int choix;
    do {
        afficher_menu();
        if (scanf("%d", &choix) != 1) break;

        switch (choix) {

        case 1: {
            int n;
            printf("Nombre d'ouvrages a générer : ");
            scanf("%d", &n);

            while (lc->tete) {
                Noeud *tmp = lc->tete;
                lc->tete = lc->tete->suivant;
                free(tmp);
            }
            lc->taille = 0;

            for (int i = 0; i < n; i++) {
                Ouvrage o = ouvrage_aleatoire(i + 1);
                if (!lc_inserer(lc, o)) {
                    fprintf(stderr, "Erreur lors de l'insertion\n");
                    break;
                }
            }
            printf("%d ouvrage(s) généré(s).\n", n);
            break;
        }

        case 2:
            lc_afficher(lc);
            break;

        case 3: {
            int id;
            printf("ID a chercher : ");
            scanf("%d", &id);
            Ouvrage *o = lc_rechercher_id(lc, id);
            if (o) {
                printf("\n=== Resultat de recherche (ID %d) ===\n", id);
                ouvrage_afficher(o);
            } else {
                printf("\nAucun ouvrage trouve avec l'ID %d.\n", id);
            }
            break;
        }

        case 4: {
            char prefixe[TITRE_LEN];
            printf("Debut du titre : ");
            scanf(" %99[^\n]", prefixe);
            Ouvrage *o = lc_rechercher_titre(lc, prefixe);
            if (o) {
                printf("\n=== Resultat de recherche (titre) ===\n");
                ouvrage_afficher(o);
            } else {
                printf("\nAucun titre ne commence par \"%s\".\n", prefixe);
            }
            break;
        }

        case 5: {
            int id;
            printf("ID a supprimer : ");
            scanf("%d", &id);
            if (lc_supprimer(lc, id))
                printf("Ouvrage avec ID %d supprime avec succes.\n", id);
            else
                printf("Aucun ouvrage trouve avec l'ID %d.\n", id);
            break;
        }

        case 6:
            lc_tri_insertion(lc);
            printf("Trie par titre.\n");
            break;

        case 7:
            lc_tri_rapide(lc, 0, lc->taille - 1);
            printf("Trie par année.\n");
            break;

        case 8:
            if (lc->taille == 0) {
                printf("\nAucun ouvrage disponible pour les statistiques.\n");
                break;
            }
            printf("\n=== Statistiques (liste chainee) ===\n");
            printf("Nombre d'ouvrages : %d\n", lc->taille);
            printf("Annee moyenne     : %.1f\n", lc_moyenne_annee(lc));
            printf("Annee minimale    : %d\n", lc_min_annee(lc));
            printf("Max exemplaires   : %d\n", lc_max_exemplaires(lc));
            break;

        case 9:
            sauvegarder_ouvrages_lc(lc, FICHIER_DATA);
            break;

        case 10:
            charger_ouvrages_lc(lc, FICHIER_DATA);
            break;

        case 11: {
            int min, max, count = 0;
            printf("Annee minimum : ");
            scanf("%d", &min);
            printf("Annee maximum : ");
            scanf("%d", &max);
            Ouvrage **resultats = lc_rechercher_intervalle_annee(lc, min, max, &count);
            if (!resultats || count == 0) {
                printf("Aucun ouvrage trouve dans l'intervalle [%d, %d].\n", min, max);
                break;
            }
            printf("\n=== Resultats (%d ouvrage(s)) ===\n", count);
            for (int i = 0; i < count; i++)
                ouvrage_afficher(resultats[i]);
            free(resultats);
            break;
        }

        case 12: {
            int id;
            printf("ID a modifier : ");
            scanf("%d", &id);
            Ouvrage *actuel = lc_rechercher_id(lc, id);
            if (!actuel) {
                printf("Aucun ouvrage trouve avec l'ID %d.\n", id);
                break;
            }
            Ouvrage nouveau = *actuel;
            printf("Nouveau titre : ");
            scanf(" %99[^\n]", nouveau.titre);
            printf("Nouvel auteur : ");
            scanf(" %79[^\n]", nouveau.auteur);
            printf("Nouvel ISBN : ");
            scanf(" %19[^\n]", nouveau.isbn);
            printf("Nouvelle annee : ");
            scanf("%d", &nouveau.annee_publication);
            printf("Nouveau genre : ");
            scanf(" %39[^\n]", nouveau.genre);
            printf("Nouveaux exemplaires total : ");
            scanf("%d", &nouveau.nb_exemplaires_total);
            printf("Nouveaux exemplaires disponibles : ");
            scanf("%d", &nouveau.nb_exemplaires_dispo);
            if (lc_maj_ouvrage(lc, id, nouveau))
                printf("Ouvrage mis a jour avec succes.\n");
            else
                printf("Echec de la mise a jour.\n");
            break;
        }

        case 0:
            printf("Au revoir.\n");
            break;

        default:
            printf("Choix invalide.\n");
        }

    } while (choix != 0);

    lc_detruire(lc);
    return 0;
}
