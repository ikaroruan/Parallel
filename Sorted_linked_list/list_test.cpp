#include <iostream>
#include <cstdlib>
#include <chrono>
#include "Linked_list.h"

void print_menu()
{
	std::cout << "\n===================\n"
		  << "1 - Insert\n"
		  << "2 - Print\n"
		  << "3 - Exit\n"
		  << "Your choice: ";
}

int main(int argc, char** argv)
{
	Linked_list<int> list_sequential;
	Linked_list<int> list_parallel;
	double stime, ptime;
	std::vector<int> v;
	srand(1);

	for(int i = 0; i < 1000; ++i){
		v.push_back(rand() % 10000);
	}

	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::duration<double> elapsed;

	start = std::chrono::high_resolution_clock::now();
	list_sequential.insert(v);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	stime = elapsed.count();

	std::cout << "Sequential insertion done.\n";
	std::cout << "Elapsed time = " << stime << "\n\n";

	start = std::chrono::high_resolution_clock::now();
	list_parallel.parallel_insert(v);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	ptime = elapsed.count();

	std::cout << "Parallel insertion done.\n";
	std::cout << "Elapsed time = " << ptime << "\n";



	/*Linked_list<int> list;
	int choice = 0;
	int num;

	while(choice != 3){
		print_menu();
		std::cin >> choice;
		switch(choice){
			case 1: 
				std::cout << "Number to be inserted: ";
				std::cin >> num;
				list.insert(num);
				break;
			case 2:
				std::cout << "Printing:\n";
				list.print();
				std::cout << "\n";
				break;
			case 3:
				choice = 3;
				break;
			default:
				std::cout << "Error. Try again: ";
				std::cin >> choice;
				break;
		}
	}// End of while*/

	return 0;

}
