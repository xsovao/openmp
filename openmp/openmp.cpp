// openmp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include "omp.h"

int main()
{

	std::vector<float> v = std::vector<float>();
	std::chrono::time_point<std::chrono::system_clock> start, end;
	double avg = 0.0;
	int n = 15000000;
	int maxx = 1000;
	int minn = 10;
	for (int i = 0; i < n; i++) {
		v.push_back((float)(rand() % maxx + minn));
	}
	std::chrono::duration<double> elapsed_seconds = end - start;

	/*
	std::cout << "serial avg \n";
	start = std::chrono::system_clock::now();

	for (int i = 0; i < n; i++) {
		avg += v[i];
	}
	end = std::chrono::system_clock::now();
	std::cout << avg / n << " \n";
	std::cout << elapsed_seconds.count() << " s\n";


	std::cout << "parallel avg \n";
	start = std::chrono::system_clock::now();
	avg = 0.0;
#pragma omp parallel for reduction(+:avg)
	for (int i = 0; i < n; i++) {
		avg += v[i];
	}
	
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	std::cout << avg / n << " \n";
	std::cout << elapsed_seconds.count() << " s\n";

	system("pause");

	std::cout << "serial min \n";
	double min = maxx+1;
	start = std::chrono::system_clock::now();
	for (int i = 0; i < n; i++) {
		if(v[i]<min)min = v[i];
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	std::cout << min << " \n";
	std::cout << elapsed_seconds.count() << " s\n";

	std::cout << "parallel min \n";
	start = std::chrono::system_clock::now();
	min = maxx+1;
#pragma omp parallel
	{
		double locmin = maxx+1;
#pragma omp parallel for
		for (int i = 0; i < n; i++) {
			if (v[i] < locmin)locmin = v[i];
		}
#pragma omp critical
		{
			if (locmin < min)min = locmin;
		}
	}

	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	std::cout << min << " \n";
	std::cout << elapsed_seconds.count() << " s\n";
	*/
	system("pause");

	std::vector<int> even;

	std::cout << "serial select even \n";
	start = std::chrono::system_clock::now();
	for (int i = 0; i < n; i++) {
		if ((int)v[i] % 2 == 0) even.push_back((int)v[i]);
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	std::cout << elapsed_seconds.count() << " s\n";
	even.clear();
	std::cout << "parallel select even \n";
	start = std::chrono::system_clock::now();
	
#pragma omp parallel
	{
		std::vector<int> loceven;
#pragma omp for 
		for (int i = 0; i < n; i++) {
			if ((int)v[i] % 2 == 0) loceven.push_back((int)v[i]);
		}

		/*UNORDERED
#pragma omp critical
		{
			for (int i = 0; i < loceven.size(); i++) {
				even.push_back(loceven[i]);
			}}*/

		/*ORDERED*/
#pragma omp for ordered schedule(static,1)
		for (int i = 0; i < omp_get_num_threads(); i++) {

#pragma omp ordered
			{
				for (int i = 0; i < loceven.size(); i++) {
					even.push_back(loceven[i]);
				}
			}
		}
	}

	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	std::cout << elapsed_seconds.count() << " s\n";

	system("pause");

	return 0;
}



