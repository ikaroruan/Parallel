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
	_number_of_threads = omp_get_max_threads();
	_thread_size = new int[_number_of_threads];
	_tbegin = new List_node<T>*[_number_of_threads + 1];
	_rebalance_factor = 0;
	_time_parallel = 0;
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
double Linked_list<T>::time_parallel()
{
	return _time_parallel;
}

template<typename T>
void Linked_list<T>::update_info()
{
	int r = size() % _number_of_threads;
	int range = (size() - r)/_number_of_threads;
	for(int i = 0; i < _number_of_threads; ++i)
		_thread_size[i] = range + (i < r ? 1 : 0);

	for(int i = 0; i < _number_of_threads + 1; ++i)
		_tbegin[i] = nullptr;
	
	List_node<T>* p = front();
	int tid = 0; _tbegin[tid] = front(); int sum = _thread_size[tid]; 
	for(int i = 0; i < size(); ++i){
		if(i == sum){
			_tbegin[++tid] = p;
			sum = sum + (tid < _number_of_threads ? _thread_size[tid] : 0);
		}
		p = p->get_next();
	}
	_rebalance_factor = size() - range;
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
	update_info();
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
	update_info();
}

template<typename T>
void Linked_list<T>::insert(T value)
{
	List_node<T>* new_node = new List_node<T>(value);
	insert(new_node);
}

template<typename T>
int Linked_list<T>::check_insertion_type(List_node<T>* node)
{
	if(size() == 0)
		return (int)FIRST;
	if(node->get_value() <= front()->get_value())
		return (int)FRONT;
	if(node->get_value() >= back()->get_value())
		return (int)BACK;
	
	return (int)MIDDLE;
}

template<typename T>
void Linked_list<T>::insert(List_node<T>* new_node)
{
	Insertion_type tp = static_cast<Insertion_type>(check_insertion_type(new_node));

	switch(tp){
		case FIRST:
			insert_first(new_node);
			break;
		case FRONT:
			insert_front(new_node);
			break;
		case MIDDLE:
			insert_middle(new_node);
			break;
		case BACK:
			insert_back(new_node);
			break;
	}

	size(size() + 1);
}

template<typename T>
void Linked_list<T>::insert_first(List_node<T>* new_node)
{
	front(new_node);
	back(new_node);
}

template<typename T>
void Linked_list<T>::insert_front(List_node<T>* new_node)
{
	front()->set_previous(new_node);
	new_node->set_next(front());
	front(new_node);
}

template<typename T>
void Linked_list<T>::insert_middle(List_node<T>* new_node)
{
	List_node<T>* current_node = front();
	
	// Find node to insert before it.
	while(current_node->get_value() < new_node->get_value()){
		current_node = current_node->get_next();
	}
	
	current_node->get_previous()->set_next(new_node);
	new_node->set_previous(current_node->get_previous());
	current_node->set_previous(new_node);
	new_node->set_next(current_node);
}

template<typename T>
void Linked_list<T>::insert_back(List_node<T>* new_node)
{
	back()->set_next(new_node);
	new_node->set_previous(back());
	back(new_node);
}

template<typename T>
void Linked_list<T>::insert(std::vector<T> v)
{
 	typedef typename std::vector<T>::iterator Vector_iterator;
	for(Vector_iterator it = v.begin(); it != v.end(); ++it){
		insert(*(it));
	}
}

