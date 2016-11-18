#include <iostream>
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

int main()
{

	int option = -1;
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
	}
	
	return 0;
}
