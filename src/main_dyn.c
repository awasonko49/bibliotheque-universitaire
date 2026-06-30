#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "structure.h"
#include "utils.h"
#include "tableau_dynamique.h"
#include "persistence.h"

/*
 * Programme principal de la version dynamique de la bibliothèque.
 * Il fournit une interface interactive pour manipuler les ouvrages
 * stockés dans un tableau dynamique.
 */

#define FICHIER_DATA "ouvrages_dyn.dat"

static void afficher_menu(void) {
    printf("\n=== Bibliotheque UIDT (Dynamique) ===\n");
    printf(" 1. Generer des ouvrages\n");
    printf(" 2. Afficher tous les ouvrages\n");
    printf(" 3. Rechercher par ID\n");
    printf(" 4. Rechercher par titre (prefixe)\n");
    printf(" 5. Supprimer un ouvrage\n");
    printf(" 6. Trier par titre (insertion)\n");
    printf(" 7. Trier par annee (rapide)\n");
    printf(" 8. Statistiques\n");
    printf(" 9. Sauvegarder\n");
    printf("10. Charger depuis fichier\n");
    printf("11. Rechercher par intervalle d'annee\n");
    printf("12. Mettre a jour un ouvrage\n");
    printf(" 0. Quitter\n");
    printf("Choix : ");
}

int main(void) {
    srand((unsigned)time(NULL));

    TabDynamique *td = td_creer();
    if (!td) {
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
            printf("Nombre d'ouvrages a generer : ");
            scanf("%d", &n);

            for (int i = 0; i < td->taille; i++)
                free(td->data[i]);
            td->taille = 0;

            for (int i = 0; i < n; i++) {
                Ouvrage *o = malloc(sizeof(*o));
                if (!o) {
                    fprintf(stderr, "Erreur d'allocation\n");
                    break;
                }
                *o = ouvrage_aleatoire(i + 1);
                if (!td_inserer(td, o)) {
                    free(o);
                    break;
                }
            }
            printf("%d ouvrage(s) g\u00e9n\u00e9r\u00e9(s).\n", n);
            break;
        }

        case 2:
            td_afficher(td);
            break;

        case 3: {
            int id;
            printf("ID a chercher : ");
            scanf("%d", &id);
            Ouvrage *o = td_rechercher_id(td, id);
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
            Ouvrage *o = td_rechercher_titre(td, prefixe);
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
            if (td_supprimer(td, id)) 
                printf("Supprime.\n");
            else                           
                printf("ID %d non trouve.\n", id);
            break;
        }

        case 6:
            td_tri_insertion(td);
            printf("Trie par titre.\n");
            break;

        case 7:
            if (td->taille > 1)
                td_tri_rapide(td, 0, td->taille - 1);
            printf("Trie par annee.\n");
            break;

        case 8:
            if (td->taille == 0) {
                printf("\nAucun ouvrage disponible pour les statistiques.\n");
                break;
            }
            printf("\n=== Statistiques (tableau dynamique) ===\n");
            printf("Nombre d'ouvrages : %d\n", td->taille);
            printf("Annee moyenne     : %.1f\n", td_moyenne_annee(td));
            printf("Annee minimale    : %d\n", td_min_annee(td));
            printf("Max exemplaires   : %d\n", td_max_exemplaires(td));
            break;


        case 9:
            sauvegarder_ouvrages_dyn(td, FICHIER_DATA);
            break;

        case 10:
            charger_ouvrages_dyn(td, FICHIER_DATA);
            break;

        case 11: {
            int min, max, count = 0;
            printf("Annee minimum : ");
            scanf("%d", &min);
            printf("Annee maximum : ");
            scanf("%d", &max);
            Ouvrage **resultats = td_rechercher_intervalle_annee(td, min, max, &count);
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
            Ouvrage *actuel = td_rechercher_id(td, id);
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
            if (td_maj_ouvrage(td, id, &nouveau))
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

    td_detruire(td);
    return 0;
}