template<typename T>
void Linked_list<T>::parallel_insert(std::vector<T> v)
{
	// Inserting a chunk sequentially
	int sequential_size = 0.2*v.size();
	for(int i = 0; i < sequential_size; ++i)
		insert(v[i]);
	
	// Dividing work among threads
	int tbegin[_number_of_threads+1];
	int tsize = (sequential_size - v.size())/_number_of_threads;
	int remainder = (sequential_size - v.size())%_number_of_threads;
	for(int i = 0; i < _number_of_threads; ++i){
		_thread_size[i] = tsize + (i < remainder ? 1 : 0);
		tbegin[i] = (i == 0 ? 0 : (tbegin[i-1] + _thread_size[i-1]));
	}
	tbegin[_number_of_threads] = v.size();

	// Beginning of Parallel Region.
	#pragma omp parallel shared(tbegin) num_threads(_number_of_threads)
	{
		int tid = omp_get_thread_num();
		std::queue<List_node<T>*> queue;

		for(int i = tbegin[tid]; i < tbegin[tid+1]; ++i){
			List_node<T>* new_node = new List_node<T>(v[i]);
			if(!try_parallel_insert(new_node))
				queue.push(new_node);
		}

		while(!queue.empty()){
			List_node<T>* to_insert = queue.front();
			queue.pop();
			if(!try_parallel_insert(to_insert))
				queue.push(to_insert);
		}
	}// End of Parallel Region.
}

template<typename T>
bool Linked_list<T>::parallel_insert_middle(List_node<T>* new_node)
{
	List_node<T>* current_node = front();

	if(!new_node->try_lock())
		return false;
	if(!current_node->try_lock()){
		new_node->unlock();
		return false;
	}
	
	// Find node to insert before it.
	while(current_node->get_value() < new_node->get_value()){
		if(!current_node->get_next()->try_lock()){
			current_node->unlock();
			new_node->unlock();
			return false;
		}
		current_node = current_node->get_next();
		current_node->get_previous()->unlock();
	}

	if(!current_node->get_previous()->try_lock()){
		current_node->unlock();
		new_node->unlock();
		return false;
	}
	
	current_node->get_previous()->set_next(new_node);
	new_node->set_previous(current_node->get_previous());
	current_node->set_previous(new_node);
	new_node->set_next(current_node);

	new_node->get_previous()->unlock();
	new_node->unlock();
	new_node->get_next()->unlock();

	return true;
}

template<typename T>
bool Linked_list<T>::try_parallel_insert(List_node<T>* new_node)
{
	Insertion_type tp = static_cast<Insertion_type>(check_insertion_type(new_node));
	List_node<T>* to_unlock = nullptr;

	switch(tp){
		case FIRST:
			// Should never get here.
			std::cerr << "ERROR: Trying to insert first again. (parallel_insert)\n";
			break;
		case FRONT:
			if(!front()->try_lock())
				return false;
			else{
				to_unlock = front();
				insert_front(new_node);
				to_unlock->unlock();
			}
			break;
		case MIDDLE:
			if(!parallel_insert_middle(new_node))
				return false;
			break;
		case BACK:
			if(!back()->try_lock())
				return false;
			else{
				to_unlock = back();
				insert_back(new_node);
				to_unlock->unlock();
			}
			break;
	}
	
	#pragma omp critical (size_increment)
	{
		size(size() + 1);
	}

	return true;
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
	update_info();
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
	update_info();
}

template<typename T>
void Linked_list<T>::remove(T value)
{
	List_node<T>* node = find(value);
	remove(node);

}

template<typename T>
void Linked_list<T>::remove(List_node<T>* node)
{
	if(node != nullptr){
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
	List_node<T>* node[_number_of_threads];
	List_node<T>* ans = nullptr;
	int tid;
	List_node<T>* current;

	double start = omp_get_wtime();
	//start = std::chrono::high_resolution_clock::now();
	#pragma omp parallel shared(node, value) private(tid, current) num_threads(_number_of_threads)
	{
	 	tid = omp_get_thread_num();
 		node[tid] = nullptr;
		current = _tbegin[tid];
		
		for(int i = 0; i < _thread_size[tid]; ++i){
			if(current != nullptr){
				if(current->get_value() == value){
					node[tid] = current;
				}
			current = current->get_next();
			}
		}
	}
	double end = omp_get_wtime();
	//end = std::chrono::high_resolution_clock::now();
	//elapsed = end - start;
	_time_parallel = _time_parallel + (end - start);
	
	for(int i = 0; i < _number_of_threads; ++i){
		if(node[i] != nullptr){
			ans = node[i];
			break;
		}
	}
	//if(size() < _rebalance_factor)
	//	update_info();
	return ans;
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
