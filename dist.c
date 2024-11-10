#include "kmeans.h"

void centroide_init(centroide *to_initialise)
{
    if(((to_initialise->list_points) = malloc((sizeof (point *) * NB_POINTS))) == NULL)
    {
        fprintf(stderr, "Error allocating memory clusters[]");
        exit (99);
    }
    to_initialise->list_index = 0;
}

void centroide_free(centroide *clusters)
{
    for(int i=0; i<NB_CLUSTERS; i++)
    {
        free(clusters[i].list_points);
    }
}

void add_point_to_centroide(centroide *mon_centroide, point *mon_point) {
    if (mon_centroide->list_index < NB_POINTS) {
        mon_centroide->list_points[mon_centroide->list_index] = mon_point;
        mon_centroide->list_points[mon_centroide->list_index]->name = mon_point->name;
        mon_centroide->list_index++;
        mon_point->centroide = mon_centroide;
    }
}

void delete_point_from_centroide(centroide *mon_centroide)
{
    if(mon_centroide->list_index>0 && mon_centroide->list_points[mon_centroide->list_index-1]->centroide != NULL)
    {
        mon_centroide->list_points[mon_centroide->list_index-1]->centroide=NULL;
        mon_centroide->list_points[mon_centroide->list_index-1] = NULL;
        mon_centroide->list_index--;
    }
}

void k_means(point points_before[], centroide clusters_before[])
{
    for (int i = 0; i < NB_CLUSTERS; i++) {
        clusters_before[i].list_index = 0;  // Réinitialiser les indices
    }

    for(int i=0; i<NB_POINTS; i++)
    {
        float min_dist = pow((points_before[i].x - clusters_before[0].x),2) + pow((points_before[i].y - clusters_before[0].y),2);
        add_point_to_centroide(&clusters_before[0], &points_before[i]);

        for(int j=1; j<NB_CLUSTERS; j++)
        {            
            float dist = pow((points_before[i].x - clusters_before[j].x),2) + pow((points_before[i].y - clusters_before[j].y),2);
            if (dist < min_dist)
            {
                min_dist = dist;
                delete_point_from_centroide(points_before[i].centroide);
                add_point_to_centroide(&clusters_before[j], &points_before[i]);
            }
        }
        //printf("pour i = %d, X->list_points[0] = %c\n", i, clusters_before[0].list_points[0]->name);
    }

    for(int i=0; i<NB_CLUSTERS; i++)
    {
        float x_mean=0;
        float y_mean=0;
        int nb_points_clust = clusters_before[i].list_index;
        for(int j=0; j<nb_points_clust; j++)
        {
            x_mean += clusters_before[i].list_points[j]->x;
            y_mean += clusters_before[i].list_points[j]->y;
        }
        clusters_before[i].x = x_mean/(float)nb_points_clust;
        clusters_before[i].y = y_mean/(float)nb_points_clust;
    }
}

bool converged_centroide(centroide clusters_before[], centroide clusters_after[])
{
    for(int i=0; i<NB_CLUSTERS; i++)
    {
        float distance = sqrt(pow(clusters_before[i].x - clusters_after[i].x, 2) + pow(clusters_before[i].y - clusters_after[i].y, 2));
        if(distance > CONVERGE_DISTANCE)
            return false;
    }
    return true;
}

void print_clusters(centroide *clusters)
{
    for(int i=0; i<NB_CLUSTERS; i++)
    {
        //printf("X->list_points[0] = %c\n", clusters_before[0].list_points[0]->name);
        printf("cluster %c : (%f,%f) | Points : ", clusters[i].name, clusters[i].x, clusters[i].y);
        for(int j=0; j<clusters[i].list_index; j++)
        {
            printf("%c ", clusters[i].list_points[j]->name);
        }
        printf("\n");
    }
}

int main(void)
{
    point points_before[NB_POINTS];
    centroide clusters_before[NB_CLUSTERS];
    centroide clusters_after[NB_CLUSTERS];

    for(int i=0; i<NB_CLUSTERS; i++)
    {
        centroide_init(&clusters_before[i]);
        centroide_init(&clusters_after[i]);
    }

    points_before[0].name = 'A'; points_before[0].x = 2; points_before[0].y = 10;
    points_before[1].name = 'B'; points_before[1].x = 2; points_before[1].y = 8;
    points_before[2].name = 'C'; points_before[2].x = 8; points_before[2].y = 4;
    points_before[3].name = 'D'; points_before[3].x = 5; points_before[3].y = 8;
    points_before[4].name = 'E'; points_before[4].x = 7; points_before[4].y = 5;
    points_before[5].name = 'F'; points_before[5].x = 6; points_before[5].y = 4;
    points_before[6].name = 'G'; points_before[6].x = 1; points_before[6].y = 2;
    points_before[7].name = 'H'; points_before[7].x = 4; points_before[7].y = 9;
    clusters_before[0].name = 'X'; clusters_before[0].x = 1; clusters_before[0].y = 9;
    clusters_before[1].name = 'Y'; clusters_before[1].x = 3; clusters_before[1].y = 8;
    clusters_before[2].name = 'Z'; clusters_before[2].x = 7; clusters_before[2].y = 3;
    // expected X: G | Y: A, B, D, H | Z: C, E, F
    
    do {
        for(int i = 0; i < NB_CLUSTERS; i++) {
            clusters_after[i].x = clusters_before[i].x;
            clusters_after[i].y = clusters_before[i].y;
        }
        k_means(points_before, clusters_before);
    } while (!converged_centroide(clusters_before, clusters_after));

    print_clusters(clusters_before);

    centroide_free(clusters_after);
    centroide_free(clusters_before);
   
    return 0;
}
