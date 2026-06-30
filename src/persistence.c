#include "persistence.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Persistance des ouvrages sur disque.
 * Ce module sauvegarde et recharge les structures statique et dynamique
 * depuis des fichiers binaires pour conserver les données entre exécutions.
 */

static int sauvegarder_tableau(const Ouvrage *data, size_t taille, const char *fichier) {
    FILE *fp = fopen(fichier, "wb");
    if (!fp) return -1;

    if (fwrite(&taille, sizeof(taille), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }

    if (taille > 0 && fwrite(data, sizeof(*data), taille, fp) != taille) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

static int charger_tableau(Ouvrage *data, size_t *taille, const char *fichier, size_t max_count) {
    FILE *fp = fopen(fichier, "rb");
    if (!fp) return -1;

    size_t count = 0;
    if (fread(&count, sizeof(count), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }

    if (count > max_count) {
        fclose(fp);
        return -1;
    }

    if (count > 0 && fread(data, sizeof(*data), count, fp) != count) {
        fclose(fp);
        return -1;
    }

    *taille = count;
    fclose(fp);
    return 0;
}

int sauvegarder_ouvrages(const TabStatique *ts, const char *fichier) {
    if (!ts || !fichier) return -1;
    return sauvegarder_tableau(ts->data, (size_t)ts->taille, fichier);
}

int charger_ouvrages(TabStatique *ts, const char *fichier) {
    if (!ts || !fichier) return -1;
    size_t count = 0;
    if (charger_tableau(ts->data, &count, fichier, MAX_STATIQUE) != 0) return -1;
    ts->taille = (int)count;
    return 0;
}

int sauvegarder_ouvrages_dyn(const TabDynamique *td, const char *fichier) {
    if (!td || !fichier) return -1;

    Ouvrage *buffer = NULL;
    if (td->taille > 0) {
        buffer = malloc((size_t)td->taille * sizeof(*buffer));
        if (!buffer) return -1;
        for (int i = 0; i < td->taille; i++)
            buffer[i] = *td->data[i];
    }

    int status = sauvegarder_tableau(buffer, (size_t)td->taille, fichier);
    free(buffer);
    return status;
}

int charger_ouvrages_dyn(TabDynamique *td, const char *fichier) {
    if (!td || !fichier) return -1;

    FILE *fp = fopen(fichier, "rb");
    if (!fp) return -1;

    size_t count = 0;
    if (fread(&count, sizeof(count), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }

    Ouvrage *buffer = NULL;
    if (count > 0) {
        buffer = malloc(count * sizeof(*buffer));
        if (!buffer) {
            fclose(fp);
            return -1;
        }
        if (fread(buffer, sizeof(*buffer), count, fp) != count) {
            free(buffer);
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);

    for (int i = 0; i < td->taille; i++)
        free(td->data[i]);
    td->taille = 0;

    if (count > (size_t)td->capacite) {
        Ouvrage **nouveau = realloc(td->data, count * sizeof(*nouveau));
        if (!nouveau) {
            free(buffer);
            return -1;
        }
        td->data = nouveau;
        td->capacite = (int)count;
    }

    for (size_t i = 0; i < count; i++) {
        Ouvrage *o = malloc(sizeof(*o));
        if (!o) {
            for (int j = 0; j < (int)i; j++) free(td->data[j]);
            free(buffer);
            return -1;
        }
        *o = buffer[i];
        td->data[td->taille++] = o;
    }

    free(buffer);
    return 0;
}

int sauvegarder_ouvrages_lc(const ListeChainee *lc, const char *fichier) {
    if (!lc || !fichier) return -1;

    Ouvrage *buffer = NULL;
    if (lc->taille > 0) {
        buffer = malloc((size_t)lc->taille * sizeof(*buffer));
        if (!buffer) return -1;

        int i = 0;
        for (Noeud *n = lc->tete; n; n = n->suivant)
            buffer[i++] = n->valeur;
    }

    int status = sauvegarder_tableau(buffer, (size_t)lc->taille, fichier);
    free(buffer);
    return status;
}

int charger_ouvrages_lc(ListeChainee *lc, const char *fichier) {
    if (!lc || !fichier) return -1;

    FILE *fp = fopen(fichier, "rb");
    if (!fp) return -1;

    size_t count = 0;
    if (fread(&count, sizeof(count), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }

    Ouvrage *buffer = NULL;
    if (count > 0) {
        buffer = malloc(count * sizeof(*buffer));
        if (!buffer) {
            fclose(fp);
            return -1;
        }
        if (fread(buffer, sizeof(*buffer), count, fp) != count) {
            free(buffer);
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);

    while (lc->tete) {
        Noeud *tmp = lc->tete;
        lc->tete = lc->tete->suivant;
        free(tmp);
    }
    lc->taille = 0;

    for (size_t i = 0; i < count; i++) {
        if (!lc_inserer(lc, buffer[i])) {
            free(buffer);
            return -1;
        }
    }

    free(buffer);
    return 0;
}
