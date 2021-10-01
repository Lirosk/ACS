#include <omp.h>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <random>
#include <chrono>

using namespace std;

const int NUM_THREADS = 8;
const int N = 1<<10;
const int M = 1<<10;
const int CHUNK = M/NUM_THREADS;

void MxV(vector<vector<int>>& m, vector<int> v)
{
	int d1 = m.size();
	int d2 = m[0].size();
    exception smth_about_size;

	for (int i = 1; i < d1; i++)
	{
		if (m[i].size() != d2)
		{
			throw smth_about_size;
		}
	}
	if (v.size() != d2)
	{
		throw smth_about_size;
	}

    int i, j;

	#pragma omp parallel for schedule(static, CHUNK) shared(m, v) private(i, j)
    for (i = 0; i < d1; i++)
    {
        for (j = 0; j < d2; j++)
        {
            m[i][j] *= v[j];
        }
    }
}

int main()
{
	srand(time(0));

	vector<vector<int>> m = vector<vector<int>>();
	vector<int> v = vector<int>();

	for (int i = 0; i < N; i++)
	{
		m.push_back(vector<int>());
		for (int j = 0; j < M; j++)
		{
			m[i].push_back(rand()%1000);
		}
	}
	for (int j = 0; j < M; j++)
	{
		v.push_back(rand()%1000);
	}

    for (auto current_num_threads: {NUM_THREADS, 1})
    {
        omp_set_num_threads(current_num_threads);
        auto start = chrono::high_resolution_clock::now();
        
        MxV(m, v);

        auto end = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
		long long count = duration.count();

        printf("time: %d mcs\nnum of threads: %d\n\n", count, current_num_threads);
    }

}
