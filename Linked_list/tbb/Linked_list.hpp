// File: Linked_list.hpp
// Author: Ikaro Costa
// Date: 11/16/2016
// Implementation of Linked list member functions.

template<typename T>
Linked_list<T>::Linked_list()
{
	_size = 0;
	_front = nullptr;
	_back = nullptr;
}

template<typename T>
Linked_list<T>::~Linked_list()
{
	while(size() > 0){
		pop_front();
	}
}

template<typename T>
int Linked_list<T>::size() 
{
	return _size;
}

template<typename T>
void Linked_list<T>::size(int num)
{
	_size = num;
}

template<typename T>
List_node<T>* Linked_list<T>::front() 
{
	return _front;
}

template<typename T>
void Linked_list<T>::front(List_node<T>* node)
{
	_front = node;
}

template<typename T>
List_node<T>* Linked_list<T>::back() 
{
	return _back;
}

template<typename T>
void Linked_list<T>::back(List_node<T>* node)
{
	_back = node;
}

template<typename T>
void Linked_list<T>::push_back(T value)
{
	List_node<T>* node = new List_node<T>();
	node->set_value(value);
	push_back(node);
}

template<typename T>
void Linked_list<T>::push_back(List_node<T>* node)
{
	if(size() == 0){
		back(node); front(node);
	}
	else{
		back()->set_next(node);
		node->set_previous(back());
		back(node);
	}	
	size(size() + 1);
}

template<typename T>
void Linked_list<T>::push_front(T value)
{
	List_node<T>* node = new List_node<T>();
	node->set_value(value);
	push_front(node);
}

template<typename T>
void Linked_list<T>::push_front(List_node<T>* node)
{
	if(size() == 0){
		back(node); front(node);
	}
	else{
		front()->set_previous(node);
		node->set_next(front());
		front(node);
	}
	size(size() + 1);
}

template<typename T>
void Linked_list<T>::pop_back()
{
	List_node<T>* node = back();
	if(size() > 1){
		back(back()->get_previous());
		back()->set_next(nullptr);
		delete node; size(size() - 1);
	}
	else{
		delete node; size(size() - 1);
		front(nullptr); back(nullptr);
	}
}

template<typename T>
void Linked_list<T>::pop_front()
{
	List_node<T>* node = front();
	if(size() > 1){
		front(front()->get_next());
		front()->set_previous(nullptr);
		delete node; size(size() - 1);
	}
	else if(size() == 1){
		delete node; size(size() - 1);
		front(nullptr); back(nullptr);
	}
}

template<typename T>
void Linked_list<T>::remove(T value)
{
	List_node<T>* node = find(value);
	/*if(node == nullptr)
		std::cout << "NULL!\n";
	else if(node->get_value() != value)
		std::cout << "WRONG!\n";
	else
		std::cout << "OK!\n";*/
	remove(node);

	/*if(node != nullptr){
		if(size() == 1){
			front(nullptr); back(nullptr);
		}
		else if(node == front()){
			node->get_next()->set_previous(nullptr);
			front(node->get_next());
		}
		else if(node == back()){
			node->get_previous()->set_next(nullptr);
			back(node->get_previous());
		}
		else{
			node->get_previous()->set_next(node->get_next());
			node->get_next()->set_previous(node->get_previous());
		}
		delete node; size(size() - 1);
	}*/
}

template<typename T>
void Linked_list<T>::remove(List_node<T>* node)
{
	if(is_element(node)){
		if(size() == 1){
			front(nullptr); back(nullptr);
		}
		else if(node == front()){
			node->get_next()->set_previous(nullptr);
			front(node->get_next());
		}
		else if(node == back()){
			node->get_previous()->set_next(nullptr);
			back(node->get_previous());
		}
		else{
			node->get_previous()->set_next(node->get_next());
			node->get_next()->set_previous(node->get_previous());
		}
		delete node; size(size() - 1);
	}
}

template<typename T>
List_node<T>* Linked_list<T>::index(int pos)
{
	List_node<T>* current = front();
	for(int i = 0; i < pos; ++i){
		if(current != nullptr)
			current = current->get_next();
	}

	return current;
}

template<typename T>
List_node<T>* Linked_list<T>::find(T value)
{
	List_node<T>* node = nullptr;

	/*while(current != nullptr){
		if(current->get_value() == value){
			//if(node == nullptr){
				node = current;
			//}
			}
		current = current->get_next();
	}*/
	List_node<T>* current = nullptr;

	tbb::parallel_for(0, size(), [=, &current, &node](int j){
	for(int i = 0; i < size(); ++i){
		current = index(i);
		if(current != nullptr){
			if(current->get_value() == value){
				//m.lock();
				node = current;
				//m.unlock();
				//std::cout << "Got here!\n";
				//std::cout << "    value=" << value << "   node_value=" << node->get_value() << std::endl;
			}
		}
	}});
	//if(node != nullptr)
	//	std::cout << "    value=" << value << "   node_value=" << node->get_value() << std::endl;
	return node;
}

template<typename T>
List_node<T>* Linked_list<T>::find(List_node<T>* node)
{
	List_node<T>* current = front();
	List_node<T>* to_return = nullptr;

	while(current != nullptr){
		if(current == node)
			to_return = current;
		current = current->get_next();
	}	

	return to_return;
}

template<typename T>
bool Linked_list<T>::is_element(T value)
{
	if(find(value) == nullptr)
		return false;
	return true;
}

template<typename T>
bool Linked_list<T>::is_element(List_node<T>* node)
{
	if(find(node) == nullptr)
		return false;
	return true;
}

template<typename T>
void Linked_list<T>::print()
{
	List_node<T>* current = front();
	while(current != nullptr){
		std::cout << current->get_value() << " ";
		current = current->get_next();
	}
}
