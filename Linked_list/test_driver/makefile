test_driver: test_driver.o
	g++ -g -Wall -std=c++11 -pthread -fopenmp test_driver.o -o test_driver

test_driver.o: test_driver.cpp Linked_list_sequential.h Linked_list_sequential.hpp Linked_list_cppthreads.h Linked_list_cppthreads.hpp Linked_list_omp.h Linked_list_omp.hpp
	g++ -g -Wall -std=c++11 -pthread -fopenmp -c test_driver.cpp

clean:
	rm test_driver *.o
	@echo Clean done.
