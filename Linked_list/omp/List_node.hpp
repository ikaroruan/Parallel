// File: List_node.hpp
// Author: Ikaro Costa
// Date: 11/13/2016
// Implementation of doubly linked list node access and mofiers.

template<typename T>
List_node<T>::List_node()
{
	_value = 0;
	_next = nullptr;
	_previous = nullptr;
}

template<typename T>
List_node<T>::~List_node()
{

}

template<typename T>
void List_node<T>::set_value(T value)
{
	_value = value;
}

template<typename T>
T List_node<T>::get_value()
{
	return _value;
}

template<typename T>
void List_node<T>::set_next(List_node<T>* next)
{
	_next = next;
}

template<typename T>
List_node<T>* List_node<T>::get_next()
{
	return _next;
}

template<typename T>
void List_node<T>::set_previous(List_node<T>* previous)
{
	_previous = previous;
}

template<typename T>
List_node<T>* List_node<T>::get_previous() 
{
	return _previous;
}
