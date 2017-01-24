#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

#include "Linked_list_sequential.h"
#include "Linked_list_cppthreads.h"
#include "Linked_list_omp.h"

int main(int argc, char** argv)
{
	if(argc < 2){
		std::cout << "Not enough arguments.\n";
		return 0;
	}

	// Handling files for reading numbers 
	std::string filename1 = argv[1];
	std::string filename2 = filename1.substr(0, filename1.find_last_of(".")) + "s.txt";
	std::ifstream input1(filename1);
	std::ifstream input2(filename2);

	// Vectors will have same numbers in different orders
	std::vector<int> v1, v2;
	int size1, size2;
	int num1, num2;
	input1 >> size1; input2 >> size2;
	for(int i = 0; i < size1; ++i){
		input1 >> num1;
		v1.push_back(num1);
		input2 >> num2;
		v2.push_back(num2);
	}

	// Declaring timers and lists
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::duration<double> elapsed;
	double tseq, tcpp, tomp;
	Linked_list_sequential<int> lseq;
	Linked_list_cppthreads<int> lcpp;
	Linked_list_omp<int> lomp;
	
	// Inserting numbers on the list
	for(int i = 0; i < size1; ++i){
		lseq.push_back(v1[i]);
		lcpp.push_back(v1[i]);
		lomp.push_back(v1[i]);
	}

	// SEQUENTIAL 
	start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < size2; ++i){
		lseq.remove(v2[i]);
	}
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	tseq = elapsed.count();
	std::cout << "Sequential done.\n";

	// C++ THREADS
	start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < size2; ++i){
		lcpp.remove(v2[i]);
	}
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	tcpp = elapsed.count();
	std::cout << "C++ Threads done.\n";

	// OPENMP
	start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < size2; ++i){
		lomp.remove(v2[i]);
	}
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	tomp = elapsed.count();
	std::cout << "OpenMP done.\n";

	// Printing results
	std::cout << "\n";
	std::cout << std::left << std::setw(15) << std::setfill(' ') << "Sequential" 
		  << std::left << std::setw(15) << std::setfill(' ') << "C++ Threads"
		  << "OpenMP\n";
	std::cout << std::left << std::setw(15) << std::setfill(' ') << tseq 
		  << std::left << std::setw(15) << std::setfill(' ') << tcpp
		  << tomp << "\n";
	std::cout << std::left << std::setw(15) << std::setfill(' ') << "   Speedup" 
		  << std::left << std::setw(15) << std::setfill(' ') << tseq/tcpp
		  << tseq/tomp << "\n";

	
	return 0;
}
