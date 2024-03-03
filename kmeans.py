"""
hw1 in python - shahar and tom , kmeans.
"""
import math
import sys

epsilon = 0.0001


class Clusters:
    def __init__(self, k):
        self.k = k
        self.centroid_list = []
        for i in range(k):
            self.centroid_list.append([])  # each list includes:
            # --> [curr Centroid vector, Prev centroid vector, list of vectors]

    def find_closest_cluster(self, x):
        """
        adds the vector x to the closest cluster and return the cluster's index in centroid_list
        :param x: vector
        :return: index of the closest cluster's location in centroid_list to x
        """
        min_distance = distance(x, self.centroid_list[0][0])
        index = 0
        for i in range(1, self.k):
            curr_distance = distance(x, self.centroid_list[i][0])
            if curr_distance < min_distance:
                min_distance = curr_distance
                index = i
        self.centroid_list[index].append(x)
        return index

    def update_centroid_list(self):
        """
        call calc centroid for all centorids in centorid_list
        :return:
        """
        for i in range(self.k):
            self.calc_centroid(i)

    def calc_centroid(self, index):
        """
        calculate and update the new centroid of a given cluster using a specific index
        :param index: index of the cluster in centroid_list
        :return: new centroid vector
        """
        new_centroid = []
        cluster = self.centroid_list[index]
        d = len(cluster[2])
        for i in range(d):
            sum_d = 0
            for j in range(2, len(cluster)):
                sum_d += cluster[j][i]
            coordinate = sum_d / (len(cluster) - 2)
            new_centroid.append(round(coordinate, 4))
        cluster[1] = cluster[0]
        cluster[0] = new_centroid
        return new_centroid

    def check_convergence(self):
        """
        check for all clusters if the distance between current centroid to previous one smaller than epsilon=0.001
        :return: True if all distances are smaller than epsilon
        """
        for i in range(self.k):
            dist = distance(self.centroid_list[i][0], self.centroid_list[i][1])
            if dist >= epsilon:
                return False
        return True

    def print_centroids(self):
        """
        print all the centroids from current centroid list
        :return: void --> printing all the centroids
        """
        for i in range(self.k):
            for j in range(len(self.centroid_list[i][0])):
                if j < len(self.centroid_list[i][0]) -1:
                    print('%.4f,' % self.centroid_list[i][0][j], end='')
                else:
                    print('%.4f' % self.centroid_list[i][0][j], end='')
            print("")

    def delete_vectors(self):
        """
        delete all the vectors from the list
        :return: void --> deleting all the vectors
        """
        for i in range(self.k):
            lst = self.centroid_list[i]
            del lst[2:]


def line_to_vector(line):
    """
    :param: line: string representation of x
    :return: list of floats representing the vector of x
    """
    x = line.split(",")
    for i in range(len(x)):
        x[i] = float(x[i])
    return x


def distance(x, y):
    """
    calculates the Euclidean distance between x and y vectors
    :param x: list of floats representing x vector
    :param y: list of floats representing y vector
    :return: distance as a float
    """
    sum = 0
    for i in range(len(x)):
        sum += (x[i] - y[i]) ** 2
    return sum ** 0.5



def kmean_flow(k, file_name):
    with open(file_name, 'r') as file:
        error_cnt = 0
        vector_list = file.readlines()
        line_count = len(vector_list)

    # initialize data structures --> UNION-FIND
    itr = 300
    clusters = Clusters(k)

    # set default centroids --> first k vectors in vector list
    for i in range(k):
        x = line_to_vector(vector_list[i])
        clusters.centroid_list[i].extend([x, x])  # initialize curr=x, Prev=x, vector_list = [x]

    assigned_to_cluster = [-1 for _ in range(line_count)]

    # iterate over vectors
    cnt = 0
    while cnt < itr:
        for j, vector in enumerate(vector_list):
            x = line_to_vector(vector)
            idx = clusters.find_closest_cluster(x)
            assigned_to_cluster[j] = idx
        cnt += 1
        clusters.update_centroid_list()
        if clusters.check_convergence():
            break
        clusters.delete_vectors()


    return assigned_to_cluster


