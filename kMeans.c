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
    char c;
    double value;
    int num_of_points = 0;
    int dimension = 1;
    FILE *fstdin = stdin;
    char vector[1000];
    char *token;

    
    int max_iter = 0;
    int k = 0;
    Cluster* clusters;
    double** data_points;
    int same_average = 0;
    int cnt = 0;

    printf("%d", argc);
    // assert((argc == 3) && "Missing arguments");
    assert(atoi(argv[1]) > 0 && "Negative Input");
    assert(strchr(argv[1], '.') == NULL && "Invalid Input");
    
    k = atoi(argv[1]);
    printf("k: %d\n", k);

    if(argc == 2){ 
        max_iter = 200;
    }else{
        assert(atoi(argv[2]) > 0 && "Invalid Input");
        assert(strchr(argv[2], '.') == NULL && "Invalid Input");
        max_iter = atoi(argv[2]);
    }
    printf("max iter: %d\n", max_iter);


    /* finding number of points and dimension from the input */
    while(scanf("%lf%c", &value, &c) == 2){
        if((c==',') && num_of_points == 0){
            dimension++;
        }
        if(c == '\n'){
            num_of_points++;
        }
    }

    printf("num: %d\n", num_of_points);
    printf("dim: %d\n", dimension);

    data_points = calloc(num_of_points,sizeof(double*));
    printf("after calloc\n");
    for(int i = 0; i < num_of_points; i++){
        data_points[i] = calloc(dimension,sizeof(double));
    }
    printf("befor rewind\n");
    
    rewind(stdin);
    while(scanf("%lf%c", &value, &c) == 2){
        for(int i = 0; i < dimension-1; i++){
            data_points[cnt][i] = value;
            scanf("%lf%c", &value, &c);
        }
        cnt++;
    }
    // fread(fstdin, vector, 1000);
    // for(int i = 0; i < num_of_points; i++){
    //     for(int j = 0; j < dimension; j++){
    //         scanf("%lf%c", &value, &c);
    //         data_points[i][j] = value;
    //     }
    // }
    printf("after rewind\n");

    /*
    Initializing k clusters
    */
    printf("im here");
    clusters = (Cluster*)calloc(k,sizeof(Cluster));
    for(int i = 0; i < k; i++){
        clusters[i].centroid = (double*)calloc(dimension, sizeof(double));
        assert(clusters[i].centroid != NULL);
        memcpy(clusters[i].centroid, data_points[i], sizeof(data_points[i])); //will be equal to the i'th vector
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