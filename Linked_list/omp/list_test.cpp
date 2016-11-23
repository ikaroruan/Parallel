#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>
#include "Linked_list.h"

void print_menu()
{
	std::cout << "1 - Push Front\n"
		  << "2 - Push Back\n"
		  << "3 - Remove\n"
		  << "4 - Pop Front\n"
		  << "5 - Pop Back\n"
		  << "6 - Print\n"
		  << "7 - Exit\n"
		  << "Your choice: ";
}

int main(int argc, char** argv)
{
	if(argc < 3){
		std::cerr << "Not enough arguments.\n";
		return 0;
	}
	std::string filename1 = argv[1];
	std::string filename2 = argv[2];
	std::ifstream input(filename1);
	Linked_list<int> list;
	int size = -1; int num = -1;

	input >> size;
	for(int i = 0; i < size; ++i){
		input >> num;
		list.push_back(num);
	}
	std::cout << "Inserted.\n";
	input.close();
	
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::duration<double> elapsed;

	std::ifstream in(filename2);
	in >> size;
	start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < size; ++i){
		in >> num;
		list.remove(num);
	}
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;

	std::cout << "Deleted.\n";
	std::cout << "Elapsed time = " << elapsed.count() << "s\n";

	if(list.size() > 0)
		std::cerr << "ERROR: There are remaining nodes.\n";
	//std::cout << "Printing:\n";
	//list.print();

	/*int option = -1;
	Linked_list<int> list;
	int num;

	while(option != 7){
		print_menu();
		std::cin >> option;
		std::cout << "\n";
		switch(option){
			case 1:
				std::cout << "\nNumber: ";
				std::cin >> num;
				std::cout << "\n";
				list.push_front(num);
				std::cout << "\nAdded!";
				break;
			case 2: 
				std::cout << "\nNumber: ";
				std::cin >> num;
				std::cout << "\n";
				list.push_back(num);
				std::cout << "\nAdded!";
				break;
			case 3:
				std::cout << "\nNumber: ";
				std::cin >> num;
				std::cout << "\n";
				list.remove(num);
				std::cout << "\nRemoved!";
				break;
			case 4:
				list.pop_front();
				std::cout << "\nPopped!";
				break;
			case 5:
				list.pop_back();
				std::cout << "\nPopped!";
				break;
			case 6:
				std::cout << "\nPrinting:";
				list.print();
				std::cout << "\n";
				break;
			default:
				while(option < 1 && option > 6){
					std::cout << "Try again: ";
					std::cin >> option;
					std::cout << "\n";
				}
				break;
		}
		std::cout << "\n============================\n";
	}*/
	
	return 0;
}
