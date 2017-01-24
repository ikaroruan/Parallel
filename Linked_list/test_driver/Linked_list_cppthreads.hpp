// File: Linked_list_cppthreads.hpp
// Author: Ikaro Costa
// Date: 11/16/2016
// Implementation of Linked list member functions.

template<typename T>
Linked_list_cppthreads<T>::Linked_list_cppthreads()
{
	_size = 0;
	_front = nullptr;
	_back = nullptr;
	_thread_size = new int[NUMBER_OF_THREADS];
	_tbegin = new List_node<T>*[NUMBER_OF_THREADS + 1];
	_rebalance_factor = 0;
}

template<typename T>
Linked_list_cppthreads<T>::~Linked_list_cppthreads()
{
	while(size() > 0){
		pop_front();
	}
	delete _thread_size;
}

template<typename T>
int Linked_list_cppthreads<T>::size() 
{
	return _size;
}

template<typename T>
void Linked_list_cppthreads<T>::size(int num)
{
	_size = num;
}

template<typename T>
List_node<T>* Linked_list_cppthreads<T>::front() 
{
	return _front;
}

template<typename T>
void Linked_list_cppthreads<T>::front(List_node<T>* node)
{
	_front = node;
}

template<typename T>
List_node<T>* Linked_list_cppthreads<T>::back() 
{
	return _back;
}

template<typename T>
void Linked_list_cppthreads<T>::back(List_node<T>* node)
{
	_back = node;
}

template<typename T>
void Linked_list_cppthreads<T>::update_info()
{
	int r = size() % NUMBER_OF_THREADS;
	int thread_work_size = (size() - r)/NUMBER_OF_THREADS;
	for(int i = 0; i < NUMBER_OF_THREADS; ++i)
		_thread_size[i] = thread_work_size + (i < r ? 1 : 0);

	for(int i = 0; i < NUMBER_OF_THREADS + 1; ++i)
		_tbegin[i] = nullptr;

	List_node<T>* p = front(); 
	int tid = 0; _tbegin[tid] = p; int sum = _thread_size[tid];
	for(int i = 0; i < size(); ++i){
		if(i == sum){
			_tbegin[++tid] = p;
			sum = sum + (tid < NUMBER_OF_THREADS ? _thread_size[tid] : 0);
		}
		p = p->get_next();
	}
	_tbegin[NUMBER_OF_THREADS] = nullptr;
	_rebalance_factor = size() - thread_work_size;
}

template<typename T>
void Linked_list_cppthreads<T>::insert(std::vector<T>& v)
{	
	Linked_list_cppthreads* list = new Linked_list_cppthreads<T>[NUMBER_OF_THREADS];
	std::thread t[NUMBER_OF_THREADS];
	int tindex[NUMBER_OF_THREADS + 1];
	
	// Calculating thread work size and indices
	int r = v.size() % NUMBER_OF_THREADS;
	int range = (v.size() - r)/NUMBER_OF_THREADS;
	tindex[0] = 0;
	for(int i = 1; i < NUMBER_OF_THREADS; ++i){
		tindex[i] = tindex[i-1] + range + (i < r ? 1 : 0);
	}
	tindex[NUMBER_OF_THREADS] = v.size();
	
	// Insert elements on each thread's list
	for(int tid = 0; tid < NUMBER_OF_THREADS; ++tid){
		t[tid] = std::thread([=, &list](){
			for(int i = tindex[tid]; i < tindex[tid+1]; ++i){
				list[tid].push_back(v[i]);
			}
		});
	}
	
	// Joining threads.
	for(int tid = 0; tid < NUMBER_OF_THREADS; tid++){
		if(t[tid].joinable())
			t[tid].join();
	}

	// Concatenating the lists
	// TODO: Concatenate lists when size != 0 
	for(int i = 0; i < NUMBER_OF_THREADS-1; ++i){
		if(i+1 < NUMBER_OF_THREADS){
			list[i].back()->set_next(list[i+1].front());
			list[i+1].front()->set_previous(list[i].back());
		}
	}
	front(list[0].front());
	back(list[NUMBER_OF_THREADS-1].back());
	size(size() + v.size());
	update_info();
}

template<typename T>
void Linked_list_cppthreads<T>::push_back(T value)
{
	List_node<T>* node = new List_node<T>();
	node->set_value(value);
	push_back(node);
}

template<typename T>
void Linked_list_cppthreads<T>::push_back(List_node<T>* node)
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
void Linked_list_cppthreads<T>::push_front(T value)
{
	List_node<T>* node = new List_node<T>();
	node->set_value(value);
	push_front(node);
}

template<typename T>
void Linked_list_cppthreads<T>::push_front(List_node<T>* node)
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
void Linked_list_cppthreads<T>::pop_back()
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
void Linked_list_cppthreads<T>::pop_front()
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
void Linked_list_cppthreads<T>::remove(T value)
{
	List_node<T>* node = find(value);
	/*if(node == nullptr)
		std::cout << "NULL!\n";
	else if(node->get_value() == value)
		std::cout << "OK!\n";
	else
		std::cout << "WRONG!\n";*/
	remove(node);
}

template<typename T>
void Linked_list_cppthreads<T>::remove(List_node<T>* node)
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
	if(size() < _rebalance_factor)
		update_info();
}

template<typename T>
List_node<T>* Linked_list_cppthreads<T>::index(int pos)
{
	List_node<T>* current = front();
	for(int i = 0; i < pos; ++i){
		if(current != nullptr)
			current = current->get_next();
	}

	return current;
}

template<typename T>
List_node<T>* Linked_list_cppthreads<T>::find_parallel(T value, List_node<T>* start, List_node<T>* end)
{
	List_node<T>* current = start;
	List_node<T>* node = nullptr;

	while(current != end){
		if(current == nullptr) break;
		if(current->get_value() == value){
				node = current;
		}
		current = current->get_next();
	}

	return node;
}

template<typename T>
List_node<T>* Linked_list_cppthreads<T>::find(T value)
{
	std::thread t[NUMBER_OF_THREADS];
	List_node<T>* node[NUMBER_OF_THREADS];
	List_node<T>* ans = nullptr;
	
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::duration<double> elapsed;

	// Starting parallel region.
	start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < NUMBER_OF_THREADS; ++i){
		node[i] = nullptr;
		t[i] = std::thread([=, &node](){ node[i] = find_parallel(value, _tbegin[i], _tbegin[i+1]);});
	}

	// Joining threads and checking if the value was found.
	for(int i = 0; i < NUMBER_OF_THREADS; ++i){
		if(t[i].joinable())
			t[i].join();
		if(node[i] != nullptr)
			ans = node[i];
	}
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	time_parallel = time_parallel + elapsed.count();
	
	return ans;
}

template<typename T>
List_node<T>* Linked_list_cppthreads<T>::find(List_node<T>* node)
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
bool Linked_list_cppthreads<T>::is_element(T value)
{
	if(find(value) == nullptr)
		return false;
	return true;
}

template<typename T>
bool Linked_list_cppthreads<T>::is_element(List_node<T>* node)
{
	if(find(node) == nullptr)
		return false;
	return true;
}

template<typename T>
void Linked_list_cppthreads<T>::print()
{
	List_node<T>* current = front();
	while(current != nullptr){
		std::cout << current->get_value() << " ";
		current = current->get_next();
	}
}
