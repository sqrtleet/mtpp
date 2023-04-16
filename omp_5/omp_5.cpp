#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <omp.h>

using namespace std;

struct Point3D {
    double x, y, z;
};

Point3D parallel(const vector<Point3D>& points, int num_threads) {
    Point3D center = { 0.0, 0.0, 0.0 };

#pragma omp parallel shared(points) num_threads(num_threads)
    {
        Point3D center_local = { 0.0, 0.0, 0.0 };
#pragma omp for
        for (int i = 0; i < points.size(); ++i) {
            center_local.x += points[i].x;
            center_local.y += points[i].y;
            center_local.z += points[i].z;
        }
#pragma omp critical
        {
            center.x += center_local.x;
            center.y += center_local.y;
            center.z += center_local.z;
        }
    }

    center.x /= points.size();
    center.y /= points.size();
    center.z /= points.size();

    return center;
}


Point3D functional(const vector<Point3D>& points, int num_threads) {
    Point3D center = { 0.0, 0.0, 0.0 };
    int total_points = points.size();

#pragma omp parallel num_threads(num_threads)
    {
        Point3D center_local = { 0.0, 0.0, 0.0 };
        int points_per_thread = total_points / num_threads;
        int thread_id = omp_get_thread_num();
        int start = thread_id * points_per_thread;
        int end = (thread_id == num_threads - 1) ? total_points : start + points_per_thread;

        for (int i = start; i < end; ++i) {
            center_local.x += points[i].x;
            center_local.y += points[i].y;
            center_local.z += points[i].z;
        }

#pragma omp critical
        {
            center.x += center_local.x;
            center.y += center_local.y;
            center.z += center_local.z;
        }
    }

    center.x /= total_points;
    center.y /= total_points;
    center.z /= total_points;

    return center;
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

    Point3D center_parallel = parallel(points, num_threads);
    cout << "Parallel: " << center_parallel.x << ", " << center_parallel.y << ", " << center_parallel.z << endl;

    Point3D center_functional = functional(points, num_threads);
    cout << "Functional decomposition: " << center_functional.x << ", " << center_functional.y << ", " << center_functional.z << endl;

    return 0;
}
