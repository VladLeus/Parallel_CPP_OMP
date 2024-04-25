#include <iostream>
#include <limits>
#include <omp.h>
#include <random>
#include <vector>

int calculateTotalSum(const std::vector<std::vector<int>> &array) {
    int sum = 0;
#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < array.size(); i++) {
        for (int j = 0; j < array[i].size(); j++) {
            sum += array[i][j];
        }
    }
    return sum;
}

std::pair<int, int> findRowWithMinSum(const std::vector<std::vector<int>> &array) {
    int minimum = std::numeric_limits<int>::max();
    int minRowIndex = -1;
#pragma omp parallel for
    for (int i = 0; i < array.size(); i++) {
        int rowSum = 0;
        for (int j = 0; j < array[i].size(); j++) {
            rowSum += array[i][j];
        }
        if (rowSum < minimum) {
#pragma omp critical
            {
                if (rowSum < minimum) {
                    minimum = rowSum;
                    minRowIndex = i;
                }
            }
        }
    }
        return {minRowIndex, minimum};
}

std::vector<std::vector<int>> randomArrData(int rows, int cols) {
    std::vector<std::vector<int>> data(rows, std::vector<int>(cols));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(-1000, 1000);

    for (auto &row : data) {
        for (auto &elem : row) {
            elem = distrib(gen);
        }
    }
    return data;
}

int main() {
    int rows = 10000;
    int cols = 10000;

    std::vector<std::vector<int>> array = randomArrData(rows, cols);

    int totalSum;
    std::pair<int, int> minRowSum;

    double start_time = omp_get_wtime();

    omp_set_nested(1);

#pragma omp parallel sections
    {
#pragma omp section
        { totalSum = calculateTotalSum(array); }
#pragma omp section
        { minRowSum = findRowWithMinSum(array); }
    }

    double time_elapsed = omp_get_wtime() - start_time;

    std::cout << "Total Sum of array elements is: " << totalSum << std::endl;
    std::cout << "Row with minimum sum is: " << minRowSum.first + 1
              << " where the sum is: " << minRowSum.second << std::endl;
    std::cout << "Time elapsed: " << time_elapsed << " seconds." << std::endl;

    return 0;
}