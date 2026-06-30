#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "structure.h"
#include "tableau_statique.h"
#include "tableau_dynamique.h"
#include "liste_chainee.h"
#include "utils.h"

/*
 * Programme de benchmark des structures de données.
 * Il mesure les performances d'insertion, recherche et tri
 * pour les versions statique et dynamique.
 */

#define REPEATS 10
#define OUTPUT_FILE "data/benchmark.csv"

static double now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000000000.0 + (double)ts.tv_nsec;
}

static Ouvrage make_ouvrage(int id, int year) {
    Ouvrage o = ouvrage_aleatoire(id);
    o.annee_publication = year;
    return o;
}

static void fill_uniform(Ouvrage *data, int n) {
    for (int i = 0; i < n; i++) {
        data[i] = make_ouvrage(i + 1, 1900 + rand() % 126);
    }
}

static void fill_gaussian(Ouvrage *data, int n) {
    for (int i = 0; i < n; i++) {
        double x = 0.0;
        for (int k = 0; k < 12; k++) {
            x += (double)(rand() % 100);
        }
        int year = (int)(2010.0 + (x / 12.0 - 50.0) * 0.8);
        if (year < 1900) year = 1900;
        if (year > 2025) year = 2025;
        data[i] = make_ouvrage(i + 1, year);
    }
}

static void fill_sorted(Ouvrage *data, int n) {
    for (int i = 0; i < n; i++) {
        data[i] = make_ouvrage(i + 1, 1900 + (i % 100));
    }
}

static void fill_profile(Ouvrage *data, int n, const char *profile) {
    if (strcmp(profile, "gaussienne") == 0) {
        fill_gaussian(data, n);
    } else if (strcmp(profile, "tri") == 0) {
        fill_sorted(data, n);
    } else {
        fill_uniform(data, n);
    }
}

static void write_csv(const char *op, const char *structure, const char *profile,
                      int n, double mean_ms, double std_ms) {
    FILE *fp = fopen(OUTPUT_FILE, "a");
    if (!fp) return;
    fprintf(fp, "%s,%s,%s,%d,%.6f,%.6f\n", op, structure, profile, n, mean_ms, std_ms);
    fclose(fp);
}

static double mean_ms(double *vals, int count) {
    double s = 0.0;
    for (int i = 0; i < count; i++) s += vals[i];
    return s / count;
}

static double std_ms(double *vals, int count) {
    double m = mean_ms(vals, count);
    double v = 0.0;
    for (int i = 0; i < count; i++) {
        double d = vals[i] - m;
        v += d * d;
    }
    return sqrt(v / count);
}

static void measure_insertion_static(int n, const char *profile) {
    double vals[REPEATS];
    Ouvrage *data = malloc((size_t)n * sizeof(*data));
    if (!data) return;
    fill_profile(data, n, profile);

    for (int r = 0; r < REPEATS; r++) {
        TabStatique *ts = ts_creer();
        double t0 = now_ns();
        for (int i = 0; i < n; i++) ts_inserer(ts, data[i]);
        double t1 = now_ns();
        vals[r] = (t1 - t0) / 1000000.0;
        ts_detruire(ts);
    }

    write_csv("insertion", "statique", profile, n, mean_ms(vals, REPEATS), std_ms(vals, REPEATS));
    free(data);
}

static void measure_insertion_dynamic(int n, const char *profile) {
    double vals[REPEATS];
    Ouvrage *data = malloc((size_t)n * sizeof(*data));
    if (!data) return;
    fill_profile(data, n, profile);

    for (int r = 0; r < REPEATS; r++) {
        TabDynamique *td = td_creer();
        double t0 = now_ns();
        for (int i = 0; i < n; i++) {
            Ouvrage *o = malloc(sizeof(*o));
            *o = data[i];
            td_inserer(td, o);
        }
        double t1 = now_ns();
        vals[r] = (t1 - t0) / 1000000.0;
        td_detruire(td);
    }

    write_csv("insertion", "dynamique", profile, n, mean_ms(vals, REPEATS), std_ms(vals, REPEATS));
    free(data);
}

static void measure_insertion_linked(int n, const char *profile) {
    double vals[REPEATS];
    Ouvrage *data = malloc((size_t)n * sizeof(*data));
    if (!data) return;
    fill_profile(data, n, profile);

    for (int r = 0; r < REPEATS; r++) {
        ListeChainee *lc = lc_creer();
        double t0 = now_ns();
        for (int i = 0; i < n; i++) {
            if (!lc_inserer(lc, data[i])) {
                break;
            }
        }
        double t1 = now_ns();
        vals[r] = (t1 - t0) / 1000000.0;
        lc_detruire(lc);
    }

    write_csv("insertion", "liste_chainee", profile, n, mean_ms(vals, REPEATS), std_ms(vals, REPEATS));
    free(data);
}

static void measure_search_static(int n, const char *profile) {
    double vals[REPEATS];
    Ouvrage *data = malloc((size_t)n * sizeof(*data));
    if (!data) return;
    fill_profile(data, n, profile);

    TabStatique *ts = ts_creer();
    for (int i = 0; i < n; i++) ts_inserer(ts, data[i]);

    for (int r = 0; r < REPEATS; r++) {
        double t0 = now_ns();
        volatile Ouvrage *tmp = ts_rechercher_id(ts, data[r % n].id_ouvrage);
        (void)tmp;
        double t1 = now_ns();
        vals[r] = (t1 - t0) / 1000000.0;
    }

    write_csv("recherche", "statique", profile, n, mean_ms(vals, REPEATS), std_ms(vals, REPEATS));
    ts_detruire(ts);
    free(data);
}

