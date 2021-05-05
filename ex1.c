#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct 
{
    int num_of_points;
    double* sum_of_points;
    double* centroid;
}Cluster;


double Euclidian_Distance(double* vector1, double* vector2, int dimension);
void finding_cluster(double* vector, Cluster* clusters, int k, int dimension);
int update_mean(Cluster* clusters, int same_average, int k, int dimension);
void update_sum_of_elements_in_cluster(double* vector, int loc, Cluster* clusters, int dimension);
void free_memory(Cluster* clusters, double** data_points, int k, int num_of_points);

void main(int argc, char *argv[]){
    int max_iter = 0;
    int k = 0;
    Cluster* clusters;
    double** data_points;
    int num_of_points = 0;
    int dimension = 1;
    int same_average = 0;
    FILE *fp;
    char vector[1000];
    char *token;
    int cnt = 0;

    if(argc == 1){
        assert("Missing arguments");
    }

    if(atoi(argv[1]) < 0 || strchr(argv[1], '.') != NULL){
        printf("Invalid Input");
        assert(NULL);
    }
    k = atoi(argv[1]);
    printf("k: %d\n", k);

    if(argc == 2){ 
        max_iter = 200;
    }else if(atoi(argv[2]) < 0 || strchr(argv[2], '.') != NULL){
        printf("Invalid Input");
        assert(NULL);
    }else{
        max_iter = atoi(argv[2]);
    }
    
    printf("max iter: %d\n", max_iter);


    /* opening file for reading */
    scanf("%s", vector);
    fp = fopen(vector, "r");
    while (!feof(fp))
    {
        fgets(vector, 1000, fp);
        if(vector == NULL){
            /*line is empty*/
            break;
        }
        if(num_of_points == 0){
            for(int i = 0; vector[i] != '\n'; ++i){
                 if(vector[i] == ','){
                     dimension++;
                     }
                }
        }
        num_of_points += 1;
    }
    printf("num: %d\n", num_of_points);
    printf("dim: %d\n", dimension);

    data_points = calloc(num_of_points,sizeof(double*));
    for(int i = 0; i < num_of_points; i++){
        data_points[i] = calloc(dimension,sizeof(double));
    }

    rewind(fp);
    while (!feof(fp)){
        fgets(vector, 1000, fp);
        token = strtok(vector, ",");
        
        for(int i = 0; i < dimension; i++){
            if(token != NULL){
                data_points[cnt][i] = atof(token);
                token = strtok(NULL, ",");
            }
        }
        cnt++;
    }

    fclose(fp);

    /*
    Initializing k clusters
    */
    clusters = (Cluster*)calloc(k,sizeof(Cluster));
    for(int i = 0; i < k; i++){
        clusters[i].centroid = (double*)calloc(dimension, sizeof(double));
        assert(clusters[i].centroid != NULL);
        clusters[i].centroid = data_points[i]; //will be equal to the i'th vector
        clusters[i].num_of_points = 0;
        clusters[i].sum_of_points = (double*)calloc(dimension, sizeof(double));
        assert(clusters[i].sum_of_points != NULL);
        clusters[i].sum_of_points[i] = 0;
    }

    cnt = 0;
    while ((cnt < max_iter) && (!same_average))
    {
        same_average = 1;
        for(int i = 0; i < num_of_points; i++){
            finding_cluster(data_points[i], clusters, k, dimension);
        }
        
        same_average = update_mean(clusters, same_average, k, dimension);
        if(same_average == 1){
            break;
        }

        for(int i = 0; i < k; i++){
            clusters[i].num_of_points = 0;
            for(int j = 0; j < dimension; j++){
                clusters[i].sum_of_points[j] = 0;
            }
        }
        cnt++;
    }
    for(int i = 0; i < k; i++){
        for(int j = 0; j < dimension; j++){
            if(j == dimension-1){
                printf("%.4lf", clusters[i].centroid[j]);
            }else{
                printf("%.4lf,", clusters[i].centroid[j]);
            }
        }
        printf("\n");
    }
    free_memory(clusters, data_points, k, num_of_points);
}


void finding_cluster(double* vector, Cluster* clusters, int k, int dimension){
    double min_distance = -1.0;
    int num_of_cluster = -1;
    double distance;

    for(int i = 0; i < k; i++){
        distance = Euclidian_Distance(vector, clusters[i].centroid, dimension);
        if((distance < min_distance) || (min_distance < 0)){
            min_distance = distance;
            num_of_cluster = i;
        }
    }
    clusters[num_of_cluster].num_of_points++;
    update_sum_of_elements_in_cluster(vector, num_of_cluster, clusters, dimension);
}

void update_sum_of_elements_in_cluster(double* vector, int loc, Cluster* clusters, int dimension){
    for(int i = 0; i < dimension; i++){
        clusters[loc].sum_of_points[i] += vector[i];
    }
}


int update_mean(Cluster* clusters, int same_average, int k, int dimension){
    for(int i = 0; i < k; i++){
        for(int j = 0; j < dimension; j++){
            if((clusters[i].sum_of_points[j]/clusters[i].num_of_points)!=
                clusters[i].centroid[j]){
                    same_average = 0;
                    clusters[i].centroid[j] = clusters[i].sum_of_points[j]/clusters[i].num_of_points;
                }
        }
    }
    return same_average;
}

double Euclidian_Distance(double* vector1, double* centroid, int dimension){
    double sum = 0.0;
    for(int xi = 0; xi < dimension; xi++){
        sum += (vector1[xi]-centroid[xi])*(vector1[xi]-centroid[xi]);
    } 
    return sum;
}

void free_memory(Cluster* clusters, double** data_points, int k, int num_of_points){
    for(int i = 0; i < num_of_points; i++){
        free(data_points[i]);
    }
    free(data_points);

    for(int j = 0; j < k; j++){
        free(clusters[j].centroid);
        free(clusters[j].sum_of_points);
    }
    free(clusters);
}