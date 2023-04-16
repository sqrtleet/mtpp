#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <omp.h>

using namespace std;

struct Point3D {
    double x, y, z;
};

double functional(const vector<Point3D>& points, int num_threads) {
    int N = points.size();
    double global_sum = 0;

#pragma omp parallel num_threads(num_threads)
    {
        double local_sum = 0;
        int points_per_thread = N / num_threads;
        int thread_id = omp_get_thread_num();
        int start = thread_id * points_per_thread;
        int end;
        if (thread_id == num_threads - 1) {
            end = points.size();
        }
        else {
            end = start + points_per_thread;
        }

        for (int i = start; i < end; ++i) {
            local_sum += sqrt(pow(points[i].x, 2) + pow(points[i].y, 2) + pow(points[i].z, 2));
        }

#pragma omp critical
        {
            global_sum += local_sum;
        }
    }

    return global_sum / N;
}

int main() {
    string filename = "input.txt";
    int num_threads = 8;

    vector<Point3D> points;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Point3D point;
        ss >> point.x >> point.y >> point.z;
        points.push_back(point);
    }

    file.close();

    double formula_result = functional(points, num_threads);
    cout << "Functional decomposition: " << formula_result << endl;

    return 0;
}
