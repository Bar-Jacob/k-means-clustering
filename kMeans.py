import sys

same_average = False
cnt = 0
k = sys.argv[1]

def argu_check(k,max_iter=200):
    if(('.' in k) or int(k) < 0):
        print("Invalid Input")
        assert()
    if(max_iter != 200):
        if(('.' in max_iter) or int(max_iter) < 0):
            print("Invalid Input")
            assert()
    return int(max_iter)

try:
    max_iter = sys.argv[2]
    max_iter = argu_check(k,max_iter)
except IndexError:
    max_iter = argu_check(k)

k = int(sys.argv[1])
clusters = [[] for i in range(k)]

data_points = []

line = input()
while (line != EOFError):
    data_points.append([float(xi) for xi in line.split(",")])
    try:   
        line = input()
    except EOFError:
        break

for i in range(k):
    clusters[i].append(list.copy(data_points[i])) 
    clusters[i].append(0)
    clusters[i].append([0.0 for xi in range(len(data_points[i]))])

def Euclidian_Distance(vector, centroid):
    sum = 0.0
    for xi in range(len(vector)):
        sum += pow((vector[xi]-centroid[xi]),2)
    return sum

def Update_Mean(clusters, same_average):
    for i in range(k):
        for j in range(len(clusters[i][2])):
            if(clusters[i][2][j]/clusters[i][1] != clusters[i][0][j]):
                same_average = False
                clusters[i][0][j] = clusters[i][2][j]/clusters[i][1]
    return same_average


def Update_Sum_Of_Elements_In_Cluster(float_vector, loc, clusters):
    for i in range(len(float_vector)):
        clusters[loc][2][i] += float_vector[i]

def Finding_Cluster(float_vector, clusters):
    min_distance, num_of_cluster = -1, -1
    for i in range(k):
        distance = Euclidian_Distance(float_vector, clusters[i][0])
        if((distance < min_distance) or (min_distance < 0)): #distance is smaller or we are doing the first calc of distance
            min_distance = distance
            num_of_cluster = i
    
    clusters[num_of_cluster][1] += 1 #update number of elements in cluster
    Update_Sum_Of_Elements_In_Cluster(float_vector, num_of_cluster, clusters)


    
while (cnt < max_iter) and (not same_average):
    same_average = True #will be changed to False if one of the means change
    for vector in data_points:
        Finding_Cluster(vector, clusters)
    same_average = Update_Mean(clusters, same_average) #check if there's no change in mu
    
    for cluster in clusters:
        cluster[1] = 0
        cluster[2] = [0.0 for x in cluster[2]] #initializing clusters properties before next iteration

    cnt += 1

for cluster in clusters:
    print(*["{:.4f}".format(num) for num in cluster[0]],sep=",")












