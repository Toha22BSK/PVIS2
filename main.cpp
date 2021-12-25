#include <iostream>
#include "pch.h"


struct baseMap
{
    int d1[3];
    int s1[3];
    int d2[3];
    int s2[3];
};

std::vector<int> createMatrix(int d, int s)
{
    std::vector<int> matrix(d * s);

    for (int i = 0; i < matrix.size(); ++i)
    {
        matrix[i] = 1 + std::rand() / ((RAND_MAX + 1u) / 99);
    }

    return matrix;
}

std::vector<int> changeMatrix(std::vector<int> matrix, int g, int sizeMatrix)
{
    int size = pow(2, sizeMatrix + g);
    int sizeDif = pow(2, g);
    int widthMatrix = pow(2, sizeMatrix);
    int index = 0;
    std::vector<int> newMatrix(size * size);
    for (int i = 0; i < matrix.size(); ++i)
    {
        for (int j = 0; j < sizeDif * sizeDif; ++j)
        {
            index = sizeDif * i + (j / sizeDif) * size + (i / widthMatrix) * size * (sizeDif-1) + j % (sizeDif);
            newMatrix[index] = matrix[i];
        }
    }
    return newMatrix;
}

std::vector<int> differenceMatrix(std::vector<int> matrix1, std::vector<int> matrix2)
{
    std::vector<int> differenceMatrix (matrix1.size());
    for (int i = 0; i < matrix1.size(); ++i)
        differenceMatrix[i] = matrix1[i] - matrix2[2];
    return differenceMatrix;
}

double work(int d1, int s1, int d2, int s2, int thread)
{
    omp_set_num_threads(thread);
    int g = d1 - d2;

    std::vector<int> matrix1 = createMatrix(d1, s1);
    std::vector<int> matrix2 = createMatrix(d2, s2);


    auto startCPUWork = clock();
    std::vector<int> matrixSizeDif = changeMatrix(matrix2, g, d2);
    std::vector<int> matrixDif = differenceMatrix(matrix1, matrixSizeDif);
    auto endCPUWork = clock();


    std::cout << "CPU rendering on " << thread << " thread with rendering parameters d1:" << d1 << " s1:" << s1 <<
         " d2:" << d2 << " s2:" << s2 << " time: " << ((double) (endCPUWork - startCPUWork)) / CLOCKS_PER_SEC << std::endl;
    return ((double) (endCPUWork - startCPUWork)) / CLOCKS_PER_SEC;
}

int main() {
    const int sizeMapThread = 15;
    const int sizeRepeat = 3;
    double result[sizeRepeat][sizeMapThread][sizeRepeat];
    int threadArray[sizeMapThread];
    for (int i = 0; i < sizeMapThread; i++)
    {
        threadArray[i] = i + 2;
    }
    baseMap zadanie {12, 12, 13, 12, 12, 13,
                     10, 9, 9, 10, 9, 9,};
    for (int l = 0; l < sizeRepeat; l++) {
        for (int i = 0; i < sizeMapThread; i++) {
            for (int j = 0; j < sizeRepeat; j++) {
                result[j][i][l] = work(zadanie.d1[j], zadanie.s1[j], zadanie.d2[j], zadanie.s2[j], threadArray[i]);
            }
        }
    }
    for (int l = 0; l < sizeRepeat; l++) {
        std::cout << "result for: " << zadanie.d1[l] << " " << zadanie.s1[l] << " " << zadanie.d2[l] << " " << zadanie.s2[l] << " " << std::endl;
        for (int i = 0; i < sizeMapThread; i++) {
            double sredTime = (result[l][i][0] + result[l][i][1] + result[l][i][2]) / 3;
            std::cout << sredTime << " average time for " << threadArray[i] << " thread" << std::endl;
        }
    }


    return 0;
}
