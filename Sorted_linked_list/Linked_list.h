// File: Linked_list.h
// Author: Ikaro Costa
// Date: 11/13/2016
// Header file for a Doubly linked list to be parallelized in the future.

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <vector>
#include <queue>
#include "List_node.h"
#include <omp.h>
#include <chrono>

template<typename T>
class Linked_list
{
	public:

	typedef enum Insertion_type{
		FRONT, MIDDLE, BACK, FIRST
	} Insertion_type;
	
	// Constructor and Destructor.
	Linked_list();
	~Linked_list();

	// PRE: Gets an value of type T or a node List_node.
	// POST: Push values or elements on the linked list.
	// RETURN: none.
	void push_back(List_node<T>* node);
	void push_front(List_node<T>* node);
	void push_back(T value);
	void push_front(T value);
	void insert(T value);
	void insert(std::vector<T> v);
	void insert(List_node<T>* new_node);
	void parallel_insert(std::vector<T> v);
	void insert_first(List_node<T>* new_node);
	void insert_front(List_node<T>* new_node);
	void insert_middle(List_node<T>* new_node);
	void insert_back(List_node<T>* new_node);
	bool parallel_insert_middle(List_node<T>* new_node);
	bool try_parallel_insert(List_node<T>* new_node);
	int check_insertion_type(List_node<T>* node);

	// PRE: none.
	// POST: Pops nodes or elements from the list.
	// RETURN: none.
	void pop_back();
	void pop_front();

	// PRE: Node or value to be removed.
	// POST: Finds and remove the node or value from the list.
	// RETURN: none.
	void remove(List_node<T>* node);
	void remove(T value);

	// PRE: Value or node to be found on the list.
	// POST: Searches the node or value on the list.
	// RETURN: Returns the respective value or node it is on the list, and nullptr otherwise.
	List_node<T>* find(T value);
	List_node<T>* find(List_node<T>* node);

	// PRE: Value or node to be checked if it is on the list.
	// POST: Uses find member function to check if it is on the list.
	// RETURN: False if find function return nullptr, and true otherwise.
	bool is_element(List_node<T>* node);
	bool is_element(T value);

	void update_info();

	// Access the member variables.
	int size();
	List_node<T>* index(int pos);
	List_node<T>* front();
	List_node<T>* back();
	void print();
	double time_parallel();
	
	private:
	int _size;
	List_node<T>* _front;
	List_node<T>* _back;
	int _number_of_threads;
	int* _thread_size;
	List_node<T>** _tbegin;
	omp_lock_t m;
	int _rebalance_factor;
	double _time_parallel;
	
	// Modifiers for the member variables.
	void size(int num);
	void front(List_node<T>* node);
	void back(List_node<T>* node);
};

#include "Linked_list.hpp"

#endif // LINKED_LIST_H
