// kmeans.h

#ifndef KMEANS_H
#define KMEANS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define NB_POINTS 8
#define NB_CLUSTERS 3
#define CONVERGE_DISTANCE 0.0001

typedef struct s_centroide centroide;
typedef struct s_point {
    char name;
    float x;
    float y;
    centroide *centroide;
} point;

struct s_centroide {
    char name;
    float x;
    float y;
    struct s_point **list_points;
    int list_index;
};

// Déclaration des fonctions
void centroide_init(centroide *to_initialise);
void centroide_free(centroide *clusters);
void add_point_to_centroide(centroide *mon_centroide, point *mon_point);
void delete_point_from_centroide(centroide *mon_centroide);
void k_means(point points_before[], centroide clusters_before[]);
bool converged_centroide(centroide clusters_before[], centroide clusters_after[]);

#endif // KMEANS_H