static void measure_search_dynamic(int n, const char *profile) {
    double vals[REPEATS];
    Ouvrage *data = malloc((size_t)n * sizeof(*data));
    if (!data) return;
    fill_profile(data, n, profile);

    TabDynamique *td = td_creer();
    for (int i = 0; i < n; i++) {
        Ouvrage *o = malloc(sizeof(*o));
        *o = data[i];
        td_inserer(td, o);
    }

    for (int r = 0; r < REPEATS; r++) {
        double t0 = now_ns();
        volatile Ouvrage *tmp = td_rechercher_id(td, data[r % n].id_ouvrage);
        (void)tmp;
        double t1 = now_ns();
        vals[r] = (t1 - t0) / 1000000.0;
    }

    write_csv("recherche", "dynamique", profile, n, mean_ms(vals, REPEATS), std_ms(vals, REPEATS));
    td_detruire(td);
    free(data);
}

static void measure_search_linked(int n, const char *profile) {
    double vals[REPEATS];
    Ouvrage *data = malloc((size_t)n * sizeof(*data));
    if (!data) return;
    fill_profile(data, n, profile);

    ListeChainee *lc = lc_creer();
    for (int i = 0; i < n; i++) {
        lc_inserer(lc, data[i]);
    }

    for (int r = 0; r < REPEATS; r++) {
        double t0 = now_ns();
        volatile Ouvrage *tmp = lc_rechercher_id(lc, data[r % n].id_ouvrage);
        (void)tmp;
        double t1 = now_ns();
        vals[r] = (t1 - t0) / 1000000.0;
    }

    write_csv("recherche", "liste_chainee", profile, n, mean_ms(vals, REPEATS), std_ms(vals, REPEATS));
    lc_detruire(lc);
    free(data);
}

static void measure_tri_static(int n, const char *profile) {
    double vals[REPEATS];
    Ouvrage *data = malloc((size_t)n * sizeof(*data));
    if (!data) return;
    fill_profile(data, n, profile);

    for (int r = 0; r < REPEATS; r++) {
        TabStatique *ts = ts_creer();
        for (int i = 0; i < n; i++) ts_inserer(ts, data[i]);
        double t0 = now_ns();
        ts_tri_insertion(ts);
        double t1 = now_ns();
        vals[r] = (t1 - t0) / 1000000.0;
        ts_detruire(ts);
    }

    write_csv("tri", "statique", profile, n, mean_ms(vals, REPEATS), std_ms(vals, REPEATS));
    free(data);
}

static void measure_tri_dynamic(int n, const char *profile) {
    double vals[REPEATS];
    Ouvrage *data = malloc((size_t)n * sizeof(*data));
    if (!data) return;
    fill_profile(data, n, profile);

    for (int r = 0; r < REPEATS; r++) {
        TabDynamique *td = td_creer();
        for (int i = 0; i < n; i++) {
            Ouvrage *o = malloc(sizeof(*o));
            *o = data[i];
            td_inserer(td, o);
        }
        double t0 = now_ns();
        td_tri_insertion(td);
        double t1 = now_ns();
        vals[r] = (t1 - t0) / 1000000.0;
        td_detruire(td);
    }

    write_csv("tri", "dynamique", profile, n, mean_ms(vals, REPEATS), std_ms(vals, REPEATS));
    free(data);
}

static void measure_tri_linked(int n, const char *profile) {
    double vals[REPEATS];
    Ouvrage *data = malloc((size_t)n * sizeof(*data));
    if (!data) return;
    fill_profile(data, n, profile);

    for (int r = 0; r < REPEATS; r++) {
        ListeChainee *lc = lc_creer();
        for (int i = 0; i < n; i++) {
            lc_inserer(lc, data[i]);
        }
        double t0 = now_ns();
        lc_tri_insertion(lc);
        double t1 = now_ns();
        vals[r] = (t1 - t0) / 1000000.0;
        lc_detruire(lc);
    }

    write_csv("tri", "liste_chainee", profile, n, mean_ms(vals, REPEATS), std_ms(vals, REPEATS));
    free(data);
}

int main(void) {
    srand((unsigned)time(NULL));
    remove(OUTPUT_FILE);
    FILE *fp = fopen(OUTPUT_FILE, "w");
    if (!fp) return 1;
    fprintf(fp, "operation,structure,profile,n,mean_ms,std_ms\n");
    fclose(fp);

    const int sizes[] = {100, 1000, 10000, 100000};
    const char *profiles[] = {"uniforme", "gaussienne", "tri"};

    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        int n = sizes[i];
        for (size_t p = 0; p < sizeof(profiles) / sizeof(profiles[0]); p++) {
            const char *profile = profiles[p];
            measure_insertion_static(n, profile);
            measure_insertion_dynamic(n, profile);
            measure_insertion_linked(n, profile);
            measure_search_static(n, profile);
            measure_search_dynamic(n, profile);
            measure_search_linked(n, profile);
            measure_tri_static(n, profile);
            measure_tri_dynamic(n, profile);
            measure_tri_linked(n, profile);
        }
    }

    printf("Benchmark termine : donnees enregistrees dans %s\n", OUTPUT_FILE);
    return 0;
}
